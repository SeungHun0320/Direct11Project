#include "Monster_Bullet.h"
#include "GameInstance.h"

CMonster_Bullet::CMonster_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBullet{ pDevice, pContext }
{
}

CMonster_Bullet::CMonster_Bullet(const CMonster_Bullet& Prototype)
	: CBullet(Prototype)
{
}

HRESULT CMonster_Bullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster_Bullet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTargetTransform = dynamic_cast<CTransform*>(GET_PLAYER->Get_Component(TEXT("Com_Transform")));
	if (nullptr != m_pTargetTransform)
		Safe_AddRef(m_pTargetTransform);

	m_fAttack = 5.f;
	m_fStaggerValue = 10.f;

	return S_OK;
}

void CMonster_Bullet::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CMonster_Bullet::Update(_float fTimeDelta)
{
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	return __super::Update(fTimeDelta);
}

void CMonster_Bullet::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CMonster_Bullet::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif
	return  __super::Render();
}

HRESULT CMonster_Bullet::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CMonster_Bullet::Free()
{
	__super::Free();

	Safe_Release(m_pTargetTransform);
}
