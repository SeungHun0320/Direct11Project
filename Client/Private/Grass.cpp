#include "Grass.h"
#include "GameInstance.h"

#include "Body_Grass.h"

CGrass::CGrass(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEnvironment_Object{ pDevice, pContext }
{
}

CGrass::CGrass(const CGrass& Prototype)
	: CEnvironment_Object(Prototype)
{
}

HRESULT CGrass::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGrass::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CGrass::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CGrass::Update(_float fTimeDelta)
{
	if (m_bDead)
		return LIFE::DEAD;

	
	return __super::Update(fTimeDelta);
}

void CGrass::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CGrass::Render()
{
	return __super::Render();
}

void CGrass::On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner)
{
	cout << "그래스 개같이 성공\n";
}

HRESULT CGrass::Ready_Components(void* pArg)
{
	if (__super::Ready_Components(pArg))
		return E_FAIL;

	return S_OK;
}

HRESULT CGrass::Ready_PartObjects()
{
	CBody_Grass::DESC BodyDesc{};

	BodyDesc.eLevelID = m_eLevelID;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	BodyDesc.pOwner = this;

	if (FAILED(__super::Add_PartObject(PART_BODY, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_Body_Grass"), &BodyDesc)))
		return E_FAIL;

	return S_OK;
}

CGrass* CGrass::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGrass* pInstance = new CGrass(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGrass");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGrass::Clone(void* pArg)
{
	CGrass* pInstance = new CGrass(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGrass");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGrass::Free()
{
	__super::Free();
}
