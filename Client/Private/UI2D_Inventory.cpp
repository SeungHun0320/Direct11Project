#include "UI2D_Inventory.h"
#include "GameInstance.h"

#include "UI.h"

CUI2D_Inventory::CUI2D_Inventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIContainerPart{ pDevice, pContext }
{
}

CUI2D_Inventory::CUI2D_Inventory(const CUI2D_Inventory& Prototype)
	: CUIContainerPart(Prototype)
{
}

HRESULT CUI2D_Inventory::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI2D_Inventory::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(445, 40.f, 0.f, 1.f));

	return S_OK;
}

void CUI2D_Inventory::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CUI2D_Inventory::Update(_float fTimeDelta)
{
	return 	__super::Update(fTimeDelta);
}

void CUI2D_Inventory::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CUI2D_Inventory::Render()
{
	return __super::Render();
}

void CUI2D_Inventory::Set_UIVisible(_uint iPart, _bool isVisible)
{
	static_cast<CUI*>(m_PartObjects[iPart])->Set_Visible(isVisible);
}

void CUI2D_Inventory::Set_TextureIndex(_uint iPart, _uint iTextureIdx)
{
	static_cast<CUI*>(m_PartObjects[iPart])->Set_TextureIndex(iTextureIdx);
}

HRESULT CUI2D_Inventory::Ready_Components(void* pArg)
{
	return S_OK;
}

HRESULT CUI2D_Inventory::Ready_PartObjects()
{

	return S_OK;
}

CUI2D_Inventory* CUI2D_Inventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI2D_Inventory* pInstance = new CUI2D_Inventory(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI2D_Inventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI2D_Inventory::Clone(void* pArg)
{
	CUI2D_Inventory* pInstance = new CUI2D_Inventory(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI2D_Inventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI2D_Inventory::Free()
{
	__super::Free();
}
