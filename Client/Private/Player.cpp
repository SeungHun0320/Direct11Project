#include "Player.h"

#include "GameInstance.h"

#include "PlayerState.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPawn{pDevice, pContext}
{
}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CPawn(Prototype)
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	m_fStaminaRecoveryPerSec = 20.f;
	m_fMaxStamina = 100.f;
	m_fStamina = m_fMaxStamina;

	Change_States(STATES::IDLE);
	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CPlayer::Update(_float fTimeDelta)
{
	if (m_bDead)
		return LIFE::DEAD;

	Key_Input(fTimeDelta);
	Stamina_Recovery(fTimeDelta);

	if (m_eCurState != m_ePreState)
	{
		m_pCurState->Enter(fTimeDelta);
		m_ePreState = m_eCurState;
	}

	m_pCurState->Execute(fTimeDelta);

	return LIFE::NONE;
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CPlayer::Render()
{
	return __super::Render();
}

void CPlayer::Change_States(STATES eStates)
{
	if (m_pCurState)
		m_pCurState->Exit();

	if (nullptr != m_pCurState)
		Safe_Release(m_pCurState);

	m_pCurState = m_pStates[ENUM_CLASS(eStates)];
	Safe_AddRef(m_pCurState);
	
	m_eCurState = eStates;
}

_bool CPlayer::Play_Animation(_float fTimeDelta)
{
	return m_pModelCom->Play_Animation(fTimeDelta);
}

void CPlayer::Change_Animation(_uint iNextIndex, _bool isLoop, _float fBlendDuration, _bool isBlend)
{
	m_pModelCom->Change_Animation(iNextIndex, isLoop, fBlendDuration, isBlend);
}

void CPlayer::Dodge(_fvector vDir, _float fTimeDelta)
{
	m_pTransformCom->Set_SpeedPerSec(9.f);
	m_pTransformCom->LookDir(vDir);
	m_pTransformCom->Go_Dir(vDir, fTimeDelta);
}

void CPlayer::Move(_fvector vDir, _float fTimeDelta, _float fSpeed)
{
	m_pTransformCom->Set_SpeedPerSec(fSpeed);
	m_pTransformCom->LookDirLerp(vDir, fTimeDelta, fSpeed * 1.5f);
	m_pTransformCom->Go_Dir(vDir, fTimeDelta);
}


_vector CPlayer::Get_State(STATE eState)
{
	return m_pTransformCom->Get_State(eState);
}

_bool CPlayer::IsKeyDown(_ubyte eKeyID)
{
	return KEY_DOWN(eKeyID);
}

_bool CPlayer::IsKeyPressing(_ubyte eKeyID)
{
	return KEY_PRESSING(eKeyID);
}

_bool CPlayer::IsKeyUp(_ubyte eKeyID)
{
	return KEY_UP(eKeyID);
}

_bool CPlayer::IsAnyMoveKeyPressed() const
{
	return KEY_PRESSING(DIK_W) || KEY_PRESSING(DIK_A) ||
		KEY_PRESSING(DIK_S) || KEY_PRESSING(DIK_D) ||
		KEY_PRESSING(DIK_SPACE);
}

void CPlayer::Key_Input(_float fTimeDelta)
{
	//if (KEY_DOWN(DIK_SPACE))
	//	m_pModelCom->Change_Animation(FAIL_DODGE, false, 0.3f);
}

void CPlayer::Stamina_Recovery(_float fTimeDelta)
{
	m_fStamina += m_fStaminaRecoveryPerSec * fTimeDelta;
	m_fStamina = min(m_fStamina, m_fMaxStamina);
}

HRESULT CPlayer::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_Fox"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Ready_States()
{
	m_pStates[ENUM_CLASS(STATES::IDLE)] = new CPlayerState_Idle(this);
	m_pStates[ENUM_CLASS(STATES::MOVE)] = new CPlayerState_Move(this);
	m_pStates[ENUM_CLASS(STATES::DODGE)] = new CPlayerState_Dodge(this);
	m_pStates[ENUM_CLASS(STATES::SPRINT)] = new CPlayerState_Sprint(this);
	m_pStates[ENUM_CLASS(STATES::ATTACK1)] = new CPlayerState_Attack1(this);
	m_pStates[ENUM_CLASS(STATES::ATTACK2)] = new CPlayerState_Attack2(this);
	m_pStates[ENUM_CLASS(STATES::ATTACK3)] = new CPlayerState_Attack3(this);
	m_pStates[ENUM_CLASS(STATES::HIT)] = new CPlayerState_Hit(this);
	m_pStates[ENUM_CLASS(STATES::DIE)] = new CPlayerState_Die(this);

	for (_uint i = 0; i < ENUM_CLASS(STATES::ST_END); i++)
	{
		if (nullptr == m_pStates[i])
			return E_FAIL;
	}

	return S_OK;
}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pCurState);

	for (_uint i = 0; i < ENUM_CLASS(STATES::ST_END); i++)
		Safe_Release(m_pStates[i]);

}
