#include "UI2D_Reward.h"
#include "GameInstance.h"

#include "UI.h"

CUI2D_Reward::CUI2D_Reward(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIContainerPart{ pDevice, pContext }
{
}

CUI2D_Reward::CUI2D_Reward(const CUI2D_Reward& Prototype)
    : CUIContainerPart(Prototype)
{
}

HRESULT CUI2D_Reward::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI2D_Reward::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    m_pParentIsOpen = pDesc->pParentIsOpen;
    m_pParentItemType = pDesc->pParentItemType;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    if (FAILED(Ready_PartObjects()))
        return E_FAIL;

    return S_OK;
}

void CUI2D_Reward::Priority_Update(_float fTimeDelta)
{
    if (!(*m_pParentIsOpen) || m_isExpired)
        return;

    __super::Priority_Update(fTimeDelta);
}

LIFE CUI2D_Reward::Update(_float fTimeDelta)
{
    if (!(*m_pParentIsOpen) || m_isExpired)
        return LIFE::NONE;

    m_fTimeAcc += fTimeDelta;

    if (2.f <= m_fTimeAcc)
        m_isExpired = true;

    return 	__super::Update(fTimeDelta);
}

void CUI2D_Reward::Late_Update(_float fTimeDelta)
{
    if (!(*m_pParentIsOpen) || m_isExpired)
        return;

    __super::Late_Update(fTimeDelta);
}

HRESULT CUI2D_Reward::Render()
{
    return S_OK;
}

HRESULT CUI2D_Reward::Ready_Components(void* pArg)
{
    return S_OK;
}

HRESULT CUI2D_Reward::Ready_PartObjects()
{
    CUI::DESC BackingDesc{};

    LEVEL eLevelID = LEVEL::STATIC;

    BackingDesc.pParentLevelID = &eLevelID;
    BackingDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
    BackingDesc.fSizeX = 320.f;
    BackingDesc.fSizeY = 320.f;
    BackingDesc.fX = g_iWinSizeX * 0.5f;
    BackingDesc.fY = g_iWinSizeY * 0.35f;
    BackingDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_Item_Backing");

    if (FAILED(__super::Add_PartObject(PART_BACKING, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &BackingDesc)))
        return E_FAIL;

    CUI::DESC ItemSlotDesc{};

    ItemSlotDesc.pParentLevelID = &eLevelID;
    ItemSlotDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
    ItemSlotDesc.fSizeX = 200.f;
    ItemSlotDesc.fSizeY = 200.f;
    ItemSlotDesc.fX = (g_iWinSizeX * 0.5f);
    ItemSlotDesc.fY = g_iWinSizeY * 0.35f;
    ItemSlotDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_UIItems");
    ItemSlotDesc.iTextureIndex = ENUM_CLASS((*m_pParentItemType));

    if (FAILED(__super::Add_PartObject(PART_ITEMS, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &ItemSlotDesc)))
        return E_FAIL;

    return S_OK;
}

CUI2D_Reward* CUI2D_Reward::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI2D_Reward* pInstance = new CUI2D_Reward(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI2D_Reward");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI2D_Reward::Clone(void* pArg)
{
    CUI2D_Reward* pInstance = new CUI2D_Reward(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI2D_Reward");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI2D_Reward::Free()
{
    __super::Free();
}
