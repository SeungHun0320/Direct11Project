#include "GlobalShadow.h"
#include "GameInstance.h"

CGlobalShadow::CGlobalShadow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject {pDevice, pContext}
{
}

CGlobalShadow::CGlobalShadow(const CGlobalShadow& Prototype)
	: CGameObject (Prototype)
{
}

HRESULT CGlobalShadow::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGlobalShadow::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_eLevelID = pDesc->eLevelID;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* 화면에 꽉 채워서 그려야 하기때문에 뷰포트사이즈를 통해 크기를 맞춰줌 */
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(static_cast<_float>(g_iWinSizeX), static_cast<_float>(g_iWinSizeY), 1.f));
	/* 직교 투영용 행렬을 만들어줌 */
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(static_cast<_float>(g_iWinSizeX), static_cast<_float>(g_iWinSizeY), 0.0f, 1.f));

	return S_OK;
}

void CGlobalShadow::Priority_Update(_float fTimeDelta)
{
}

LIFE CGlobalShadow::Update(_float fTimeDelta)
{
	return LIFE::NONE;
}

void CGlobalShadow::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_PRIORITY_SHADOW, this);
}

HRESULT CGlobalShadow::Render_Shadow()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_GlobalShadow", 0)))
		return E_FAIL;

	m_pShaderCom->Begin(7);

	m_pVIBufferCom->Bind_Buffers();
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CGlobalShadow::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Shadow"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

    return S_OK;
}

CGlobalShadow* CGlobalShadow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGlobalShadow* pInstance = new CGlobalShadow(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGlobalShadow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGlobalShadow::Clone(void* pArg)
{
	CGlobalShadow* pInstance = new CGlobalShadow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGlobalShadow");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CGlobalShadow::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);

}
