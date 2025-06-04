#include "Part_WizardSword.h"

#include "GameInstance.h"

CPart_WizardSword::CPart_WizardSword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CWizard_Weapon{ pDevice, pContext }
{
}

CPart_WizardSword::CPart_WizardSword(const CPart_WizardSword& Prototype)
    : CWizard_Weapon(Prototype)
{
}

HRESULT CPart_WizardSword::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPart_WizardSword::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_pTransformCom->Scaling(pDesc->vScale);

    return S_OK;
}

void CPart_WizardSword::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CPart_WizardSword::Update(_float fTimeDelta)
{
    return __super::Update(fTimeDelta);
}

void CPart_WizardSword::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CPart_WizardSword::Render()
{
    return __super::Render();
}

HRESULT CPart_WizardSword::Ready_Components(void* pArg)
{
    if (FAILED(__super::Ready_Components(pArg)))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_Weapon_WizardSword"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

CPart_WizardSword* CPart_WizardSword::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPart_WizardSword* pInstance = new CPart_WizardSword(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CPart_WizardSword");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPart_WizardSword::Clone(void* pArg)
{
    CPart_WizardSword* pInstance = new CPart_WizardSword(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CPart_WizardSword");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPart_WizardSword::Free()
{
    __super::Free();
}
