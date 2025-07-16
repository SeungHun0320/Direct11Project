#include "Bullet_SpiderTank.h"
#include "GameInstance.h"

#include "Effect_Part.h"
#include "Effect_Obj.h"

CBullet_SpiderTank::CBullet_SpiderTank(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBullet_Monster{ pDevice, pContext }
{
}

CBullet_SpiderTank::CBullet_SpiderTank(const CBullet_SpiderTank& Prototype)
	: CBullet_Monster(Prototype)
{
}

HRESULT CBullet_SpiderTank::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet_SpiderTank::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_vDir = pDesc->vDir;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fDeadTime = 5.f;
	m_fAttack = 7.5f;
	m_fStaggerValue = 0.f;

	m_pTransformCom->Scaling(0.3f, 0.3f, 0.3f);
	m_pTransformCom->LookAt(XMVectorSetW(XMLoadFloat3(&m_vDir), 1.f));

	if (FAILED(Create_Trail()))
		return E_FAIL;

	return S_OK;
}

void CBullet_SpiderTank::Priority_Update(_float fTimeDelta)
{
	m_pPartObject->Priority_Update(fTimeDelta);

	__super::Priority_Update(fTimeDelta);
}

LIFE CBullet_SpiderTank::Update(_float fTimeDelta)
{
	m_pPartObject->Update(fTimeDelta);

	m_pTransformCom->Go_Straight(fTimeDelta);

	_float fCurY = XMVectorGetY(m_pTransformCom->Get_State(STATE::POSITION));
	_float vGroundY = XMVectorGetY(m_pNavigationCom->SetUp_Height(m_pTransformCom->Get_State(STATE::POSITION)));

	if (vGroundY >= fCurY)
	{
		Create_Effect();
		Set_Dead(true);
	}
	

	return __super::Update(fTimeDelta);
}

void CBullet_SpiderTank::Late_Update(_float fTimeDelta)
{
	m_pPartObject->Late_Update(fTimeDelta);
	//__super::Late_Update(fTimeDelta);
}

HRESULT CBullet_SpiderTank::Render()
{
	return S_OK;
} 

HRESULT CBullet_SpiderTank::Create_Trail()
{
	CEffect_Part::DESC TrailDesc{};

	TrailDesc.eOrientation = CEffect_Part::LOCAL;
	TrailDesc.pParentLevelID = &m_eLevelID;
	TrailDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	TrailDesc.strParticeFilePath = TEXT("../Bin/DataFiles/Effect/SpiderTank/BossBulletTrail.Effect_Ex");
	TrailDesc.strParticleBufferTag = TEXT("Prototype_Component_VIBuffer_BossBulletTrail");
	TrailDesc.strParticleTextureTag = TEXT("Prototype_Component_Texture_HexParticle");
	TrailDesc.strName = TEXT("Effect_Part");

	if (FAILED(Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_") + TrailDesc.strName, &TrailDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBullet_SpiderTank::Create_Effect()
{
	CEffect_Obj::DESC Dead0Desc{};

	Dead0Desc.eLevelID = m_eLevelID;
	Dead0Desc.strParticeFilePath = TEXT("../Bin/DataFiles/Effect/SpiderTank/BossBulletDead0.Effect_Ex");
	Dead0Desc.strParticleBufferTag = TEXT("Prototype_Component_VIBuffer_BossBulletDead0");
	Dead0Desc.strParticleTextureTag = TEXT("Prototype_Component_Texture_SplashEffect");
	Dead0Desc.strName = TEXT("Effect_Obj");

	Dead0Desc.WorldMatrix = m_pTransformCom->Get_WorldMatrix();

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_") + Dead0Desc.strName,
		ENUM_CLASS(Dead0Desc.eLevelID), TEXT("Layer_Effect"), &Dead0Desc)))
		return E_FAIL;

	CEffect_Obj::DESC Dead1Desc{};
	_float3 vPos{};
	XMStoreFloat3(&vPos, m_pTransformCom->Get_State(STATE::POSITION));

	Dead1Desc.eLevelID = m_eLevelID;
	Dead1Desc.strParticeFilePath = TEXT("../Bin/DataFiles/Effect/SpiderTank/BossBulletDead1.Effect_Ex");
	Dead1Desc.strParticleBufferTag = TEXT("Prototype_Component_VIBuffer_BossBulletDead1");
	Dead1Desc.strParticleTextureTag = TEXT("Prototype_Component_Texture_VoidParticle");
	Dead1Desc.strName = TEXT("Effect_Obj");

	Dead1Desc.WorldMatrix = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_") + Dead1Desc.strName,
		ENUM_CLASS(Dead1Desc.eLevelID), TEXT("Layer_Effect"), &Dead1Desc)))
		return E_FAIL;

	return S_OK;
}

void CBullet_SpiderTank::On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner)
{
	COLLIDER_ID eColliderID = static_cast<COLLIDER_ID>(OtherColliderID);

	switch (eColliderID)
	{
	case COLLIDER_ID::PLAYER:
		Set_Dead(true);
		break;
	}

}

HRESULT CBullet_SpiderTank::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_Sphere::DESC	ColDesc{};
	_float3 vScale = m_pTransformCom->Get_Scaled();

	ColDesc.vCenter = _float3(0.f, 0.f, 0.f);
	ColDesc.fRadius = vScale.x + 2.f;
	ColDesc.pOwner = this;
	ColDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::MONSTER_BULLET);
	ColDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::SPIDERTANK_BULLET);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColDesc)))
		return E_FAIL;

	CNavigation::DESC tDesc{};
	XMStoreFloat3(&tDesc.vInitWorldPos, m_pTransformCom->Get_State(STATE::POSITION));

	if (LEVEL::TOOLS != m_eLevelID)
	{
		if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Navigation"),
			TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &tDesc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CBullet_SpiderTank::Add_PartObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	m_pPartObject = dynamic_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == m_pPartObject)
		return E_FAIL;

	return S_OK;
}

CBullet_SpiderTank* CBullet_SpiderTank::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBullet_SpiderTank* pInstance = new CBullet_SpiderTank(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBullet_SpiderTank");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBullet_SpiderTank::Clone(void* pArg)
{
	CBullet_SpiderTank* pInstance = new CBullet_SpiderTank(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBullet_SpiderTank");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBullet_SpiderTank::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pPartObject);
}
