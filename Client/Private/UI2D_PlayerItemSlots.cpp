#include "UI2D_PlayerItemSlots.h"
#include "GameInstance.h"

#include "UI.h"

CUI2D_PlayerItemSlots::CUI2D_PlayerItemSlots(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIContainerPart{ pDevice, pContext }
{
}

CUI2D_PlayerItemSlots::CUI2D_PlayerItemSlots(const CUI2D_PlayerItemSlots& Prototype)
	: CUIContainerPart(Prototype)
{
}

HRESULT CUI2D_PlayerItemSlots::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI2D_PlayerItemSlots::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}

void CUI2D_PlayerItemSlots::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CUI2D_PlayerItemSlots::Update(_float fTimeDelta)
{
	return 	__super::Update(fTimeDelta);
}

void CUI2D_PlayerItemSlots::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CUI2D_PlayerItemSlots::Render()
{
	return __super::Render();
}

void CUI2D_PlayerItemSlots::Set_UIVisible(_uint iPart, _bool isVisible)
{
	static_cast<CUI*>(m_PartObjects[iPart])->Set_Visible(isVisible);
}

void CUI2D_PlayerItemSlots::Set_TextureIndex(_uint iPart, _uint iTextureIdx)
{
	static_cast<CUI*>(m_PartObjects[iPart])->Set_TextureIndex(iTextureIdx);
}

HRESULT CUI2D_PlayerItemSlots::Ready_Components(void* pArg)
{
	return S_OK;
}

HRESULT CUI2D_PlayerItemSlots::Ready_PartObjects()
{
	for (_uint i = 0; i < PART_END; i++)
	{
		CUI::DESC ItemSlotDesc{};

		ItemSlotDesc.pParentLevelID = m_pLevelID;
		ItemSlotDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
		ItemSlotDesc.fSizeX = 75.f;
		ItemSlotDesc.fSizeY = 75.f;
		ItemSlotDesc.fX = (g_iWinSizeX * 0.8f) + (80 * i);
		ItemSlotDesc.fY = g_iWinSizeY * 0.1f;
		ItemSlotDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_ItemSlot");

		if (FAILED(__super::Add_PartObject(i, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &ItemSlotDesc)))
			return E_FAIL;
	}

	return S_OK;
}

CUI2D_PlayerItemSlots* CUI2D_PlayerItemSlots::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI2D_PlayerItemSlots* pInstance = new CUI2D_PlayerItemSlots(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI2D_PlayerItemSlots");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI2D_PlayerItemSlots::Clone(void* pArg)
{
	CUI2D_PlayerItemSlots* pInstance = new CUI2D_PlayerItemSlots(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI2D_PlayerItemSlots");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI2D_PlayerItemSlots::Free()
{
	__super::Free();
}
