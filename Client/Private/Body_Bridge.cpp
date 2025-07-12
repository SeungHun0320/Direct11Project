#include "Body_Bridge.h"
#include "GameInstance.h"

CBody_Bridge::CBody_Bridge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBody_Environment_Object{pDevice, pContext}
{
}

CBody_Bridge::CBody_Bridge(const CBody_Bridge& Prototype)
	: CBody_Environment_Object(Prototype)
{
}

HRESULT CBody_Bridge::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBody_Bridge::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBody_Bridge::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CBody_Bridge::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CBody_Bridge::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBody_Bridge::Render()
{
	return __super::Render();
}

HRESULT CBody_Bridge::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	DESC* pDesc = static_cast<DESC*>(pArg);

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_Bridge"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

CBody_Bridge* CBody_Bridge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBody_Bridge* pInstance = new CBody_Bridge(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBody_Bridge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBody_Bridge::Clone(void* pArg)
{
	CBody_Bridge* pInstance = new CBody_Bridge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBody_Bridge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBody_Bridge::Free()
{
	__super::Free();
}
