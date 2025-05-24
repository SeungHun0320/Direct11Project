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

	m_pModelCom->Set_Animation(0, true);

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CPlayer::Update(_float fTimeDelta)
{
	Key_Input(fTimeDelta);
	m_pModelCom->Play_Animation(fTimeDelta);

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

void CPlayer::Key_Input(_float fTimeDelta)
{
	//if (KEY_PRESSING(DIK_W))
	//{
	//	m_pModelCom->Change_Animation(GO_STRAIGHT, true, 0.03f, false);
	//}
	//if (KEY_DOWN(DIK_J))
	//{
	//	m_pModelCom->Change_Animation(SWORD_ATTACK1, true, 0.05f, true);
	//}
	//if (KEY_DOWN(DIK_SPACE))
	//{
	//	m_pModelCom->Change_Animation(FAIL_DODGE, false, 0.5f);
	//}
	//if(KEY_DOWN(DIK_Z))
	//	m_pModelCom->Change_Animation(IDLE, true, 0.5f);
	
	//if (KEY_DOWN(DIK_UP))
	//{
	//	++m_iSoonseo;

	//	if (32 <= m_iSoonseo)
	//		m_iSoonseo = 0;

	//	m_pModelCom->Change_Animation(m_iSoonseo, true, 0.5f, true, true);

	//}
	//	
	//if (KEY_DOWN(DIK_DOWN))
	//{
	//	--m_iSoonseo;

	//	if (0 >= m_iSoonseo)
	//		m_iSoonseo = 32;

	//	m_pModelCom->Change_Animation(m_iSoonseo, true, 0.5f, true, true);
	//}
		
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
	m_pStates[ENUM_CLASS(STATES::ATTACK)] = new CPlayerState_Attack(this);
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

	for (_uint i = 0; i < ENUM_CLASS(STATES::ST_END); i++)
		Safe_Release(m_pStates[i]);
}
