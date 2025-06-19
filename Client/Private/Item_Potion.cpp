#include "Item_Potion.h"
#include "GameInstance.h"

CItem_Potion::CItem_Potion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CItem{ pDevice, pContext }
{
}

CItem_Potion::CItem_Potion(const CItem_Potion& Prototype)
	: CItem(Prototype)
{
}

HRESULT CItem_Potion::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItem_Potion::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CItem_Potion::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CItem_Potion::Update(_float fTimeDelta)
{
	if (m_bDead)
		return LIFE::DEAD;

	return __super::Update(fTimeDelta);
}

void CItem_Potion::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CItem_Potion::Render()
{
	return __super::Render();
}

HRESULT CItem_Potion::Ready_Components(void* pArg)
{
	if (__super::Ready_Components(pArg))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_Potion"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

CItem_Potion* CItem_Potion::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CItem_Potion* pInstance = new CItem_Potion(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CItem_Potion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CItem_Potion::Clone(void* pArg)
{
	CItem_Potion* pInstance = new CItem_Potion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CItem_Potion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem_Potion::Free()
{
	__super::Free();
}
