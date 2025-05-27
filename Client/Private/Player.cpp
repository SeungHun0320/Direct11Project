#include "Player.h"

#include "GameInstance.h"

#include "Body_Player.h"
#include "PlayerState.h"
#include "Player_IAttackStrategy.h"

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

	m_IsShield = true;

	for (_uint i = 0; i < CPlayer::MESHES::MESHES_END; i++)
	{
		if (i == CPlayer::MESHES::MESH_SHILED && m_IsShield)
			continue;

		m_PartObjects[PART_BODY]->Set_MeshVisible(i, true);
	}

	m_pAttackStrategy = new CPlayer_SwordAttack(3, WEAPON_TYPE::SWORD);

	Change_States(STATES::WAKE_UP);
	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CPlayer::Update(_float fTimeDelta)
{
	//if (m_bDead)
	//	return LIFE::DEAD;

	Key_Input(fTimeDelta);
	Stamina_Recovery(fTimeDelta);

	if (m_eCurState != m_ePreState)
	{
		m_pCurState->Enter(fTimeDelta);
		m_ePreState = m_eCurState;
	}

	m_pCurState->Execute(fTimeDelta);

	return __super::Update(fTimeDelta);
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CPlayer::Render()
{
	return S_OK;
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

_bool CPlayer::Play_Animation(PART ePart, _float fTimeDelta)
{
	return m_PartObjects[ePart]->Play_Animation(fTimeDelta);
}

void CPlayer::Set_TrackPosition(PART ePart, _float fTrackPosition)
{
	m_PartObjects[ePart]->Set_TrackPosition(fTrackPosition);
}

void CPlayer::Change_Animation(PART ePart, _uint iNextIndex, _bool isLoop, _float fBlendDuration, _bool isBlend)
{
	m_PartObjects[ePart]->Change_Animation(iNextIndex, isLoop, fBlendDuration, isBlend);
}

void CPlayer::Set_MeshVisible(PART ePart, _uint iIndex, _bool IsVisible)
{
	m_PartObjects[ePart]->Set_MeshVisible(iIndex, IsVisible);
}

void CPlayer::Dodge(_fvector vDir, _float fTimeDelta, _float fSpeed)
{
	m_pTransformCom->Set_SpeedPerSec(fSpeed);
	m_pTransformCom->LookDir(vDir);
	m_pTransformCom->Go_Dir(vDir, fTimeDelta);
}

void CPlayer::Move(_fvector vDir, _float fTimeDelta, _float fSpeed)
{
	m_pTransformCom->Set_SpeedPerSec(fSpeed);
	m_pTransformCom->LookDirLerp(vDir, fTimeDelta, fSpeed * 1.5f);
	m_pTransformCom->Go_Dir(vDir, fTimeDelta);
}

void CPlayer::Stagger(_fvector vDir, _float fTimeDelta, _float fSpeed)
{
	m_pTransformCom->Set_SpeedPerSec(fSpeed);
	m_pTransformCom->Go_Dir(vDir, fTimeDelta);
}

void CPlayer::Go_Up(_float fTimeDelta, _float fSpeed)
{
	m_pTransformCom->Set_SpeedPerSec(fSpeed);
	m_pTransformCom->Go_Up(fTimeDelta);
}

void CPlayer::Go_Down(_float fTimeDelta, _float fSpeed)
{
	m_pTransformCom->Set_SpeedPerSec(fSpeed);
	m_pTransformCom->Go_Down(fTimeDelta);
}

_vector CPlayer::Get_InputDirection()
{
	_vector vInputDir{};

	if (KeyPressing(DIK_W))
		vInputDir += DIR_FORWARDRIGHT;

	if (KeyPressing(DIK_A))
		vInputDir += DIR_FORWARDLEFT;

	if (KeyPressing(DIK_S))
		vInputDir += DIR_BACKWARDLEFT;

	if (KeyPressing(DIK_D))
		vInputDir += DIR_BACKWARDRIGHT;

	if (XMVector3Equal(vInputDir, XMVectorZero()))
		vInputDir = m_pTransformCom->Get_State(STATE::LOOK);

	return XMVector3Normalize(vInputDir);
}


_vector CPlayer::Get_State(STATE eState)
{
	return m_pTransformCom->Get_State(eState);
}

_bool CPlayer::KeyDown(_ubyte eKeyID)
{
	return KEY_DOWN(eKeyID);
}

_bool CPlayer::KeyPressing(_ubyte eKeyID)
{
	return KEY_PRESSING(eKeyID);
}

_bool CPlayer::KeyUp(_ubyte eKeyID)
{
	return KEY_UP(eKeyID);
}

_bool CPlayer::IsAnyMoveKeyPressed() const
{
	return KEY_PRESSING(DIK_W) || KEY_PRESSING(DIK_A) ||
		KEY_PRESSING(DIK_S) || KEY_PRESSING(DIK_D) ||
		KEY_PRESSING(DIK_SPACE);
}

_bool CPlayer::IsMoveKeyPressed()
{
	return KEY_PRESSING(DIK_W) || KEY_PRESSING(DIK_A) ||
		KEY_PRESSING(DIK_S) || KEY_PRESSING(DIK_D);
}

void CPlayer::Set_AttackStrategy(CPlayer_IAttackStrategy* pStrategy)
{
	Safe_Release(m_pAttackStrategy);
	m_pAttackStrategy = pStrategy;
}

void CPlayer::Key_Input(_float fTimeDelta)
{
	if (KEY_DOWN(DIK_1))
		Set_AttackStrategy(new CPlayer_StickAttack(2, WEAPON_TYPE::STICK));
	if (KEY_DOWN(DIK_2))
		Set_AttackStrategy(new CPlayer_SwordAttack(3, WEAPON_TYPE::SWORD));
	if (KEY_DOWN(DIK_3))
		Set_AttackStrategy(new CPlayer_DaggerAttack(1, WEAPON_TYPE::DAGGER));
	if (KEY_DOWN(DIK_4))
		m_eHitType = HIT_TYPE::NORMAL;
	if (KEY_DOWN(DIK_5))
		m_eHitType = HIT_TYPE::STAGGER;
	if (KEY_DOWN(DIK_6))
		m_IsHit = true;
	if (KEY_DOWN(DIK_7))
		m_bDead = true;
	if (KEY_DOWN(DIK_LCONTROL))
		Change_States(STATES::LADDER);
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

	return S_OK;
}

HRESULT CPlayer::Ready_PartObjects()
{
	CBody_Player::DESC	BodyDesc{};

	BodyDesc.eLevelID = m_eLevelID;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();

	if (FAILED(__super::Add_PartObject(PART_BODY, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Body_Player"), &BodyDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Ready_States()
{
	m_pStates[ENUM_CLASS(STATES::IDLE)]       = new CPlayerState_Idle(this);
	m_pStates[ENUM_CLASS(STATES::MOVE)]       = new CPlayerState_Move(this);
	m_pStates[ENUM_CLASS(STATES::DODGE)]      = new CPlayerState_Dodge(this);
	m_pStates[ENUM_CLASS(STATES::SPRINT)]     = new CPlayerState_Sprint(this);
	m_pStates[ENUM_CLASS(STATES::ATTACK1)]    = new CPlayerState_Attack1(this);
	m_pStates[ENUM_CLASS(STATES::ATTACK2)]    = new CPlayerState_Attack2(this);
	m_pStates[ENUM_CLASS(STATES::ATTACK3)]    = new CPlayerState_Attack3(this);
	m_pStates[ENUM_CLASS(STATES::HIT)]        = new CPlayerState_Hit(this);
	m_pStates[ENUM_CLASS(STATES::GET_UP)]     = new CPlayerState_GetUp(this);
	m_pStates[ENUM_CLASS(STATES::DIE)]		  = new CPlayerState_Die(this);
	m_pStates[ENUM_CLASS(STATES::PARRY)]      = new CPlayerState_Parry(this);
	m_pStates[ENUM_CLASS(STATES::USE_POTION)] = new CPlayerState_UsePotion(this);
	m_pStates[ENUM_CLASS(STATES::DANCE)]	  = new CPlayerState_Dance(this);
	m_pStates[ENUM_CLASS(STATES::WAKE_UP)]    = new CPlayerState_WakeUp(this);
	m_pStates[ENUM_CLASS(STATES::WIND_UP)]	  = new CPlayerState_WindUp(this);
	m_pStates[ENUM_CLASS(STATES::TOSS)]		  = new CPlayerState_Toss(this);
	m_pStates[ENUM_CLASS(STATES::EAT)]		  = new CPlayerState_Eat(this);
	m_pStates[ENUM_CLASS(STATES::COIN_FLIP)]  = new CPlayerState_CoinFlip(this);
	m_pStates[ENUM_CLASS(STATES::OPEN_CHEST)] = new CPlayerState_OpenChest(this);
	m_pStates[ENUM_CLASS(STATES::ON_SWITCH)]  = new CPlayerState_OnSwitch(this);
	m_pStates[ENUM_CLASS(STATES::KNEEL)]	  = new CPlayerState_Kneel(this);
	m_pStates[ENUM_CLASS(STATES::LADDER)]	  = new CPlayerState_Ladder(this);
	m_pStates[ENUM_CLASS(STATES::ON_LADDER)]  = new CPlayerState_OnLadder(this);
	m_pStates[ENUM_CLASS(STATES::OFF_LADDER)] = new CPlayerState_OffLadder(this);

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
	Safe_Release(m_pAttackStrategy);

	for (_uint i = 0; i < ENUM_CLASS(STATES::ST_END); i++)
		Safe_Release(m_pStates[i]);

}
