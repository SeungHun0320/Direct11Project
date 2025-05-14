#include "Boss.h"

#include "GameInstance.h"

CBoss::CBoss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster{ pDevice, pContext }
{

}

CBoss::CBoss(const CBoss& Prototype)
	: CMonster(Prototype)
{

}

HRESULT CBoss::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoss::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBoss::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CBoss::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CBoss::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBoss::Render()
{
	return __super::Render();
}

HRESULT CBoss::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBoss::Free()
{
	__super::Free();

}
