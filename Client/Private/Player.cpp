#include "Player.h"

#include "GameInstance.h"

#include "Body_Player.h"
#include "Weapon_Player.h"
#include "UI2D_PlayerHUDPart.h"

#include "PlayerState.h"
#include "Player_IAttackStrategy.h"

/* 쩦, */
#include "Monster.h"
#include "Monster_Bullet.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBaseActor{pDevice, pContext}
{
}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CBaseActor(Prototype)
{
}

void CPlayer::Set_Level(LEVEL eLevelID)
{
	m_eLevelID = eLevelID;

	m_pGameInstance->Add_Collider(dynamic_cast<CCollider*>(m_PartObjects[PART_BODY]->Get_Component(TEXT("Com_Collider"))), ENUM_CLASS(COLLIDER_GROUP::PAWN));
	m_pGameInstance->Add_Collider(dynamic_cast<CCollider*>(m_PartObjects[PART_WEAPON]->Get_Component(TEXT("Com_Collider_Stick"))), ENUM_CLASS(COLLIDER_GROUP::WEAPON));
	m_pGameInstance->Add_Collider(dynamic_cast<CCollider*>(m_PartObjects[PART_WEAPON]->Get_Component(TEXT("Com_Collider_Sword"))), ENUM_CLASS(COLLIDER_GROUP::WEAPON));
	m_pGameInstance->Add_Collider(dynamic_cast<CCollider*>(m_PartObjects[PART_WEAPON]->Get_Component(TEXT("Com_Collider_Dagger"))), ENUM_CLASS(COLLIDER_GROUP::WEAPON));


	if (nullptr != m_pNavigationCom)
	{
		Safe_Release(m_pNavigationCom);
	}

	CNavigation::DESC tDesc{};
	XMStoreFloat3(&tDesc.vInitWorldPos, m_pTransformCom->Get_State(STATE::POSITION));

	if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &tDesc)))
		return;
}

void CPlayer::Change_Level()
{
	LEVEL eNextLevelID{};

	switch (m_eLevelID)
	{
	case LEVEL::COURTYARD:
		eNextLevelID = LEVEL::ARENA;
		break;
	case LEVEL::ARENA:
		eNextLevelID = LEVEL::SHOP;
		break;
	case LEVEL::SHOP:
		eNextLevelID = LEVEL::COURTYARD;
		break;
	}

	m_pGameInstance->Change_Level(ENUM_CLASS(eNextLevelID));
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

	/* 체력 */
	m_fHp = 100.f;
	m_fHPRecorveryStat = 20.f;
	m_fMaxHp = m_fHp;
	/* 스태미나 */
	m_fStaminaRecoveryPerSec = 20.f;
	m_fMaxStamina = 100.f;
	m_fStamina = m_fMaxStamina;
	/* 공격력 */
	m_pAttackStrategy = new CPlayer_SwordAttack(3, WEAPON_TYPE::SWORD);
	m_fAttack = m_pAttackStrategy->Get_Attack();

	/* 그로기 */
	m_fStaggerGage = 100.f;
	m_fMaxStaggerGage = m_fStaggerGage;
	m_fStaggerRecoveryPerSec = 10.f;
	/* 타깃 */
	m_fFindDistance = 20.f;

	m_isShield = true;

	for (_uint i = 0; i < CPlayer::MESHES_END; i++)
	{
		if (i == CPlayer::MESH_SHILED && m_isShield)
			continue;

		m_PartObjects[PART_BODY]->Set_MeshVisible(i, true);
	}

	Set_AttackStrategy(new CPlayer_SwordAttack(3, WEAPON_TYPE::SWORD));
	m_fAttack = m_pAttackStrategy->Get_Attack();

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

	m_pTransformCom->Set_State(Engine::STATE::POSITION, m_pNavigationCom->SetUp_Height(m_pTransformCom->Get_State(Engine::STATE::POSITION)));

	return __super::Update(fTimeDelta);
}

void CPlayer::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CPlayer::Render()
{
#ifdef _DEBUG
		m_pNavigationCom->Render();
#endif
	return S_OK;
}

