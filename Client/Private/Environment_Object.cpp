#include "Environment_Object.h"

#include "GameInstance.h"
#include "PartObject.h"

CEnvironment_Object::CEnvironment_Object(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CContainerObject{ pDevice, pContext }
{
}

CEnvironment_Object::CEnvironment_Object(const CEnvironment_Object& Prototype)
	: CContainerObject(Prototype)
{
}

HRESULT CEnvironment_Object::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnvironment_Object::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_eLevelID = pDesc->eLevelID;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}

void CEnvironment_Object::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CEnvironment_Object::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CEnvironment_Object::Late_Update(_float fTimeDelta)
{
	if (!m_pGameInstance->isIn_WorldSpace(m_pTransformCom->Get_State(STATE::POSITION), 5.f))
		return;

	__super::Late_Update(fTimeDelta);

	m_isCollision = false;
}

HRESULT CEnvironment_Object::Render()
{
	return S_OK;
}

void CEnvironment_Object::On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner)
{
	COLLIDER_ID eColliderID = static_cast<COLLIDER_ID>(OtherColliderID);

	if (CI_PLAYER(eColliderID))
		m_isCollision = true;

}

HRESULT CEnvironment_Object::Ready_Components(void* pArg)
{
	return S_OK;
}

void CEnvironment_Object::Free()
{
	__super::Free();
}