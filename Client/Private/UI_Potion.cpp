#include "UI_Potion.h"

CUI_Potion::CUI_Potion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI{pDevice, pContext}
{
}

CUI_Potion::CUI_Potion(const CUI_Potion& Prototype)
    :CUI(Prototype)
{
}

HRESULT CUI_Potion::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Potion::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CUI_Potion::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CUI_Potion::Update(_float fTimeDelta)
{
    return  __super::Update(fTimeDelta);
}

void CUI_Potion::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CUI_Potion::Render()
{
    return __super::Render();
}

HRESULT CUI_Potion::Ready_Components(void* pArg)
{
    if (FAILED(__super::Ready_Components(pArg)))
        return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Potion"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

    return S_OK;
}

CUI_Potion* CUI_Potion::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Potion* pInstance = new CUI_Potion(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Potion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Potion::Clone(void* pArg)
{
	CUI_Potion* pInstance = new CUI_Potion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Potion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Potion::Free()
{
	__super::Free();
}
