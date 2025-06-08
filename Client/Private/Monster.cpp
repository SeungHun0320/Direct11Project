#include "Monster.h"

#include "GameInstance.h"


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

void CMonster::On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner)
{
	if (CI_MONSTER(static_cast<COLLIDER_ID>(OtherColliderID)))
	{
		// 밀어낸다.
	}

	switch (static_cast<COLLIDER_ID>(OtherColliderID))
	{	
	case COLLIDER_ID::BUSH:
		// 밀어낸다
		break;
	}
}

HRESULT CMonster::Ready_Components(void* pArg)
{
	return S_OK;
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pTargetTransform);
}
