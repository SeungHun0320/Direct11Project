#include "Bridge.h"
#include "GameInstance.h"

#include "Body_Bridge.h"

CBridge::CBridge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEnvironment_Object{pDevice, pContext}
{
}

CBridge::CBridge(const CBridge& Prototype)
	: CEnvironment_Object(Prototype)
{
}

HRESULT CBridge::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBridge::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBridge::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CBridge::Update(_float fTimeDelta)
{
	if (m_bDead)
		return LIFE::DEAD;

	return 	__super::Update(fTimeDelta);
}

void CBridge::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBridge::Render()
{
	return S_OK;
}

HRESULT CBridge::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBridge::Ready_PartObjects()
{
	CBody_Bridge::DESC BodyDesc{};

	BodyDesc.eLevelID = m_eLevelID;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	BodyDesc.pOwner = this;

	if (FAILED(__super::Add_PartObject(PART_BODY, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_Body_Bridge"), &BodyDesc)))
		return E_FAIL;

	return S_OK;
}

CBridge* CBridge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBridge* pInstance = new CBridge(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBridge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBridge::Clone(void* pArg)
{
	CBridge* pInstance = new CBridge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBridge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBridge::Free()
{
	__super::Free();
}
