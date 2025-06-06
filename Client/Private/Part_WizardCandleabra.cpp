#include "Part_WizardCandleabra.h"

#include "GameInstance.h"

CPart_WizardCandleabra::CPart_WizardCandleabra(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CWizard_Weapon{ pDevice, pContext }
{
}

CPart_WizardCandleabra::CPart_WizardCandleabra(const CPart_WizardCandleabra& Prototype)
    : CWizard_Weapon(Prototype)
{
}

HRESULT CPart_WizardCandleabra::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPart_WizardCandleabra::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_pTransformCom->Rotation(XMConvertToRadians(0.f), XMConvertToRadians(90.f), XMConvertToRadians(0.f));

    return S_OK;
}

void CPart_WizardCandleabra::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CPart_WizardCandleabra::Update(_float fTimeDelta)
{
    return __super::Update(fTimeDelta);
}

void CPart_WizardCandleabra::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CPart_WizardCandleabra::Render()
{
    return __super::Render();
}

void CPart_WizardCandleabra::Set_Active(_bool isActive)
{
    m_pColliderCom->Set_Active(isActive);
}

HRESULT CPart_WizardCandleabra::Ready_Components(void* pArg)
{
    if (FAILED(__super::Ready_Components(pArg)))
        return E_FAIL;

    DESC* pDesc = static_cast<DESC*>(pArg);

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_Weapon_WizardCandleabra"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    CBounding_OBB::DESC	OBBDesc{};
    OBBDesc.vExtents = _float3(1.25f, 1.2f, 2.5f);
    OBBDesc.vCenter = _float3(-0.75f, OBBDesc.vExtents.y, OBBDesc.vExtents.z);
    OBBDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::MONSTER_ATTACK);
    OBBDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::WIZARD_ATTACK);
    OBBDesc.pOwner = pDesc->pOwner;

    /* For.Com_Collider */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_OBB"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &OBBDesc)))
        return E_FAIL;

    return S_OK;
}


CPart_WizardCandleabra* CPart_WizardCandleabra::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPart_WizardCandleabra* pInstance = new CPart_WizardCandleabra(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CPart_WizardCandleabra");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPart_WizardCandleabra::Clone(void* pArg)
{
    CPart_WizardCandleabra* pInstance = new CPart_WizardCandleabra(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CPart_WizardCandleabra");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPart_WizardCandleabra::Free()
{
    __super::Free();
}
