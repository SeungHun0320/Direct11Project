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
    m_iCoin = 0;

    /* 포션 */
    m_iNumPotion = 2;
    m_iCurNumPotion = 2;

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
    Delegate<> InvenPotionDele;
    InvenPotionDele.Bind<CInventory, &CInventory::Acquire_Potion>(this);
    m_pGameInstance->Subscribe_Event(TEXT("Acquire_Potion"), InvenPotionDele);

    Delegate<_bool> InvenShiledDele;
    InvenShiledDele.Bind<CInventory, &CInventory::Acquire_Shield>(this);
    m_pGameInstance->Subscribe_Event(TEXT("Acquire_Shield"), InvenShiledDele);
}

void CInventory::Acquire_Potion()
{
    Add_Potion();
}

void CInventory::Acquire_Shield(_bool isShield)
{
    Set_isShield(isShield);
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
}
