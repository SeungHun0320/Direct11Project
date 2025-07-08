#include "Particle_Tool.h"
#include "GameInstance.h"

CParticle_Tool::CParticle_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{ pDevice, pContext }
{
}

CParticle_Tool::CParticle_Tool(const CParticle_Tool& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CParticle_Tool::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticle_Tool::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CParticle_Tool::Priority_Update(_float fTimeDelta)
{
}

LIFE CParticle_Tool::Update(_float fTimeDelta)
{
	switch (m_eType)
	{
	case EFFECT_MOVE::DROP:
		m_pVIBufferCom->Drop(fTimeDelta);
		break;
	case EFFECT_MOVE::SPREAD:
		m_pVIBufferCom->Spread(fTimeDelta);
		break;
	case EFFECT_MOVE::CHASE:
		m_pVIBufferCom->MoveTrail(XMVectorSet(2.5f, 0.f, 0.f, 1.f), fTimeDelta);
	}

	return LIFE::NONE;
}

void CParticle_Tool::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONLIGHT, this);
}

HRESULT CParticle_Tool::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TEXTURE]->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pTextureCom[MASK]->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pTextureCom[NOISE]->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(m_ePass)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticle_Tool::Change_TextureCom(const _wstring& strTextureTag)
{
	Safe_Release(m_pTextureCom[TEXTURE]);

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::TOOLS), strTextureTag,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE]))))
		return E_FAIL;

	return E_NOTIMPL;
}

HRESULT CParticle_Tool::Ready_Components(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPointInstance"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_Component_VIBuffer_Tool"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_Component_Texture_SpikeParticle"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE]))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_Component_Texture_SteamMaskEffect"),
		TEXT("Com_TextureMask"), reinterpret_cast<CComponent**>(&m_pTextureCom[MASK]))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::TOOLS), TEXT("Prototype_Component_Texture_SteamNoiseEffect"),
		TEXT("Com_TextureNoise"), reinterpret_cast<CComponent**>(&m_pTextureCom[NOISE]))))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticle_Tool::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
		return E_FAIL;

	if(FAILED(m_pShaderCom->Bind_RawValue("g_fFar", m_pGameInstance->Get_Far_Ptr(), sizeof(_float))))
		return E_FAIL;
	
	if(FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Depth"), "g_DepthTexture", m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CParticle_Tool* CParticle_Tool::Craete(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParticle_Tool* pInstance = new CParticle_Tool(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticle_Tool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CParticle_Tool::Clone(void* pArg)
{
	CParticle_Tool* pInstance = new CParticle_Tool(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CParticle_Tool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticle_Tool::Free()
{
	__super::Free();

	for (_uint i = 0; i < TEX_END; ++i)
		Safe_Release(m_pTextureCom[i]);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
