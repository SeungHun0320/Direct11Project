#include "Player.h"

#include "GameInstance.h"

/* 몸이랑 무기 */
#include "Body_Player.h"
#include "Weapon_Player.h"
/* UI */
#include "UI2D_PlayerHPBar.h"
#include "UI2D_PlayerSPBar.h"
#include "UI2D_PlayerMPBar.h"
#include "UI2D_PlayerPotion.h"
#include "UI2D_PlayerItemSlots.h"
#include "UI2D_Inventory.h"
#include "UI3D_PlayerSweat.h"
/* 인벤토리 */
#include "Inventory.h"
/* 상태, 전략 */
#include "PlayerState.h"
#include "Player_IAttackStrategy.h"
/* 총알 */
#include "Bullet_FireCracker.h"
/* 이펙트(파티클) */
#include "Particle_Part.h"
#include "Particle_Mesh_Dash.h"

/* 쩦, */
#include "Monster.h"
#include "Bullet_Monster.h"

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

	Set_SavePosition();

	if (nullptr != m_pNavigationCom)
		Safe_Release(m_pNavigationCom);

	CNavigation::DESC tDesc{};
	XMStoreFloat3(&tDesc.vInitWorldPos, m_pTransformCom->Get_State(STATE::POSITION));

	if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &tDesc)))
		return;
}

void CPlayer::Set_SavePosition()
{
	switch (m_eLevelID)
	{
	case LEVEL::COURTYARD:
		XMStoreFloat3(&m_vSavePosition, XMVectorSet(-40.f, -2.f, -123.f, 1.f));
		break;
	case LEVEL::ARENA:
		XMStoreFloat3(&m_vSavePosition, XMVectorSet(-0.f, -4.f, 35.f, 1.f));
		break;
	case LEVEL::SHOP:
		XMStoreFloat3(&m_vSavePosition, XMVectorSet(6.f, 4.f, -41.f, 1.f));
		break;
	default:
		break;
	}
}

void CPlayer::Clear_Target()
{
	if (nullptr != m_pTargetTransform)
		Safe_Release(m_pTargetTransform);

	if (nullptr != m_pTarget)
		Safe_Release(m_pTarget);
}

