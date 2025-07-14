#include "UI2D_Purchase.h"
#include "GameInstance.h"

#include "UI.h"
#include "UI_Animation.h"

CUI2D_Purchase::CUI2D_Purchase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIContainerPart{pDevice, pContext}
{
}

CUI2D_Purchase::CUI2D_Purchase(const CUI2D_Purchase& Prototype)
    :CUIContainerPart(Prototype)
{
}

HRESULT CUI2D_Purchase::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI2D_Purchase::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    m_pParentPrice = pDesc->pParentPrice;
    m_pParentisDeal = pDesc->pParentisDeal;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;


    if (CUI* pPart = dynamic_cast<CUI*>(m_PartObjects[PART_BUY_BUTTON]))
        pPart->Set_TextureIndex(1);

    Subscribe_Events();

    return S_OK;
}

void CUI2D_Purchase::Priority_Update(_float fTimeDelta)
{
    if (!(*m_pParentisDeal))
        return;

    __super::Priority_Update(fTimeDelta);
}

LIFE CUI2D_Purchase::Update(_float fTimeDelta)
{
    if (!(*m_pParentisDeal))
        return LIFE::NONE;

    return __super::Update(fTimeDelta);
}

void CUI2D_Purchase::Late_Update(_float fTimeDelta)
{
    if (!(*m_pParentisDeal))
        return;

    __super::Late_Update(fTimeDelta);

    m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CUI2D_Purchase::Render()
{
    _int iPrice = (*m_pParentPrice);
    const _wstring& strMoney = to_wstring(iPrice);
    const _wstring& strQuestionMark = TEXT("             ?");
    const _wstring& strPrice = strMoney + strQuestionMark;

    const _wstring& strChoose = TEXT("확인                                       아니요");

    m_pGameInstance->Draw_Font(TEXT("Font_Money"), strPrice.c_str(), _float2(610.f, 270.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 1.2f);

    m_pGameInstance->Draw_Font(TEXT("Font_Money"), strChoose.c_str(), _float2(445.f, 382.5f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 1.1f);

    return S_OK;
}

void CUI2D_Purchase::ChooseButton(PART ePart)
{
    if ((*m_pParentisDeal))
    {
        if (CUI* pPart = dynamic_cast<CUI*>(m_PartObjects[ENUM_CLASS(ePart)]))
            pPart->Set_ButtonTextureIndex();
    }
}

void CUI2D_Purchase::Subscribe_Events() 
{
    Delegate<PART> DealButtonDele;
    DealButtonDele.Bind<CUI2D_Purchase, &CUI2D_Purchase::ChooseButton>(this);
    m_pGameInstance->Subscribe_Event(TEXT("Choose_PurchaseUI"), DealButtonDele);
}

HRESULT CUI2D_Purchase::Ready_Components(void* pArg)
{
    return S_OK;
}

HRESULT CUI2D_Purchase::Ready_PartObjects()
{
    CUI::DESC UIBoxDesc{};
    UIBoxDesc.pParentLevelID = m_pLevelID;
    UIBoxDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
    UIBoxDesc.fSizeX = 833.5f;
    UIBoxDesc.fSizeY = 329.f;
    UIBoxDesc.fX = g_iWinSizeX * 0.5f;
    UIBoxDesc.fY = g_iWinSizeY * 0.5f;
    UIBoxDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_UI_Box");

    if (FAILED(__super::Add_PartObject(PART_UIBOX, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &UIBoxDesc)))
        return E_FAIL;
    
    CUI::DESC NameDesc{};
    NameDesc.pParentLevelID = m_pLevelID;
    NameDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
    NameDesc.fSizeX = 91.5f;
    NameDesc.fSizeY = 49.4f;
    NameDesc.fX = g_iWinSizeX * 0.42f;
    NameDesc.fY = g_iWinSizeY * 0.4f;
    NameDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_Direction");

    if (FAILED(__super::Add_PartObject(PART_NAME, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &NameDesc)))
        return E_FAIL;

    CUI::DESC MoneyDesc{};
    MoneyDesc.pParentLevelID = m_pLevelID;
    MoneyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
    MoneyDesc.fSizeX = 73.1f;
    MoneyDesc.fSizeY = 73.1f;
    MoneyDesc.fX = g_iWinSizeX * 0.56f;
    MoneyDesc.fY = g_iWinSizeY * 0.4f;
    MoneyDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_Money");

    if (FAILED(__super::Add_PartObject(PART_MONEY, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &MoneyDesc)))
        return E_FAIL;

    CUI::DESC BuyButtonDesc{};
    BuyButtonDesc.pParentLevelID = m_pLevelID;
    BuyButtonDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
    BuyButtonDesc.fSizeX = 288.f;
    BuyButtonDesc.fSizeY = 51.2f;
    BuyButtonDesc.fX = g_iWinSizeX * 0.375f;
    BuyButtonDesc.fY = g_iWinSizeY * 0.55f;
    BuyButtonDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_UI_SelectionFrame");
    BuyButtonDesc.iTextureIndex = 1;

    if (FAILED(__super::Add_PartObject(PART_BUY_BUTTON, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &BuyButtonDesc)))
        return E_FAIL;

    CUI::DESC CancelButtonDesc{};
    CancelButtonDesc.pParentLevelID = m_pLevelID;
    CancelButtonDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
    CancelButtonDesc.fSizeX = 288.f;
    CancelButtonDesc.fSizeY = 51.2f;
    CancelButtonDesc.fX = g_iWinSizeX * 0.625f;
    CancelButtonDesc.fY = g_iWinSizeY * 0.55f;
    CancelButtonDesc.strPrototypeTag = TEXT("Prototype_Component_Texture_UI_SelectionFrame");

    if (FAILED(__super::Add_PartObject(PART_CANCEL_BUTTON, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_UI2D"), &CancelButtonDesc)))
        return E_FAIL;

    return S_OK;
}

CUI2D_Purchase* CUI2D_Purchase::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI2D_Purchase* pInstance = new CUI2D_Purchase(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI2D_Purchase");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI2D_Purchase::Clone(void* pArg)
{
    CUI2D_Purchase* pInstance = new CUI2D_Purchase(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI2D_Purchase");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI2D_Purchase::Free()
{
    __super::Free();
}
