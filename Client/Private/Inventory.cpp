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
    
    /* 코인 */
    m_iCoin = 0;

    /* 포션 */
    m_iNumPotion = 2;
    m_iCurNumPotion = 2;

    /* 방패 */
    m_isShield = true;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    return S_OK;
}

void CInventory::Priority_Update(_float fTimeDelta)
{
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

void CInventory::Acquire_Potion()
{
    Add_Potion();
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
