#include "Monster.h"

#include "GameInstance.h"
#include "PartObject.h"

#include "Effect_Hit.h"
#include "Effect_Mesh_Smoke.h"

#include "Player.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBaseActor{ pDevice, pContext }
{

}

CMonster::CMonster(const CMonster& Prototype)
	: CBaseActor( Prototype )
{

}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_eLevelID = pDesc->eLevelID;

	if (m_eLevelID != LEVEL::TOOLS)
	{
		m_pTargetTransform = dynamic_cast<CTransform*>(GET_PLAYER->Get_Component(TEXT("Com_Transform")));
		if (nullptr != m_pTargetTransform)
			Safe_AddRef(m_pTargetTransform);
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CMonster::Update(_float fTimeDelta)
{
	if (m_bDead)
		return LIFE::DEAD;

	if (nullptr != m_pTargetTransform)
	{
		m_fDistanceToPlayer = XMVectorGetX(XMVector3Length(m_pTargetTransform->Get_State(STATE::POSITION) - m_pTransformCom->Get_State(STATE::POSITION)));
	}

	return __super::Update(fTimeDelta);
}

void CMonster::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CMonster::Render()
{
	return S_OK;
}

_bool CMonster::Find_Player()
{
	if(m_fDetectDistance >= m_fDistanceToPlayer)
		return true;

	return false;
}

const _vector CMonster::Get_TargetPosition() const
{
	if(nullptr != m_pTargetTransform)
		return m_pTargetTransform->Get_State(STATE::POSITION);

	return XMVectorSet(FLT_MAX, FLT_MAX, FLT_MAX, 1.f);
}

CCollider* CMonster::Get_Collider(_uint iColliderIndex)
{
	/* 무조건 첫번째에는 바디가 들어간다는 전제하에 쓰는 코드,, 아 진짜 에바네 이거 진짜 에바네 진짜 에바네*/
	return m_PartObjects[0]->Get_Collider(iColliderIndex);
}

void CMonster::On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner)
{
	COLLIDER_ID eColliderID = static_cast<COLLIDER_ID>(OtherColliderID);

	if (CI_WEAPON(eColliderID))
		m_pGameInstance->Shake_Camera(ENUM_CLASS(m_eLevelID), TEXT("Camera_TPS"), 0.4f, 0.2f);

	if (CI_MONSTER(eColliderID))
	{
		CCollider* pCollider = Get_Collider();
		if (nullptr == pCollider)
			return;

		m_pTransformCom->Apply_Sliding(pCollider->Get_SlidingVector());
		m_isBlocked = true;
	}

	switch (eColliderID)
	{	
	case COLLIDER_ID::BUSH:
		CCollider* pCollider = Get_Collider();
		if (nullptr == pCollider)
			return;

		m_pTransformCom->Apply_Sliding(pCollider->Get_SlidingVector());
		m_isBlocked = true;
		break;
	}
}

HRESULT CMonster::Create_HitEffect()
{
	CEffect_Hit::DESC tDesc{};

	tDesc.iNumPartObjects = CEffect_Hit::PART_END;
	tDesc.pParentLevelID = &m_eLevelID;
	tDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	tDesc.strName = TEXT("Effect_Hit");

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_") + tDesc.strName,
		ENUM_CLASS(m_eLevelID), TEXT("Layer_Effect"), &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Craete_SmokeEffect()
{
	CEffect_Mesh_Smoke::DESC tDesc{};
	_float3 vPos{};

	XMStoreFloat3(&vPos, m_pTransformCom->Get_State(STATE::POSITION));

	tDesc.eLevelID = m_eLevelID;
	tDesc.WorldMatrix = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
	tDesc.strEffectModelTag = TEXT("Prototype_Component_Model_Particle_Instance_Smoke");
	tDesc.strName = TEXT("Effect_Mesh_Smoke");

	if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_") + tDesc.strName,
		ENUM_CLASS(m_eLevelID), TEXT("Layer_Effect"), &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Ready_Components(void* pArg)
{
	if (__super::Ready_Components(pArg))
		return E_FAIL;

	return S_OK;
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pTargetTransform);
}
