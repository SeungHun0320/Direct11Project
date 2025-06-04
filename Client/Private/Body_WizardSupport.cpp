#include "Body_WizardSupport.h"

#include "GameInstance.h"

CBody_WizardSupport::CBody_WizardSupport(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CBody_Wizard{ pDevice, pContext }
{
}

CBody_WizardSupport::CBody_WizardSupport(const CBody_WizardSupport& Prototype)
    : CBody_Wizard(Prototype)
{
}

HRESULT CBody_WizardSupport::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_WizardSupport::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CBody_WizardSupport::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CBody_WizardSupport::Update(_float fTimeDelta)
{
    return __super::Update(fTimeDelta);
}

void CBody_WizardSupport::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CBody_WizardSupport::Render()
{
    return __super::Render();
}

HRESULT CBody_WizardSupport::Ready_Components(void* pArg)
{
    if (FAILED(__super::Ready_Components(pArg)))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_Wizard_Support"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

CBody_WizardSupport* CBody_WizardSupport::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_WizardSupport* pInstance = new CBody_WizardSupport(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CBody_WizardSupport");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_WizardSupport::Clone(void* pArg)
{
    CBody_WizardSupport* pInstance = new CBody_WizardSupport(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CBody_WizardSupport");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_WizardSupport::Free()
{
    __super::Free();
}
