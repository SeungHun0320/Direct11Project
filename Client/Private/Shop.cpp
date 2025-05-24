#include "Shop.h"

CShop::CShop(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMap{ pDevice, pContext }
{
}

CShop::CShop(const CShop& Prototype)
	: CMap(Prototype)
{
}

HRESULT CShop::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CShop::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CShop::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CShop::Update(_float fTimeDelta)
{
	if (m_bDead)
		return LIFE::DEAD;

	return __super::Update(fTimeDelta);
}

void CShop::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CShop::Render()
{
	return  __super::Render();
}

HRESULT CShop::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_Shop"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

CShop* CShop::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CShop* pInstance = new CShop(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CShop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CShop::Clone(void* pArg)
{
	CShop* pInstance = new CShop(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CShop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShop::Free()
{
	__super::Free();
}
