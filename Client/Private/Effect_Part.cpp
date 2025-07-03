#include "Effect_Part.h"
#include "GameInstance.h"

CEffect_Part::CEffect_Part(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject{pDevice, pContext}
{
}

CEffect_Part::CEffect_Part(const CEffect_Part& Prototype)
	:CPartObject(Prototype)
{
}

HRESULT CEffect_Part::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Part::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_pParentLevelID = pDesc->pParentLevelID;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Desc(pDesc->strParticeFilePath)))
		return E_FAIL;

	return S_OK;
}

void CEffect_Part::Priority_Update(_float fTimeDelta)
{
}

LIFE CEffect_Part::Update(_float fTimeDelta)
{
	switch (m_eMoveType)
	{
	case EFFECT_MOVE::DROP:
		m_pVIBufferCom->Drop(fTimeDelta);
		break;
	case EFFECT_MOVE::SPREAD:
		m_pVIBufferCom->Spread(fTimeDelta);
		break;
	}

	_matrix ParentMatrix = XMLoadFloat4x4(m_pParentMatrix);

	ParentMatrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	ParentMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	ParentMatrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f);

	XMStoreFloat4x4(&m_CombinedWorldMatrix,
		XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Float4x4()) * ParentMatrix);

	return LIFE::NONE;
}

void CEffect_Part::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONLIGHT, this);
}

HRESULT CEffect_Part::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(m_ePass)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Part::Effect_Reset()
{
	m_pVIBufferCom->Reset();
}

HRESULT CEffect_Part::Ready_Components(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPointInstance"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(*m_pParentLevelID), pDesc->strParticleBufferTag,
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(*m_pParentLevelID), pDesc->strParticleTextureTag,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Part::Ready_Desc(const wstring& strParticleFilePath)
{
	ifstream LoadFile(strParticleFilePath, ios::binary);

	if (!LoadFile.is_open())
		return E_FAIL;

	LoadFile.read(reinterpret_cast<_char*>(&m_vColor), sizeof(_float4));
	LoadFile.read(reinterpret_cast<_char*>(&m_ePass), sizeof(EFFECT_PASS));
	LoadFile.read(reinterpret_cast<_char*>(&m_eMoveType), sizeof(EFFECT_MOVE));

	LoadFile.close();

	return S_OK;
}

HRESULT CEffect_Part::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fFar", m_pGameInstance->Get_Far_Ptr(), sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Depth"), "g_DepthTexture", m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CEffect_Part* CEffect_Part::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Part* pInstance = new CEffect_Part(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Part");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Part::Clone(void* pArg)
{
	CEffect_Part* pInstance = new CEffect_Part(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Part");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Part::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
