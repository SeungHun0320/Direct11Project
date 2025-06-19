#include "Chest.h"
#include "GameInstance.h"

#include "Body_Chest.h"

#include "UI3D_Interaction.h"

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
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

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

	return __super::Update(fTimeDelta);
}

void CChest::Late_Update(_float fTimeDelta)
{
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

	CUI2D_Interaction::DESC InteractionDesc{};

	InteractionDesc.pParentLevelID = &m_eLevelID;
	InteractionDesc.iNumPartObjects = CUI3D_Interaction::PART_END;
	InteractionDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	InteractionDesc.pParentIsCollisioned = &m_isCollision;

	if (FAILED(__super::Add_PartObject(PART_INTERACTION, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_UI3D_Interaction"), &InteractionDesc)))
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
