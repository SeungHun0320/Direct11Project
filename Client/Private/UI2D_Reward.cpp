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
    __super::Priority_Update(fTimeDelta);
}

LIFE CUI2D_Reward::Update(_float fTimeDelta)
{
    return 	__super::Update(fTimeDelta);
}

void CUI2D_Reward::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CUI2D_Reward::Render()
{
    return __super::Render();
}

HRESULT CUI2D_Reward::Ready_Components(void* pArg)
{
    return E_NOTIMPL;
}

HRESULT CUI2D_Reward::Ready_PartObjects()
{
    return E_NOTIMPL;
}

CUI2D_Reward* CUI2D_Reward::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    return nullptr;
}

CGameObject* CUI2D_Reward::Clone(void* pArg)
{
    return nullptr;
}

void CUI2D_Reward::Free()
{
}
