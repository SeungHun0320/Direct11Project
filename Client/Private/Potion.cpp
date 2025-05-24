#include "Potion.h"
#include "GameInstance.h"

CPotion::CPotion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CItem{ pDevice, pContext }
{
}

CPotion::CPotion(const CPotion& Prototype)
	: CItem(Prototype)
{
}

HRESULT CPotion::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPotion::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CPotion::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CPotion::Update(_float fTimeDelta)
{
	if (m_bDead)
		return LIFE::DEAD;

	return __super::Update(fTimeDelta);
}

void CPotion::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CPotion::Render()
{
	return __super::Render();
}

HRESULT CPotion::Ready_Components(void* pArg)
{
	if (__super::Ready_Components(pArg))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(m_eLevelID), TEXT("Prototype_Component_Model_Potion"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

CPotion* CPotion::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPotion* pInstance = new CPotion(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPotion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPotion::Clone(void* pArg)
{
	CPotion* pInstance = new CPotion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPotion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPotion::Free()
{
	__super::Free();
}
