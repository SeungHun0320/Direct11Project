#include "UI3D_PlayerSweat.h"
#include "GameInstance.h"

CUI3D_PlayerSweat::CUI3D_PlayerSweat(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI_Sprite{pDevice, pContext}
{
}

CUI3D_PlayerSweat::CUI3D_PlayerSweat(const CUI3D_PlayerSweat& Prototype)
    : CUI_Sprite(Prototype)
{
}

HRESULT CUI3D_PlayerSweat::Initialize_Prototype()
{
    m_eType = UI_TYPE::UI3D;

    return S_OK;
}

HRESULT CUI3D_PlayerSweat::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    m_pParentisNoStamina = pDesc->pParentisNoStamina;

    if (__super::Initialize(pArg))
        return E_FAIL;

    return S_OK;
}

void CUI3D_PlayerSweat::Priority_Update(_float fTimeDelta)
{
    if (!(*m_pParentisNoStamina))
        return;

    __super::Priority_Update(fTimeDelta);
}

LIFE CUI3D_PlayerSweat::Update(_float fTimeDelta)
{
    if (!(*m_pParentisNoStamina))
        return LIFE::NONE;

    return  __super::Update(fTimeDelta);
}

void CUI3D_PlayerSweat::Late_Update(_float fTimeDelta)
{
    if (!(*m_pParentisNoStamina))
        return;

    __super::Late_Update(fTimeDelta);
}

HRESULT CUI3D_PlayerSweat::Render()
{
    if (!(*m_pParentisNoStamina))
        return S_OK;

    return  __super::Render();
}

CUI3D_PlayerSweat* CUI3D_PlayerSweat::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI3D_PlayerSweat* pInstance = new CUI3D_PlayerSweat(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI3D_PlayerSweat");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI3D_PlayerSweat::Clone(void* pArg)
{
    CUI3D_PlayerSweat* pInstance = new CUI3D_PlayerSweat(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI3D_PlayerSweat");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI3D_PlayerSweat::Free()
{
    __super::Free();
}