void CPlayer::Change_Level()
{
	Clear_Target();

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
	/* 체력 */
	m_fHp = 100.f;
	m_fHPRecorveryStat = 20.f;
	m_fMaxHp = m_fHp;

	/* 스태미나 */
	m_fStaminaRecoveryPerSec = 20.f;
	m_fMaxStamina = 100.f;
	m_fStamina = m_fMaxStamina;
	/* 마나 */
	m_fMaxMana = 100.f;
	m_fManaRecoveryStat = 30.f;
	m_fMana = m_fMaxMana;
	/* 그로기 */
	m_fStaggerGage = 100.f;
	m_fMaxStaggerGage = m_fStaggerGage;
	m_fStaggerRecoveryPerSec = 10.f;
	/* 타깃 */
	m_fFindDistance = 20.f;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	for (_uint i = 0; i < CPlayer::MESHES_END; i++)
	{
		if (i == CPlayer::MESH_SHILED && Has_Shield())
			continue;

		m_PartObjects[PART_BODY]->Set_MeshVisible(i, true);
	}

	Set_SavePosition();
	Subscribe_Events();

	Change_States(STATES::WAKE_UP);
	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CPlayer::Update(_float fTimeDelta)
{
	if (m_bDead)
		Respawn();

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

	m_eCurInteractID = COLLIDER_ID::CI_END;

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
	if(COLLIDER_ID::BUSH != m_eCurInteractID)
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
		
		m_pTarget = dynamic_cast<CMonster*>(pTarget);
		Safe_AddRef(m_pTarget);

		m_pTarget->Set_IsLockOnTarget();

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

		m_pTarget->Set_IsLockOnTarget(false);

		if (nullptr != m_pTarget)
			Safe_Release(m_pTarget);

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

CInventory* CPlayer::Get_Inventory() const
{
	return m_pInventory;
}

void CPlayer::Use_QuickSlot(_uint eSlot)
{
	CInventory::QSLOT_TYPE eQSlotType = static_cast<CInventory::QSLOT_TYPE>(eSlot);
	CInventory::QUICK_SLOT QuickSlot = m_pInventory->Get_QuickSlot(eQSlotType);

	if (!QuickSlot.bHasItem)
		return;

	if (m_pInventory->Is_UseItem(QuickSlot.eType))
	{
		if (!m_pInventory->Use_QuickSlot(eQSlotType))
			return;
	}

	switch (QuickSlot.eType)
	{
	case ITEM_TYPE::STICK:
		Equip_Weapon(new CPlayer_SwordAttack(1, WEAPON_TYPE::STICK));
		break;
	case ITEM_TYPE::SWORD:
		Equip_Weapon(new CPlayer_SwordAttack(3, WEAPON_TYPE::SWORD));
		break;
	case ITEM_TYPE::DAGGER:
		Equip_Weapon(new CPlayer_DaggerAttack(1, WEAPON_TYPE::DAGGER));
		break;
	case ITEM_TYPE::BERRY:
		m_byEatType = 1; /* 베리 먹어요 */
		Change_States(STATES::EAT);
		break;
	case ITEM_TYPE::BLUEBERRY:
		m_byEatType = 2; /* 블루베리 먹어요 */
		Change_States(STATES::EAT);
		break;
	case ITEM_TYPE::COIN_QUESTION:
		Use_Coin_Question();
		break;
	case ITEM_TYPE::FIRE_CRACKER:
		Change_States(STATES::WIND_UP);
		break;
	default:
		break;
	}
}

void CPlayer::Equip_Weapon(CPlayer_IAttackStrategy* pStrategy)
{
	Set_AttackStrategy(pStrategy);
	m_fAttack = m_pAttackStrategy->Get_Attack();
	Start_Attack();
}

void CPlayer::Use_Berrys(_byte byEatType)
{
	switch (byEatType)
	{
	case 1:
		m_fHp = clamp(m_fHp + (m_fHPRecorveryStat * 0.5f), 0.f, m_fMaxHp);
		break;
	case 2:
		m_fMana = clamp(m_fMana + m_fManaRecoveryStat, 0.f, m_fMaxMana);
		break;
	}
}

void CPlayer::Use_Coin_Question()
{
	Change_States(STATES::COIN_FLIP);
}

void CPlayer::Use_FireCracker()
{
	CBullet_FireCracker::DESC tDesc{};
	_float3 vPos{};

	tDesc.eLevelID = m_eLevelID;
	tDesc.fSpeedPerSec = 7.5f;
	tDesc.strName = TEXT("FireCracker");
	tDesc.strPrototypeModelTag = TEXT("Prototype_Component_Model_FireCracker");

	XMStoreFloat3(&tDesc.vDir, XMVector3Normalize(m_pTransformCom->Get_State(STATE::LOOK)));
	XMStoreFloat3(&vPos, m_pTransformCom->Get_State(STATE::POSITION));

	tDesc.WorldMatrix = XMMatrixTranslation(vPos.x, vPos.y + 1.f, vPos.z);

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Bullet_") + tDesc.strName,
		ENUM_CLASS(tDesc.eLevelID), TEXT("Layer_Bullet"), &tDesc)))
		return;

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

void CPlayer::Change_States_ByInteract()
{
	switch (m_eCurInteractID)
	{
	case COLLIDER_ID::CHEST:
		Change_States(STATES::OPEN_CHEST);
		break;
	case COLLIDER_ID::LADDER:
		Change_States(STATES::LADDER);
		break;
	case COLLIDER_ID::SWITCH:
		Change_States(STATES::ON_SWITCH);
		break;
	case COLLIDER_ID::CHECKPOINT:
		Active_CheckPoint();
		Change_States(STATES::KNEEL);
		break;
	default:
		Change_States(STATES::DODGE);
		break;
	}

	m_eCurInteractID = COLLIDER_ID::CI_END;
}

void CPlayer::Use_Potion()
{
	if (m_fHp == m_fMaxHp)
		return;

	if (KEY_DOWN(DIK_P))
	{
		if (m_pInventory->Use_Potion())
		{
			Change_States(CPlayer::STATES::USE_POTION);
		}
	}
}

