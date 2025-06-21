#include "UI2D_InventorySlot.h"
#include "GameInstance.h"

#include "UI.h"
#include "Inventory.h"

CUI2D_InventorySlot::CUI2D_InventorySlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIContainerPart {pDevice, pContext}
{
}

CUI2D_InventorySlot::CUI2D_InventorySlot(const CUI2D_InventorySlot& Prototype)
    :CUIContainerPart(Prototype)
{
}

HRESULT CUI2D_InventorySlot::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI2D_InventorySlot::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    m_pParentIsOnInven = pDesc->pParentIsOnInven;
    m_pInventory = pDesc->pInventory;

    if (nullptr == m_pInventory)
        return E_FAIL;

    Safe_AddRef(m_pInventory);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    if (FAILED(Ready_PartObjects()))
        return E_FAIL;

    Set_UIVisible(PART_SELECTOR, false);
    Set_UIVisible(PART_ITEMS, false);

    return S_OK;
}

void CUI2D_InventorySlot::Priority_Update(_float fTimeDelta)
{
    if (!(*m_pParentIsOnInven))
        return;

    __super::Priority_Update(fTimeDelta);
}

LIFE CUI2D_InventorySlot::Update(_float fTimeDelta)
{
    if (!(*m_pParentIsOnInven))
        return LIFE::NONE;

    return 	__super::Update(fTimeDelta);
}

void CUI2D_InventorySlot::Late_Update(_float fTimeDelta)
{
    if (!(*m_pParentIsOnInven))
        return;

    __super::Late_Update(fTimeDelta);

    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CUI2D_InventorySlot::Render()
{
    if (!(*m_pParentIsOnInven))
        return E_FAIL;

    const wstring& strCount = to_wstring(m_iCount);

    _float3 vPos{};
    XMStoreFloat3(&vPos, m_pTransformCom->Get_State(STATE::POSITION));
    _float2 vFontPos = _float2(fabs(vPos.x) + 230.f, fabs(vPos.y) + 370.f);

    if(0 != m_iCount)
        m_pGameInstance->Draw_Font(TEXT("Font_Money"), strCount.c_str(), vFontPos, XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 0.8f);
        
    return S_OK;
}

_fvector CUI2D_InventorySlot::Get_State(STATE eState)
{
    return m_pTransformCom->Get_State(eState);
}

void CUI2D_InventorySlot::Set_State(STATE eState, _fvector vState)
{
    m_pTransformCom->Set_State(eState, vState);
}

void CUI2D_InventorySlot::Set_Selected(_bool isSelected)
{
    Set_UIVisible(PART_SELECTOR, isSelected);
}

HRESULT CUI2D_InventorySlot::Ready_Components(void* pArg)
{
    return S_OK;
}

HRESULT CUI2D_InventorySlot::Ready_PartObjects()
{
    CUI::DESC BackingDesc{};

    BackingDesc.pParentLevelID = m_pLevelID;
    BackingDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
    BackingDesc.fSizeX = 80.f;
    BackingDesc.fSizeY = 80.f;
    BackingDesc.fX = g_iWinSizeX * 0.17f;
    BackingDesc.fY = g_iWinSizeY * 0.5f;
    BackingDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_Item_Backing");

    if (FAILED(__super::Add_PartObject(PART_BACK, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &BackingDesc)))
        return E_FAIL;

    CUI::DESC UIItemsDesc{};

    UIItemsDesc.pParentLevelID = m_pLevelID;
    UIItemsDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
    UIItemsDesc.fSizeX = 60.f;
    UIItemsDesc.fSizeY = 60.f;
    UIItemsDesc.fX = g_iWinSizeX * 0.17f;
    UIItemsDesc.fY = g_iWinSizeY * 0.5f;
    UIItemsDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_UIItems");

    if (FAILED(__super::Add_PartObject(PART_ITEMS, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &UIItemsDesc)))
        return E_FAIL;

    CUI::DESC UISelectorDesc{};

    UISelectorDesc.pParentLevelID = m_pLevelID;
    UISelectorDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
    UISelectorDesc.fSizeX = 80.f;
    UISelectorDesc.fSizeY = 80.f;
    UISelectorDesc.fX = g_iWinSizeX * 0.17f;
    UISelectorDesc.fY = g_iWinSizeY * 0.5f;
    UISelectorDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_UISelector");

    if (FAILED(__super::Add_PartObject(PART_SELECTOR, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &UISelectorDesc)))
        return E_FAIL;

    return S_OK;
}

CUI2D_InventorySlot* CUI2D_InventorySlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI2D_InventorySlot* pInstance = new CUI2D_InventorySlot(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI2D_Inventory");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI2D_InventorySlot::Clone(void* pArg)
{
    CUI2D_InventorySlot* pInstance = new CUI2D_InventorySlot(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI2D_Inventory");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI2D_InventorySlot::Free()
{
    __super::Free();

    Safe_Release(m_pInventory);
}
