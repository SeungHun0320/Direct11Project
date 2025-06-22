#include "Bullet_Monster.h"
#include "GameInstance.h"

CBullet_Monster::CBullet_Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBullet{ pDevice, pContext }
{
}

CBullet_Monster::CBullet_Monster(const CBullet_Monster& Prototype)
	: CBullet(Prototype)
{
}

HRESULT CBullet_Monster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet_Monster::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTargetTransform = dynamic_cast<CTransform*>(GET_PLAYER->Get_Component(TEXT("Com_Transform")));
	if (nullptr != m_pTargetTransform)
		Safe_AddRef(m_pTargetTransform);


	/* 여기 나중에 수정하쇼,, */
	m_fAttack = 5.f;
	m_fStaggerValue = 10.f;

	return S_OK;
}

void CBullet_Monster::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CBullet_Monster::Update(_float fTimeDelta)
{
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

	return __super::Update(fTimeDelta);
}

void CBullet_Monster::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBullet_Monster::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif
	return  __super::Render();
}

HRESULT CBullet_Monster::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBullet_Monster::Free()
{
	__super::Free();

	Safe_Release(m_pTargetTransform);
}