void CPlayer::Heal()
{
	m_fHp = clamp(m_fHp + m_fHPRecorveryStat, 0.f, m_fMaxHp);
}

void CPlayer::Start_Attack()
{
	if (m_pAttackStrategy->Get_WeaponType() == WEAPON_TYPE::DAGGER && m_fMana <= 30.f)
		return;

	Change_States(STATES::ATTACK1);
}

void CPlayer::SetUp_AttackMeshVisible(WEAPON_TYPE eWeaponType)
{
	for (_uint i = 0; i < MESHES_END; i++)
	{
		if (i == MESH_SHILED && Has_Shield())
			continue;
		Set_MeshVisible(PART_BODY, i, true);
	}

	switch (eWeaponType)
	{
	case WEAPON_TYPE::SWORD:
		Set_MeshVisible(PART_BODY, MESH_SWORD, false);
		break;
	case WEAPON_TYPE::STICK:
		Set_MeshVisible(PART_BODY, MESH_STICK, false);
		break;
	case WEAPON_TYPE::DAGGER:
		Set_MeshVisible(PART_BODY, MESH_DAGGER, false);
		break;
	}
}

_bool CPlayer::DodgeComboAttack(ITEM_TYPE eItemType)
{
	if (nullptr == m_pAttackStrategy)
		return false;

	WEAPON_TYPE eType{ WEAPON_TYPE::WT_END };

	switch (eItemType)
	{
	case ITEM_TYPE::STICK:
		eType =  WEAPON_TYPE::STICK;
		break;
	case ITEM_TYPE::SWORD:
		eType = WEAPON_TYPE::SWORD;
		break;
	case ITEM_TYPE::DAGGER:
		eType = WEAPON_TYPE::DAGGER;
		break;
	default:
		eType = WEAPON_TYPE::WT_END;
		break;
	}

	return eType == m_pAttackStrategy->Get_WeaponType();
}

_bool CPlayer::CheckDodgeComboWeapon()
{
	ITEM_TYPE eItemType{ ITEM_TYPE::IT_END };

	if (KeyPressing(DIK_J))
		eItemType = m_pInventory->IsWeaponInQuickSlot(CInventory::QSLOT_J);
	else if (KeyPressing(DIK_K))
		eItemType = m_pInventory->IsWeaponInQuickSlot(CInventory::QSLOT_K);
	else if(KeyPressing(DIK_L))
		eItemType = m_pInventory->IsWeaponInQuickSlot(CInventory::QSLOT_L);

	if (ITEM_TYPE::IT_END == eItemType)
		return false;

	return DodgeComboAttack(eItemType);//여기서 타입을 받아서 바꾸고, 똑같은 녀석일때만 리턴 하면 될 듯
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
	if (m_isOnInven)
		return false;

	return KEY_DOWN(eKeyID);
}

_bool CPlayer::KeyPressing(_ubyte eKeyID)
{
	if (m_isOnInven)
		return false;

	return KEY_PRESSING(eKeyID);
}

_bool CPlayer::KeyUp(_ubyte eKeyID)
{
	if (m_isOnInven)
		return false;

	return KEY_UP(eKeyID);
}

_bool CPlayer::IsAnyMoveKeyPressed() const
{
	if (m_isOnInven)
		return false;

	return KEY_PRESSING(DIK_W) || KEY_PRESSING(DIK_A) ||
		KEY_PRESSING(DIK_S) || KEY_PRESSING(DIK_D) ||
		KEY_PRESSING(DIK_SPACE);
}

_bool CPlayer::IsMoveKeyPressed()
{
	if (m_isOnInven)
		return false;

	return KEY_PRESSING(DIK_W) || KEY_PRESSING(DIK_A) ||
		KEY_PRESSING(DIK_S) || KEY_PRESSING(DIK_D);
}

void CPlayer::Subscribe_Events()
{
	Delegate<> InvenPotionDele;
	InvenPotionDele.Bind<CPlayer, &CPlayer::Equip_Shield>(this);
	m_pGameInstance->Subscribe_Event(TEXT("Equip_Shield"), InvenPotionDele);
}

