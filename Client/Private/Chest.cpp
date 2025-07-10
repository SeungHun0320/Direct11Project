#include "Chest.h"
#include "GameInstance.h"

#include "Body_Chest.h"

#include "UI3D_Interaction.h"
#include "UI2D_Reward.h"

#include "Item_Coin.h"

CChest::CChest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEnvironment_Object{ pDevice, pContext }
{
}

CChest::CChest(const CChest& Prototype)
	: CEnvironment_Object(Prototype)
{
}

HRESULT CChest::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CChest::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_eType = pDesc->eType;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pBodyIsOpened = dynamic_cast<CBody_Chest*>(m_PartObjects[PART_BODY])->Get_isOpened();

	return S_OK;
}

void CChest::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CChest::Update(_float fTimeDelta)
{
	if (m_bDead)
		return LIFE::DEAD;

	if ((*m_pBodyIsOpened) && !m_IsOpend)
	{
		m_fTimeAcc += fTimeDelta;
		if(0.8f <= m_fTimeAcc)
			On_Opened();
	}

	return __super::Update(fTimeDelta);
}

void CChest::Late_Update(_float fTimeDelta)
{
	if (!m_pGameInstance->isIn_WorldSpace(m_pTransformCom->Get_State(STATE::POSITION), 3.f))
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CChest::Render()
{
	return S_OK;
}

void CChest::On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner)
{
	__super::On_Collision(MyColliderID, OtherColliderID, pOwner);
}


void CChest::On_Opened()
{
	if (ITEM_TYPE::COIN == m_eType)
	{
		/* 코인 생성 */
		for (_uint i = 0; i < 20; i++)
		{
			_float fAngle = XM_2PI * i / 20;

			_float fX = 1.5f * cosf(fAngle);
			_float fZ = 1.5f * sinf(fAngle);

			_float3 vSpawnPos{};
			XMStoreFloat3(&vSpawnPos, m_pTransformCom->Get_State(STATE::POSITION) + XMVectorSet(fX, 0.5f, fZ, 0.f));

			if (FAILED(Spawn_Coin(vSpawnPos)))
				continue;
		}
	}
	else
		Give_Item();

	m_IsOpend = true;
}

HRESULT CChest::Spawn_Coin(const _float3& vSpawnPos)
{
	CItem_Coin::DESC tDesc{};
	tDesc.eLevelID = m_eLevelID;
	tDesc.fSpeedPerSec = 10.f;
	tDesc.iNumPartObjects = CItem_Coin::PART_END;
	tDesc.WorldMatrix = XMMatrixTranslation(vSpawnPos.x , vSpawnPos.y, vSpawnPos.z);

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_Coin"),
		ENUM_CLASS(tDesc.eLevelID), TEXT("Layer_Coin"), &tDesc)))
		return E_FAIL;

	return S_OK;
}

void CChest::Give_Item()
{
	m_pGameInstance->Publish_Event(TEXT("Acquire_Item"), m_eType);
}

HRESULT CChest::Ready_Components(void* pArg)
{
	return S_OK;
}

HRESULT CChest::Ready_PartObjects()
{
	CBody_Chest::DESC BodyDesc{};

	BodyDesc.eLevelID = m_eLevelID;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	BodyDesc.pOwner = this;
	BodyDesc.pParentisCollision = &m_isCollision;

	if (FAILED(__super::Add_PartObject(PART_BODY, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_Body_Chest"), &BodyDesc)))
		return E_FAIL;

	CUI3D_Interaction::DESC InteractionDesc{};

	InteractionDesc.pParentLevelID = &m_eLevelID;
	InteractionDesc.iNumPartObjects = CUI3D_Interaction::PART_END;
	InteractionDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	InteractionDesc.pParentIsCollisioned = &m_isCollision;

	if (FAILED(__super::Add_PartObject(PART_INTERACTION, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_UI3D_Interaction"), &InteractionDesc)))
		return E_FAIL;


	/* 코인은 유아이 안뜨게 */
	if (ITEM_TYPE::COIN == m_eType)
		return S_OK;

	CUI2D_Reward::DESC RewardDesc{};

	RewardDesc.pParentLevelID = &m_eLevelID;
	RewardDesc.iNumPartObjects = CUI2D_Reward::PART_END;
	RewardDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	RewardDesc.pParentIsOpen = dynamic_cast<CBody_Chest*>(m_PartObjects[PART_BODY])->Get_isOpened();
	RewardDesc.pParentItemType = &m_eType;

	if (FAILED(__super::Add_PartObject(PART_REWARD, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_UI2D_Reward"), &RewardDesc)))
		return E_FAIL;

	return S_OK;
}

CChest* CChest::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CChest* pInstance = new CChest(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CChest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CChest::Clone(void* pArg)
{
	CChest* pInstance = new CChest(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CChest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChest::Free()
{
	__super::Free();
}
