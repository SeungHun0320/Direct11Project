#include "Inventory.h"
#include "GameInstance.h"

#include "Item.h"

CInventory::CInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject{pDevice, pContext}
{
}

CInventory::CInventory(const CInventory& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CInventory::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInventory::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    m_pLevelID = pDesc->pParentLevelID;
    m_pParentIsOnInven = pDesc->pParentIsOnInven;
    
    /* 코인 */
    m_iCoin = 1000;

    /* 포션 */
    m_iNumPotion = 0;
    m_iCurNumPotion = 0;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    Subscribe_Events();

    return S_OK;
}

void CInventory::Priority_Update(_float fTimeDelta)
{
    Key_Input();
}

LIFE CInventory::Update(_float fTimeDelta)
{
    return LIFE::NONE;
}

void CInventory::Late_Update(_float fTimeDelta)
{
}

HRESULT CInventory::Render()
{
    return S_OK;
}

void CInventory::Subscribe_Events()
{
    Delegate<ITEM_TYPE> InvenItemDele;
    InvenItemDele.Bind<CInventory, &CInventory::Acquire_Item>(this);
    m_pGameInstance->Subscribe_Event(TEXT("Acquire_Item"), InvenItemDele);

    Delegate<ITEM_TYPE, _int, _bool*> BuyShopItemDele;
    BuyShopItemDele.Bind<CInventory, &CInventory::Buy_ShopItem>(this);
    m_pGameInstance->Subscribe_Event(TEXT("Buy_ShopItem"), BuyShopItemDele);

}

void CInventory::Acquire_Item(ITEM_TYPE eType)
{
    switch (eType)
    {
    case ITEM_TYPE::BERRY:
    case ITEM_TYPE::BLUEBERRY:
    case ITEM_TYPE::COIN_QUESTION:
        Add_UseItemSlot(eType);
        break;
    case ITEM_TYPE::FIRE_CRACKER:
        Add_UseItemSlot(eType, 3);
        break;
    case ITEM_TYPE::STICK:
    case ITEM_TYPE::SWORD:
    case ITEM_TYPE::DAGGER:
        Add_WeaponSlot(eType);
        break;
    case ITEM_TYPE::POTION:
        Add_Potion();
        break;
    case ITEM_TYPE::SHILED:
        Set_isShield(true);
        m_pGameInstance->Publish_Event(TEXT("Equip_Shield"));
        break;
    default:
        break;
    }
}

void CInventory::Buy_ShopItem(ITEM_TYPE eType, _int iPrice, _bool* pSell)
{
    if (iPrice > m_iCoin)
    {
        (*pSell) = false;
        return;
    }
       
    Use_Coin(iPrice);
    Acquire_Item(eType);
    (*pSell) = true;
    m_pGameInstance->Publish_Event(TEXT("Sucsess_Deal"), true);
}

_bool CInventory::Add_Potion(_int iCount)
{
    if (m_iNumPotion >= m_iMaxNumPotion)
        return false;

    m_iNumPotion = clamp(m_iNumPotion + iCount, 0, m_iMaxNumPotion);
    m_iCurNumPotion =  clamp(m_iCurNumPotion + iCount, 0, m_iNumPotion);

    return true;
}

_bool CInventory::Use_Potion()
{
    if (0 >= m_iCurNumPotion)
        return false;

    m_iCurNumPotion = max(m_iCurNumPotion - 1, 0);
    return true;
}

void CInventory::Add_UseItemSlot(ITEM_TYPE eType, _int iCount)
{
    for (_uint i = SLOT_USEITEM0; i <= SLOT_USEITEM3; i++)
    {
        if (m_InvenSlot[i].bHasItem && eType == m_InvenSlot[i].eType)
        {
            m_InvenSlot[i].iCount += iCount;
            return;
        }
    }

    for (_uint i = SLOT_USEITEM0; i <= SLOT_USEITEM3; i++)
    {
        if (!m_InvenSlot[i].bHasItem)
        {
            m_InvenSlot[i].eType = eType;
            m_InvenSlot[i].bHasItem = true;
            ++m_InvenSlot[i].iCount = iCount;
            return;
        }
    }
}

void CInventory::Add_WeaponSlot(ITEM_TYPE eType)
{
    for (_uint i = SLOT_WEAPON0; i <= SLOT_WEAPON2; i++)
    {
        if (m_InvenSlot[i].bHasItem && eType == m_InvenSlot[i].eType)
            return;
    }

    for (_uint i = SLOT_WEAPON0; i <= SLOT_WEAPON2; i++)
    {
        if (!m_InvenSlot[i].bHasItem)
        {
            m_InvenSlot[i].eType = eType;
            m_InvenSlot[i].bHasItem = true;
            return;
        }
    }
}