void CPlayer::Equip_Shield()
{
	m_PartObjects[PART_BODY]->Set_MeshVisible(MESH_SHILED, false);
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

void CPlayer::Use_Stamina(_float fStamina)
{
	m_fStamina -= fStamina;
	m_isUseStamina = true;
	m_fStaminaDelayTimeAcc = 0.f;

	if (0 >= m_fStamina)
	{
		m_fStamina = 0.f;
		m_fStaminaDelayTime = 3.f;
		m_isNoStamina = true;
	}
	else
	{
		m_fStaminaDelayTime = 1.5f;
	}
}

void CPlayer::Stamina_Recovery(_float fTimeDelta)
{
	m_fStaminaDelayTimeAcc += fTimeDelta;

	if (m_fStaminaDelayTime <= m_fStaminaDelayTimeAcc)
	{
		m_fStamina += m_fStaminaRecoveryPerSec * fTimeDelta;
		m_fStamina = min(m_fStamina, m_fMaxStamina);
		m_isUseStamina = false;
	}

	if (60.f <= m_fStamina)
		m_isNoStamina = false;
}

void CPlayer::Use_Mana(_float fMana)
{
	m_fMana -= fMana;
	m_fMana = max(m_fMana, 0);
}

_bool CPlayer::Has_Shield() const
{
	return m_pInventory->Get_isShield();
}

void CPlayer::Active_CheckPoint()
{
	XMStoreFloat3(&m_vSavePosition, m_pTransformCom->Get_State(STATE::POSITION));

	m_fHp = m_fMaxHp;
	m_fStamina = m_fMaxStamina;
	m_fMana = m_fMaxMana;
	m_fStaggerGage = m_fMaxStaggerGage;

	m_pInventory->Refill_Potion();

	m_pGameInstance->Respawn_Objects();
}

void CPlayer::Respawn()
{
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(XMLoadFloat3(&m_vSavePosition), 1.f));

	m_fHp = m_fMaxHp;
	m_fStamina = m_fMaxStamina;
	m_fMana = m_fMaxMana;
	m_fStaggerGage = m_fMaxStaggerGage;

	m_pInventory->Refill_Potion();

	m_pNavigationCom->Update_CellIndex(XMVectorSetW(XMLoadFloat3(&m_vSavePosition), 1.f));

	m_bDead = false;
	m_isHit = false;

	m_pGameInstance->Respawn_Objects();

	m_pGameInstance->Set_CameraMode(ENUM_CLASS(m_eLevelID), TEXT("Camera_TPS"), ENUM_CLASS(CAM_MODE::TPS));
}

void CPlayer::Key_Input(_float fTimeDelta)
{
	if (KEY_DOWN(DIK_TAB))
	{
		m_isOnInven = !m_isOnInven;
	}

	/* 테스트용으로 냅둔겨 나중에 싹 쳐내소 */
	if (KEY_DOWN(DIK_1))
	{
		m_pInventory->Acquire_Item(ITEM_TYPE::STICK);
	}
	if (KEY_DOWN(DIK_2))
	{
		m_pInventory->Acquire_Item(ITEM_TYPE::SWORD);
	}
	if (KEY_DOWN(DIK_3))
	{
		m_pInventory->Acquire_Item(ITEM_TYPE::DAGGER);
	}
	if (KEY_DOWN(DIK_4))
	{
		m_pInventory->Acquire_Item(ITEM_TYPE::FIRE_CRACKER);
	}
	if (KEY_DOWN(DIK_5))
	{
		m_pInventory->Acquire_Item(ITEM_TYPE::COIN_QUESTION);
	}
	if (KEY_DOWN(DIK_6))
	{
		m_pInventory->Acquire_Item(ITEM_TYPE::BERRY);
	}
	if (KEY_DOWN(DIK_7))
	{
		m_pInventory->Acquire_Item(ITEM_TYPE::BLUEBERRY);
	}
	if (KEY_DOWN(DIK_8))
	{
		m_pInventory->Add_Potion();
	}
	if (KEY_DOWN(DIK_9))
	{
		Change_States(STATES::DIE);
	}
	if (KEY_DOWN(DIK_F1))
	{
		m_fHp -= 25.f;
		m_fHp = max(m_fHp, 0);
	}
	if (KEY_DOWN(DIK_F2))
		Use_Mana(25.f);

	//if (KEY_DOWN(DIK_LCONTROL))
	//	Change_States(STATES::LADDER);
}

