#include "Body_WizardSword.h"

#include "GameInstance.h"

CBody_WizardSword::CBody_WizardSword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CBody_Wizard{ pDevice, pContext }
{
}

CBody_WizardSword::CBody_WizardSword(const CBody_WizardSword& Prototype)
    : CBody_Wizard(Prototype)
{
}

HRESULT CBody_WizardSword::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_WizardSword::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_vEmissiveColor = _float4(0.74f, 0.19f, 0.f, 1.f);

    return S_OK;
}

void CBody_WizardSword::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CBody_WizardSword::Update(_float fTimeDelta)
{
    return __super::Update(fTimeDelta);
}

void CBody_WizardSword::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CBody_WizardSword::Render()
{
    return __super::Render();
}

HRESULT CBody_WizardSword::Ready_Components(void* pArg)
{
    if (FAILED(__super::Ready_Components(pArg)))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_Wizard_Sword"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

CBody_WizardSword* CBody_WizardSword::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_WizardSword* pInstance = new CBody_WizardSword(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CBody_WizardSword");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_WizardSword::Clone(void* pArg)
{
    CBody_WizardSword* pInstance = new CBody_WizardSword(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CBody_WizardSword");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_WizardSword::Free()
{
    __super::Free();
}
