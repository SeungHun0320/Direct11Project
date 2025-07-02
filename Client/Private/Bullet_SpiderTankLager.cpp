#include "Bullet_SpiderTankLager.h"

#include "GameInstance.h"

CBullet_SpiderTankLager::CBullet_SpiderTankLager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CBullet_Monster{pDevice, pContext}
{
}

CBullet_SpiderTankLager::CBullet_SpiderTankLager(const CBullet_SpiderTankLager& Prototype)
    :CBullet_Monster(Prototype)
{
}

HRESULT CBullet_SpiderTankLager::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBullet_SpiderTankLager::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

	DESC* pDesc = static_cast<DESC*>(pArg);

	m_pParentMatrix = pDesc->pParentMatrix;

	m_pTransformCom->Set_Matrix(XMLoadFloat4x4(m_pParentMatrix));

	m_fDeadTime = 1.f;
	m_fAttack = 30.f;
	m_fStaggerValue = 50.f;

    return S_OK;
}

void CBullet_SpiderTankLager::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CBullet_SpiderTankLager::Update(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_bDead)
		return LIFE::DEAD;

	if (m_fDeadTime <= m_fTimeAcc)
		Set_Dead(true);

	m_pColliderCom->Update(XMLoadFloat4x4(m_pParentMatrix));

    return LIFE::NONE;
}

void CBullet_SpiderTankLager::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CBullet_SpiderTankLager::Render()
{
    return S_OK;
}

HRESULT CBullet_SpiderTankLager::Ready_Components(void* pArg)
{
    //if (FAILED(__super::Ready_Components(pArg))) // 나중에 추가
    //    return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_OBB::DESC	OBBDesc{};

	OBBDesc.vExtents = _float3(2.f, 1.f, 20.f);
	OBBDesc.vCenter = _float3(0.f, OBBDesc.vExtents.y, OBBDesc.vExtents.z);
	OBBDesc.vRotation = _float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	OBBDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::MONSTER_BULLET);
	OBBDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::SPIDERTANK_BOMB);
	OBBDesc.pOwner = this;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &OBBDesc)))
		return E_FAIL;

    return S_OK;
}

CBullet_SpiderTankLager* CBullet_SpiderTankLager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBullet_SpiderTankLager* pInstance = new CBullet_SpiderTankLager(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBullet_SpiderTankLager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBullet_SpiderTankLager::Clone(void* pArg)
{
	CBullet_SpiderTankLager* pInstance = new CBullet_SpiderTankLager(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBullet_SpiderTankLager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBullet_SpiderTankLager::Free()
{
    __super::Free();
}