void CPlayer::On_Hit(_float fDamage, _float fStaggerValue, _float fInvicibleDuration)
{
	if (m_isInvincible || STATES::DIE == m_eCurState )
		return;

	m_fHp -= fDamage;
	m_fStaggerGage -= fStaggerValue;
	m_isHit = true;
	m_pGameInstance->Shake_Camera(ENUM_CLASS(m_eLevelID), TEXT("Camera_TPS"), 0.2f, 0.2f);

	if (0 >= m_fHp)
	{
		m_fHp = 0.f;
		Change_States(STATES::DIE);
	}
	else
	{
		if (0 >= m_fStaggerGage)
		{
			m_isStagger = true;
			m_fInvicibleTime = 4.3f;
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
	_float fInvicibleDuration = Compute_InvincibleTime(static_cast<COLLIDER_ID>(OtherColliderID));

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
		if (CBullet* Bullet = dynamic_cast<CBullet*>(pOwner))
		{
			On_Hit(Bullet->Get_AttackValue(), Bullet->Get_StaggerValue(), fInvicibleDuration);
		}
	}

	if (CI_ENVIRONMENT(eColliderID))
		m_eCurInteractID = eColliderID;

	switch (eColliderID)
	{
	case COLLIDER_ID::BUSH:
	case COLLIDER_ID::CHEST:
	case COLLIDER_ID::CHECKPOINT:
	{
		CCollider* pCollider = Get_Collider(PART_BODY);
		if (nullptr == pCollider)
			break;

		m_pTransformCom->Apply_Sliding(pCollider->Get_SlidingVector(), m_pNavigationCom);
		m_isBlocked = true;
	}
	break;
	case COLLIDER_ID::COIN:
		m_pInventory->Add_Coin(10);
		break;
	case COLLIDER_ID::BULLET_EXPLOSION:
	{
		if (CBullet* Bullet = dynamic_cast<CBullet*>(pOwner))
		{
			On_Hit(Bullet->Get_AttackValue(), Bullet->Get_StaggerValue(), fInvicibleDuration);
		}
	}
	default:
		break;
	}
}

_float CPlayer::Compute_InvincibleTime(COLLIDER_ID eColliderID)
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
	case COLLIDER_ID::BULLET_EXPLOSION:
		return 1.f;
	default:
		return 0.6f;
	}
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

	LEVEL eLevelID = LEVEL::STATIC;

	CUI2D_PlayerHPBar::DESC HPBarDesc{};

	HPBarDesc.pParentLevelID = &eLevelID;
	HPBarDesc.iNumPartObjects = CUI2D_PlayerHPBar::PART_END;
	HPBarDesc.pParentHP = &m_fHp;
	HPBarDesc.pParentMaxHP = &m_fMaxHp;
	HPBarDesc.pParentHPRecorveryStat = &m_fHPRecorveryStat;

	if (FAILED(__super::Add_PartObject(PART_HP, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D_PlayerHPBar"), &HPBarDesc)))
		return E_FAIL;

	CUI2D_PlayerSPBar::DESC StaminaBarDesc{};

	StaminaBarDesc.pParentLevelID = &eLevelID;
	StaminaBarDesc.iNumPartObjects = CUI2D_PlayerSPBar::PART_END;
	StaminaBarDesc.pParentStamina = &m_fStamina;
	StaminaBarDesc.pParentMaxStamina = &m_fMaxStamina;
	StaminaBarDesc.pParentStaminaRecorveryStat = &m_fStaminaRecoveryPerSec;

	if (FAILED(__super::Add_PartObject(PART_STAMINA, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D_PlayerStaminaBar"), &StaminaBarDesc)))
		return E_FAIL;

	CUI2D_PlayerMPBar::DESC ManaBarDesc{};

	ManaBarDesc.pParentLevelID = &eLevelID;
	ManaBarDesc.iNumPartObjects = CUI2D_PlayerMPBar::PART_END;
	ManaBarDesc.pParentMana = &m_fMana;
	ManaBarDesc.pParentMaxMana = &m_fMaxMana;
	ManaBarDesc.pParentManaRecorveryStat = &m_fManaRecoveryStat;

	if (FAILED(__super::Add_PartObject(PART_MP, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D_PlayerManaBar"), &ManaBarDesc)))
		return E_FAIL;

	/* 인벤이 무조건 인벤관련UI들보다 먼저 만들어지게,,*/

	CInventory::DESC InvenDesc{};

	InvenDesc.pParentLevelID = &eLevelID;
	InvenDesc.pParentIsOnInven = &m_isOnInven;

	if (FAILED(__super::Add_PartObject(PART_INVEN, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Inventory"), &InvenDesc)))
		return E_FAIL;

	m_pInventory = dynamic_cast<CInventory*>(m_PartObjects[PART_INVEN]);
	if (nullptr == m_pInventory)
		return E_FAIL;

	Safe_AddRef(m_pInventory);

	CUI2D_PlayerPotion::DESC PotionDesc{};

	PotionDesc.pParentLevelID = &eLevelID;
	PotionDesc.iNumPartObjects = CUI2D_PlayerPotion::PART_END;
	PotionDesc.pInventory = m_pInventory;

	if (FAILED(__super::Add_PartObject(PART_POTION, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D_PlayerPotion"), &PotionDesc)))
		return E_FAIL;

	CUI2D_PlayerItemSlots::DESC ItemSlotsDesc{};

	ItemSlotsDesc.pParentLevelID = &eLevelID;
	ItemSlotsDesc.iNumPartObjects = CUI2D_PlayerItemSlots::PART_END;
	ItemSlotsDesc.pInventory = m_pInventory;

	if (FAILED(__super::Add_PartObject(PART_ITEMSLOTS, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D_PlayerItemSlots"), &ItemSlotsDesc)))
		return E_FAIL;

	CUI2D_Inventory::DESC UIInvenDesc{};

	UIInvenDesc.pParentLevelID = &eLevelID;
	UIInvenDesc.iNumPartObjects = CUI2D_Inventory::PART_END;
	UIInvenDesc.pParentIsOnInven = &m_isOnInven;
	UIInvenDesc.pInventory = m_pInventory;

	if (FAILED(__super::Add_PartObject(PART_UIINVEN, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D_Inventory"), &UIInvenDesc)))
		return E_FAIL;

	CUI3D_PlayerSweat::DESC SweatDesc{};

	SweatDesc.pParentLevelID = &eLevelID;
	SweatDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	SweatDesc.fSizeX = 128.5f;
	SweatDesc.fSizeY = 128.5f;
	SweatDesc.fOffset = 2.f;
	SweatDesc.fMaxFrame = 6.f;
	SweatDesc.iColumns = 6;
	SweatDesc.fSpeed = 6.f;
	SweatDesc.pParentisNoStamina = &m_isNoStamina;
	SweatDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_Sweat");
	SweatDesc.eUIPass = CUI::PASS_SPRITE;

	if (FAILED(__super::Add_PartObject(PART_SWEAT, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI3D_PlayerSweat"), &SweatDesc)))
		return E_FAIL;

	CParticle_Mesh_Dash::DESC DashDesc{};

	DashDesc.pParentLevelID = &eLevelID;
	DashDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	DashDesc.strParticleModelTag = TEXT("Prototype_Component_Model_Particle_Instance_Dash");
	DashDesc.pParentisNoStamina = &m_isNoStamina;
	DashDesc.pParentisUseStamina = &m_isUseStamina;

	if (FAILED(__super::Add_PartObject(PART_EFFECT, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Dash"), &DashDesc)))
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
	Safe_Release(m_pInventory);

	Safe_Release(m_pCurState);
	Safe_Release(m_pAttackStrategy);

	Safe_Release(m_pTargetTransform);
	Safe_Release(m_pTarget);

	for (_uint i = 0; i < ENUM_CLASS(STATES::STATES_END); i++)
		Safe_Release(m_pStates[i]);
}
