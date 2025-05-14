#include "Courtyard.h"
#include "GameInstance.h"

CCourtyard::CCourtyard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMap{pDevice, pContext}
{
}

CCourtyard::CCourtyard(const CCourtyard& Prototype)
	: CMap(Prototype)
{
}

HRESULT CCourtyard::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCourtyard::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CCourtyard::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CCourtyard::Update(_float fTimeDelta)
{
	if (m_bDead)
		return LIFE::DEAD;

	return __super::Update(fTimeDelta);
}

void CCourtyard::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CCourtyard::Render()
{
	return __super::Render();
}

HRESULT CCourtyard::Ready_Components(void* pArg)
{
	if (__super::Ready_Components(pArg))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototpye_Component_Model_Courtyard"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

CCourtyard* CCourtyard::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCourtyard* pInstance = new CCourtyard(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCourtyard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCourtyard::Clone(void* pArg)
{
	CCourtyard* pInstance = new CCourtyard(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCourtyard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCourtyard::Free()
{
	__super::Free();
}