CGameObject* CPlayer::Find_Target(_float fFindDistance)
{
	CGameObject* pTarget = { nullptr };
	_float fMinDist = { FLT_MAX };

	list<CGameObject*>* pTargets = m_pGameInstance->Find_ObjectList(ENUM_CLASS(m_eLevelID), TEXT("Layer_Monster"));
	if (nullptr == pTargets)
		return nullptr;

	for (auto& pObj : *pTargets)
	{
		if (nullptr == pObj || pObj->Get_Dead())
			continue;

		_float fDist = XMVectorGetX(XMVector3Length(dynamic_cast<CTransform*>(pObj->Get_Component(TEXT("Com_Transform")))->Get_State(STATE::POSITION) -
			m_pTransformCom->Get_State(STATE::POSITION)));

		if (fDist < fFindDistance && fDist < fMinDist)
		{
			fMinDist = fDist;
			pTarget = pObj;
		}
	}

	return pTarget;
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

_bool CPlayer::Is_CurrentAnim(PART ePart, _uint iNextIndex)
{
	return m_PartObjects[ePart]->Is_CurrentAnim(iNextIndex);
}

void CPlayer::CheckChange_Anim(PART ePart, _uint iNextIndex, _bool isLoop, _float fBlendDuration, _bool isBlend)
{
	if(!Is_CurrentAnim(ePart, iNextIndex))
		Change_Animation(ePart, iNextIndex, true, 0.2f);
}

void CPlayer::Set_Active(PART ePart, _bool isActive)
{
	m_PartObjects[ePart]->Set_Active(isActive);
}

void CPlayer::Set_Active(WEAPON_TYPE eType, _bool isActive)
{
	m_pWeaponPart->Set_Active(eType, isActive);
}

CCollider* CPlayer::Get_Collider(PART ePart, _uint iColliderIndex)
{
	return m_PartObjects[ePart]->Get_Collider(iColliderIndex);
}

void CPlayer::Dodge(_fvector vDir, _float fTimeDelta, _float fSpeed)
{
	m_pTransformCom->Set_SpeedPerSec(fSpeed);
	m_pTransformCom->LookDir(vDir);
	m_pTransformCom->Go_Dir(vDir, fTimeDelta, m_pNavigationCom);
}

void CPlayer::Move(_fvector vDir, _float fTimeDelta, _float fSpeed)
{
	m_pTransformCom->Set_SpeedPerSec(fSpeed);
	m_pTransformCom->LookDirLerp(vDir, fTimeDelta, fSpeed * 1.5f);
	m_pTransformCom->Go_Dir(vDir, fTimeDelta, m_pNavigationCom);
}

void CPlayer::LockOn()
{
	if (!m_isTarget)
	{
		CGameObject* pTarget = Find_Target(m_fFindDistance);
		if (nullptr == pTarget || pTarget->Get_Dead())
		{
			m_isTarget = false;
			return;
		}

		m_pTargetTransform = dynamic_cast<CTransform*>(pTarget->Get_Component((TEXT("Com_Transform"))));
		if (nullptr == m_pTargetTransform)
		{
			m_isTarget = false;
			return;
		}

		Safe_AddRef(m_pTargetTransform);
		m_isTarget = true;
	}
}

void CPlayer::LockOff()
{
	if (m_isTarget)
	{
		if (nullptr != m_pTargetTransform)
			Safe_Release(m_pTargetTransform);
		m_isTarget = false;
	}
}

void CPlayer::LockOnMove(_fvector vDir, _float fTimeDelta, _float fSpeed)
{
	if (m_isTarget && nullptr != m_pTargetTransform)
	{
		m_pTransformCom->LookAtLerpEx(m_pTargetTransform->Get_State(STATE::POSITION), fTimeDelta, 10.f);
	}

	_vector vInputDir = Get_InputDirectionEx();

	_vector vLook = XMVector3Normalize(m_pTransformCom->Get_State(STATE::LOOK));
	_vector vRight = XMVector3Normalize(m_pTransformCom->Get_State(STATE::RIGHT));

	_float fForwardDot = XMVectorGetX(XMVector3Dot(vInputDir, vLook));
	_float fRightDot = XMVectorGetX(XMVector3Dot(vInputDir, vRight));

	if (fForwardDot > 0.7f)
		CheckChange_Anim(PART_BODY, GO_STRAIGHT, true, 0.3f);
	else if (fForwardDot < -0.7f)
		CheckChange_Anim(PART_BODY, GO_BACK, true, 0.3f);
	else if (fRightDot > 0.f)
		CheckChange_Anim(PART_BODY, GO_RIGHT, true, 0.3f);
	else
		CheckChange_Anim(PART_BODY, GO_LEFT, true, 0.3f);

	Go_Dir(vInputDir, fTimeDelta, fSpeed);

}

_vector CPlayer::Get_TargetState(STATE eState)
{
	if (!m_isTarget && nullptr == m_pTargetTransform)
		return XMVectorZero();

	return m_pTargetTransform->Get_State(eState);
}

void CPlayer::Set_Target(CTransform* pTargerTransform)
{
	m_pTargetTransform = pTargerTransform;
}

void CPlayer::Go_Dir(_fvector vDir, _float fTimeDelta, _float fSpeed)
{
	m_pTransformCom->Set_SpeedPerSec(fSpeed);
	m_pTransformCom->Go_Dir(vDir, fTimeDelta, m_pNavigationCom);
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

void CPlayer::LookTarget(_float fTimeDelta)
{
	if (m_isTarget && nullptr != m_pTargetTransform)
	{
		m_pTransformCom->LookAt(m_pTargetTransform->Get_State(STATE::POSITION));
	}
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

_vector CPlayer::Get_InputDirectionEx()
{
	_float fInputX{}, fInputZ{};

	/* 상 하 좌 우 */
	if (KeyPressing(DIK_W))
		fInputZ += 1.f;
	if (KeyPressing(DIK_S))
		fInputZ -= 1.f;
	if (KeyPressing(DIK_D))
		fInputX += 1.f;
	if (KeyPressing(DIK_A))
		fInputX -= 1.f;

	/* 카메라 기준 라이트, 룩 */
	_vector vCamRight = m_pGameInstance->Get_CameraState(ENUM_CLASS(m_eLevelID), TEXT("Camera_TPS"), STATE::RIGHT);
	_vector vCamLook = m_pGameInstance->Get_CameraState(ENUM_CLASS(m_eLevelID), TEXT("Camera_TPS"), STATE::LOOK);
	
	/* Y축 제거 */
	vCamRight = XMVector3Normalize(XMVectorSetY(vCamRight, 0.f));
	vCamLook = XMVector3Normalize(XMVectorSetY(vCamLook, 0.f));

	/* 입력 방향 계산 라이트는 좌우, 룩은 앞 뒤 */
	_vector vInputDir = vCamRight * fInputX + vCamLook * fInputZ;
	
	/* 입력받지 않았다면 원래의 룩 */
	if (XMVector3Equal(vInputDir, XMVectorZero()))
		vInputDir = m_pTransformCom->Get_State(STATE::LOOK);

	/* 정규화 */
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

_bool CPlayer::IsLockOn() const
{
	return KEY_PRESSING(DIK_LSHIFT);
}

void CPlayer::Set_AttackStrategy(CPlayer_IAttackStrategy* pStrategy)
{
	Safe_Release(m_pAttackStrategy);
	m_pAttackStrategy = pStrategy;
}

_float CPlayer::Compute_StaggerValue() const
{
	return m_pAttackStrategy->Get_StaggerValue();
}

void CPlayer::Key_Input(_float fTimeDelta)
{
	/* 테스트용으로 냅둔겨 나중에 싹 쳐내소 */
	if (KEY_DOWN(DIK_1))
	{
		Set_AttackStrategy(new CPlayer_StickAttack(2, WEAPON_TYPE::STICK));
		m_fAttack = m_pAttackStrategy->Get_Attack();
	}
	if (KEY_DOWN(DIK_2))
	{
		Set_AttackStrategy(new CPlayer_SwordAttack(3, WEAPON_TYPE::SWORD));
		m_fAttack = m_pAttackStrategy->Get_Attack();
	}
	if (KEY_DOWN(DIK_3))
	{
		Set_AttackStrategy(new CPlayer_DaggerAttack(1, WEAPON_TYPE::DAGGER));
		m_fAttack = m_pAttackStrategy->Get_Attack();
	}	

	if (KEY_DOWN(DIK_4))
	{
		m_isStagger = true;
		Change_States(STATES::HIT);
	}
	if (KEY_DOWN(DIK_5))
	{
		m_isStagger = false;
		Change_States(STATES::HIT);
	}
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

void CPlayer::On_Hit(_float fDamage, _float fStaggerValue, _float fInvicibleDuration)
{
	if (m_isInvincible || m_bDead)
		return;

	m_fHp -= fDamage;
	m_fStaggerGage -= fStaggerValue;
	m_isHit = true;
	m_pGameInstance->Shake_Camera(ENUM_CLASS(m_eLevelID), TEXT("Camera_TPS"), 0.2f, 0.2f);

	if (0 >= m_fHp)
	{
		m_fHp = 0.f;
		m_bDead = true;
		Change_States(STATES::DIE);
	}
	else
	{
		if (0 >= m_fStaggerGage)
		{
			m_isStagger = true;
			m_fInvicibleTime = 4.f;
			m_fStaggerGage = m_fMaxStaggerGage;
		}
		else
		{
			m_fInvicibleTime = fInvicibleDuration;
		}

		m_isInvincible = true;
		Change_States(STATES::HIT);
	}
}

void CPlayer::On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner)
{
	COLLIDER_ID eColliderID = static_cast<COLLIDER_ID>(OtherColliderID);
	_float fInvicibleDuration = Compute_InvincibleTime_ByCollider(static_cast<COLLIDER_ID>(OtherColliderID));

	if (CI_MONSTER(eColliderID))
	{
		CCollider* pCollider = Get_Collider(PART_BODY);
		if (nullptr == pCollider)
			return;

		m_pTransformCom->Apply_Sliding(pCollider->Get_SlidingVector(), m_pNavigationCom);
		m_isBlocked = true;
	}

	if (CI_MONSTER_ATTACK(eColliderID))
	{
		if (CMonster* pMonster = dynamic_cast<CMonster*>(pOwner))
		{
			On_Hit(pMonster->Get_AttackValue(), pMonster->Get_StaggerValue(), fInvicibleDuration);
		}
	}

	if (CI_MONSTER_BULLET(eColliderID))
	{
		if (CMonster_Bullet* pMonsterBullet = dynamic_cast<CMonster_Bullet*>(pOwner))
		{
			On_Hit(pMonsterBullet->Get_AttackValue(), pMonsterBullet->Get_StaggerValue(), fInvicibleDuration);
		}
	}

	switch (eColliderID)
	{
	case COLLIDER_ID::BUSH:
	{
		CCollider* pCollider = Get_Collider(PART_BODY);
		if (nullptr == pCollider)
			break;

		m_pTransformCom->Apply_Sliding(pCollider->Get_SlidingVector(), m_pNavigationCom);
		m_isBlocked = true;
	}
		break;
	default:
		break;
	}
}

_float CPlayer::Compute_InvincibleTime_ByCollider(COLLIDER_ID eColliderID)
{
	switch (eColliderID)
	{
	case COLLIDER_ID::BLOB_ATTACK:
		return 1.f;
	case COLLIDER_ID::WIZARD_ATTACK:
		return 1.f;
	case COLLIDER_ID::WIZARD_AOE:
		return 2.f;
	case COLLIDER_ID::SPIDERTANK_ATTACK:
		return 1.5f;
	case COLLIDER_ID::SPIDERTANK_BULLET:
		return 0.4f;
	case COLLIDER_ID::SPIDERTANK_BOMB:
		return 1.f;
	case COLLIDER_ID::SPIDERTANK_LAGER:
		return 1.f;
	default:
		return 0.6f;
	}
}

HRESULT CPlayer::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	CNavigation::DESC tDesc{};
	XMStoreFloat3(&tDesc.vInitWorldPos, m_pTransformCom->Get_State(STATE::POSITION));

	if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Ready_PartObjects()
{
	CBody_Player::DESC	BodyDesc{};

	BodyDesc.eLevelID = m_eLevelID;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	BodyDesc.pParentState = &m_eCurState;
	BodyDesc.strName = TEXT("Body_Player");
	BodyDesc.pOwner = this;

	if (FAILED(__super::Add_PartObject(PART_BODY, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Body_Player"), &BodyDesc)))
		return E_FAIL;

	CWeapon_Player::DESC	WeaponDesc{};

	WeaponDesc.eLevelID = m_eLevelID;
	WeaponDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	WeaponDesc.pParentState = &m_eCurState;
	WeaponDesc.strName = TEXT("Weapon_Player");
	WeaponDesc.pOwner = this;

	if (FAILED(__super::Add_PartObject(PART_WEAPON, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Weapon_Player"), &WeaponDesc)))
		return E_FAIL;

	m_pWeaponPart = dynamic_cast<CWeapon_Player*>(m_PartObjects[PART_WEAPON]);
	if (nullptr == m_pWeaponPart)
		return E_FAIL;

	Safe_AddRef(m_pWeaponPart);

	CUI2D_PlayerHUDPart::DESC UI2DDesc{};

	UI2DDesc.eLevelID = m_eLevelID;
	UI2DDesc.iNumPartObjects = CUI2D_PlayerHUDPart::PART_END;

	if (FAILED(__super::Add_PartObject(PART_UI2D, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D_PlayerHUDPart"), &UI2DDesc)))
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

	for (_uint i = 0; i < ENUM_CLASS(STATES::STATES_END); i++)
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

	Safe_Release(m_pWeaponPart);

	Safe_Release(m_pCurState);
	Safe_Release(m_pAttackStrategy);
	Safe_Release(m_pTargetTransform);

	for (_uint i = 0; i < ENUM_CLASS(STATES::STATES_END); i++)
		Safe_Release(m_pStates[i]);

}
