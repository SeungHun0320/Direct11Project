#include "UI2D_PlayerItemSlots.h"
#include "GameInstance.h"

#include "UI.h"
#include "Inventory.h"

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
	m_pInventory = pDesc->pInventory;

	if (nullptr == m_pInventory)
		return E_FAIL;

	Safe_AddRef(m_pInventory);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Set_TextureIndex(PART_J_ITEMS, 7);

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
	for (_uint i = 0; i < CInventory::QSLOT_END; i++)
	{
		const CInventory::QUICK_SLOT QuickSlot = m_pInventory->Get_QuickSlot(static_cast<CInventory::QSLOT_TYPE>(i));
		const CInventory::INVEN_SLOT InvenSlot = m_pInventory->Get_Slot(static_cast<CInventory::SLOT>(i));

		_uint iSlotItemIndex = PART_J_ITEMS + i;

		if (QuickSlot.bHasItem)
		{
			Set_UIVisible(iSlotItemIndex, true);
			Set_TextureIndex(iSlotItemIndex, ENUM_CLASS(QuickSlot.eType));
		}
		else
		{
			Set_UIVisible(iSlotItemIndex, false);
		}

	}

	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CUI2D_PlayerItemSlots::Render()
{
	for (_uint i = 0; i <= PART_L; i++)
	{
		const CInventory::QUICK_SLOT QuickSlot = m_pInventory->Get_QuickSlot(static_cast<CInventory::QSLOT_TYPE>(i));
		const CInventory::INVEN_SLOT InvenSlot = m_pInventory->Get_Slot(static_cast<CInventory::SLOT>(QuickSlot.iInvenSlotIndex));

		if (!QuickSlot.bHasItem || 0 >= InvenSlot.iCount)
			continue;

		const wstring& strCount = to_wstring(InvenSlot.iCount);

		_float3 vPos{};
		XMStoreFloat3(&vPos, Get_State(i, STATE::POSITION));

		_float2 vFontPos = _float2(vPos.x + 655.f, vPos.y - 200.f);

		m_pGameInstance->Draw_Font(TEXT("Font_Money"), strCount.c_str(), vFontPos, XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 0.8f);
	}

	return S_OK;
}

_vector CUI2D_PlayerItemSlots::Get_State(_int iPartIndex, STATE eState)
{
	return static_cast<CUI*>(m_PartObjects[iPartIndex])->Get_State(eState);
}

HRESULT CUI2D_PlayerItemSlots::Ready_Components(void* pArg)
{
	return S_OK;
}

HRESULT CUI2D_PlayerItemSlots::Ready_PartObjects()
{
	for (_uint i = 0; i <= PART_L; i++)
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

	for (_uint i = PART_J_ITEMS; i <= PART_L_ITEMS; i++)
	{
		CUI::DESC ItemSlotDesc{};

		ItemSlotDesc.pParentLevelID = m_pLevelID;
		ItemSlotDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
		ItemSlotDesc.fSizeX = 50.f;
		ItemSlotDesc.fSizeY = 50.f;
		ItemSlotDesc.fX = (g_iWinSizeX * 0.612f) + (80 * i);
		ItemSlotDesc.fY = g_iWinSizeY * 0.1f;
		ItemSlotDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_UIItems");

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

	Safe_Release(m_pInventory);
}
