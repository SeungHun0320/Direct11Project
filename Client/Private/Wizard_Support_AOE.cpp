#include "Wizard_Support_AOE.h"
#include "GameInstance.h"

#include "Effect_Obj.h"

CWizard_Support_AOE::CWizard_Support_AOE(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CBullet_Monster_AOE{pDevice, pContext}
{
}

CWizard_Support_AOE::CWizard_Support_AOE(const CWizard_Support_AOE& Prototype)
    :CBullet_Monster_AOE(Prototype)
{
}

HRESULT CWizard_Support_AOE::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CWizard_Support_AOE::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_pTransformCom->Set_State(STATE::POSITION, m_pTargetTransform->Get_State(STATE::POSITION));

    if (FAILED(Ready_Effects(TEXT("Layer_Effect"))))
        return E_FAIL;

    m_fDeadTime = 5.f;
    m_fAttack = 10.f;
    m_fStaggerValue = 10.f;

    return S_OK;
}

void CWizard_Support_AOE::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CWizard_Support_AOE::Update(_float fTimeDelta)
{
    return __super::Update(fTimeDelta);
}

void CWizard_Support_AOE::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CWizard_Support_AOE::Render()
{
    return S_OK;
}

HRESULT CWizard_Support_AOE::Ready_Effects(const _wstring& strLayerTag)
{
    _float3 vPos{};
    XMStoreFloat3(&vPos, m_pTransformCom->Get_State(STATE::POSITION));

    CEffect_Obj::DESC AOE1Desc{};
    AOE1Desc.eLevelID = m_eLevelID;
    AOE1Desc.strParticeFilePath = TEXT("../Bin/DataFiles/Effect/Wizard/Wizard_AOE1.Effect_Ex");
    AOE1Desc.strParticleTextureTag = TEXT("Prototype_Component_Texture_Twinkle");
    AOE1Desc.strParticleBufferTag = TEXT("Prototype_Component_VIBuffer_WizardAOE1");
    AOE1Desc.WorldMatrix = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);

     if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Effect_Obj"),
        ENUM_CLASS(m_eLevelID), strLayerTag, &AOE1Desc)))
        return E_FAIL;
     
    CEffect_Obj::DESC AOE2Desc{};
    AOE2Desc.eLevelID = m_eLevelID;
    AOE2Desc.strParticeFilePath = TEXT("../Bin/DataFiles/Effect/Wizard/Wizard_AOE2.Effect_Ex");
    AOE2Desc.strParticleTextureTag = TEXT("Prototype_Component_Texture_Twinkle");
    AOE2Desc.strParticleBufferTag = TEXT("Prototype_Component_VIBuffer_WizardAOE2");
    AOE2Desc.WorldMatrix = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);

     if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Effect_Obj"),
        ENUM_CLASS(m_eLevelID), strLayerTag, &AOE2Desc)))
        return E_FAIL;
     
    CEffect_Obj::DESC AOE3Desc{};
    AOE3Desc.eLevelID = m_eLevelID;
    AOE3Desc.strParticeFilePath = TEXT("../Bin/DataFiles/Effect/Wizard/Wizard_AOE3.Effect_Ex");
    AOE3Desc.strParticleTextureTag = TEXT("Prototype_Component_Texture_Twinkle");
    AOE3Desc.strParticleBufferTag = TEXT("Prototype_Component_VIBuffer_WizardAOE3");
    AOE3Desc.WorldMatrix = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);

     if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Effect_Obj"),
        ENUM_CLASS(m_eLevelID), strLayerTag, &AOE3Desc)))
        return E_FAIL;
     
    CEffect_Obj::DESC AOE4Desc{};
    AOE4Desc.eLevelID = m_eLevelID;
    AOE4Desc.strParticeFilePath = TEXT("../Bin/DataFiles/Effect/Wizard/Wizard_AOE4.Effect_Ex");
    AOE4Desc.strParticleTextureTag = TEXT("Prototype_Component_Texture_CrossCircle");
    AOE4Desc.strParticleBufferTag = TEXT("Prototype_Component_VIBuffer_WizardAOE4");
    AOE4Desc.WorldMatrix = XMMatrixRotationX(XMConvertToRadians(90.f)) * XMMatrixTranslation(vPos.x, vPos.y + 0.1f, vPos.z);

     if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Effect_Obj"),
        ENUM_CLASS(m_eLevelID), strLayerTag, &AOE4Desc)))
        return E_FAIL;



    return S_OK;
}

HRESULT CWizard_Support_AOE::Ready_Components(void* pArg)
{
    //if (FAILED(__super::Ready_Components(pArg))) // 나중에 추가 
    //    return E_FAIL;

    CBounding_AABB::DESC	AABBDesc{};
    AABBDesc.vExtents = _float3(3.5f, 0.2f, 3.5f);
    AABBDesc.vCenter = _float3(0.f, AABBDesc.vExtents.y, 0.f);
    AABBDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::MONSTER_BULLET);
    AABBDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::WIZARD_AOE);
    AABBDesc.pOwner = this;

    /* For.Com_Collider */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_AABB"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &AABBDesc)))
        return E_FAIL;

    return S_OK;
}

CWizard_Support_AOE* CWizard_Support_AOE::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWizard_Support_AOE* pInstance = new CWizard_Support_AOE(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWizard_Support_AOE");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWizard_Support_AOE::Clone(void* pArg)
{
	CWizard_Support_AOE* pInstance = new CWizard_Support_AOE(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWizard_Support_AOE");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWizard_Support_AOE::Free()
{
    __super::Free();
}
