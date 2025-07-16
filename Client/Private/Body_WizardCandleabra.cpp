#include "Body_WizardCandleabra.h"

#include "GameInstance.h"

CBody_WizardCandleabra::CBody_WizardCandleabra(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CBody_Wizard{ pDevice, pContext }
{
}

CBody_WizardCandleabra::CBody_WizardCandleabra(const CBody_WizardCandleabra& Prototype)
    : CBody_Wizard(Prototype)
{
}

HRESULT CBody_WizardCandleabra::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_WizardCandleabra::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_vEmissiveColor = _float4(0.74f, 0.08f, 0.08f, 1.f);

    return S_OK;
}

void CBody_WizardCandleabra::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CBody_WizardCandleabra::Update(_float fTimeDelta)
{
    return __super::Update(fTimeDelta);
}

void CBody_WizardCandleabra::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CBody_WizardCandleabra::Render()
{
    return __super::Render();
} 

HRESULT CBody_WizardCandleabra::Ready_Components(void* pArg)
{
    if (FAILED(__super::Ready_Components(pArg)))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_Wizard_Candleabra"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

CBody_WizardCandleabra* CBody_WizardCandleabra::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_WizardCandleabra* pInstance = new CBody_WizardCandleabra(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CBody_WizardCandleabra");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_WizardCandleabra::Clone(void* pArg)
{
    CBody_WizardCandleabra* pInstance = new CBody_WizardCandleabra(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CBody_WizardCandleabra");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_WizardCandleabra::Free()
{
    __super::Free();
}
