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
    return E_NOTIMPL;
}

HRESULT CInventory::Initialize(void* pArg)
{
    return E_NOTIMPL;
}

void CInventory::Priority_Update(_float fTimeDelta)
{
}

LIFE CInventory::Update(_float fTimeDelta)
{
    return LIFE();
}

void CInventory::Late_Update(_float fTimeDelta)
{
}

HRESULT CInventory::Render()
{
    return E_NOTIMPL;
}

HRESULT CInventory::Ready_Components(void* pArg)
{
    return E_NOTIMPL;
}

CInventory* CInventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    return nullptr;
}

CGameObject* CInventory::Clone(void* pArg)
{
    return nullptr;
}

void CInventory::Free()
{
}