_bool CInventory::Is_UseItem(ITEM_TYPE eType)
{
    switch (eType)
    {
    case ITEM_TYPE::BERRY:
    case ITEM_TYPE::BLUEBERRY:
    case ITEM_TYPE::COIN_QUESTION:
    case ITEM_TYPE::FIRE_CRACKER:
        return true;
    default:
        return false;
    }
}

_bool CInventory::Use_QuickSlot(QSLOT_TYPE eSlot)
{
    if (!m_QuickSlot[eSlot].bHasItem)
        return false;

    if (0 >= m_InvenSlot[m_QuickSlot[eSlot].iInvenSlotIndex].iCount)
        return false;

    --m_InvenSlot[m_QuickSlot[eSlot].iInvenSlotIndex].iCount;

    if (0 == m_InvenSlot[m_QuickSlot[eSlot].iInvenSlotIndex].iCount)
    {
        m_InvenSlot[m_QuickSlot[eSlot].iInvenSlotIndex].iCount = 0;
        m_InvenSlot[m_QuickSlot[eSlot].iInvenSlotIndex].eType = ITEM_TYPE::IT_END;
        m_InvenSlot[m_QuickSlot[eSlot].iInvenSlotIndex].bHasItem = false;

        m_QuickSlot[eSlot].eType = ITEM_TYPE::IT_END;
        m_QuickSlot[eSlot].iInvenSlotIndex = 0;
        m_QuickSlot[eSlot].bHasItem = false;
    }

    return true;
}

ITEM_TYPE CInventory::IsWeaponInQuickSlot(QSLOT_TYPE eQuickSlot)
{
    switch (m_QuickSlot[eQuickSlot].eType)
    {
    case ITEM_TYPE::STICK:
    case ITEM_TYPE::SWORD:
    case ITEM_TYPE::DAGGER:
        return m_QuickSlot[eQuickSlot].eType;
    default:
        break;
    }

    return ITEM_TYPE::IT_END;
}

void CInventory::Register_QuickSlot(_uint iQuickSlotIndex)
{
    if (QSLOT_END <= iQuickSlotIndex)
        return;

    if (!m_InvenSlot[m_iSelectSlotIndex].bHasItem)
        return;

    const ITEM_TYPE eType = m_InvenSlot[m_iSelectSlotIndex].eType;

    for (_uint i = 0; i < QSLOT_END; i++)
    {
        if (i == iQuickSlotIndex)
        {
            // 애니메이션 처리 해주면 될듯
            continue;
        }

        if (eType == m_QuickSlot[i].eType)
        {
            m_QuickSlot[i].bHasItem = false;
            m_QuickSlot[i].eType = ITEM_TYPE::IT_END;
            m_QuickSlot->iInvenSlotIndex = 0;
        }
    }

    m_QuickSlot[iQuickSlotIndex].bHasItem = true;
    m_QuickSlot[iQuickSlotIndex].eType = eType;
    m_QuickSlot[iQuickSlotIndex].iInvenSlotIndex = m_iSelectSlotIndex;
}

void CInventory::Key_Input()
{
    if (!(*m_pParentIsOnInven))
        return;

    if (KEY_DOWN(DIK_UP))
        Move_Selector(+1);
    if (KEY_DOWN(DIK_DOWN))
        Move_Selector(-1);
    if (KEY_DOWN(DIK_LEFT))
        Move_Selector(-1);
    if (KEY_DOWN(DIK_RIGHT))
        Move_Selector(+1);

    if (KEY_DOWN(DIK_J))
        Register_QuickSlot(QSLOT_J);
    if(KEY_DOWN(DIK_K))
        Register_QuickSlot(QSLOT_K);
    if(KEY_DOWN(DIK_L))
        Register_QuickSlot(QSLOT_L);
}

void CInventory::Move_Selector(_uint iSlotIndex)
{
    m_iSelectSlotIndex += iSlotIndex;
    m_iSelectSlotIndex = clamp(m_iSelectSlotIndex, 1, (_int)SLOT_END - 1);
}

HRESULT CInventory::Ready_Components(void* pArg)
{
    return S_OK;
}

CInventory* CInventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CInventory* pInstance = new CInventory(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CInventory");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CInventory::Clone(void* pArg)
{
    CInventory* pInstance = new CInventory(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CInventory");
        Safe_Release(pInstance);
    }

    return pInstance;
}
void CInventory::Free()
{
    __super::Free();

    m_pGameInstance->Unsubscribe_Event<ITEM_TYPE>(this);
    m_pGameInstance->Unsubscribe_Event<ITEM_TYPE, _int, _bool*>(this);
}
