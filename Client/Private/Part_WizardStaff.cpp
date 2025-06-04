#include "Part_WizardStaff.h"

#include "GameInstance.h"

CPart_WizardStaff::CPart_WizardStaff(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CWizard_Weapon{ pDevice, pContext }
{
}

CPart_WizardStaff::CPart_WizardStaff(const CPart_WizardStaff& Prototype)
    : CWizard_Weapon(Prototype)
{
}

HRESULT CPart_WizardStaff::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPart_WizardStaff::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    m_pParentState = pDesc->pParentState;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CPart_WizardStaff::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CPart_WizardStaff::Update(_float fTimeDelta)
{
    return __super::Update(fTimeDelta);
}

void CPart_WizardStaff::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CPart_WizardStaff::Render()
{
    return __super::Render();
}

HRESULT CPart_WizardStaff::Ready_Components(void* pArg)
{
    if (FAILED(__super::Ready_Components(pArg)))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_Weapon_WizardStaff"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

CPart_WizardStaff* CPart_WizardStaff::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPart_WizardStaff* pInstance = new CPart_WizardStaff(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CPart_WizardStaff");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPart_WizardStaff::Clone(void* pArg)
{
    CPart_WizardStaff* pInstance = new CPart_WizardStaff(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CPart_WizardStaff");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPart_WizardStaff::Free()
{
    __super::Free();
}
