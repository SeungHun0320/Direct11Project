#include "UI.h"
#include "GameInstance.h"

CUI::CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject{pDevice, pContext}
{
}

CUI::CUI(const CUI& Prototype)
	:CPartObject(Prototype),
	m_eType {Prototype.m_eType}
{
}

HRESULT CUI::Initialize_Prototype(UI_TYPE eType)
{
	m_eType = eType;

	return S_OK;
}

HRESULT CUI::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_fX = pDesc->fX;
	m_fY = pDesc->fY;
	m_fSizeX = pDesc->fSizeX;
	m_fSizeY = pDesc->fSizeY;
	m_fOffset = pDesc->fOffset;
	m_eUIPass = pDesc->eUIPass;
	m_iTextureIndex = pDesc->iTextureIndex;

	m_pLevelID = pDesc->pParentLevelID;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;


	D3D11_VIEWPORT ViewportDesc{};
	_uint          iNumViewports = { 1 };

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	/* 직교투영용 행렬을 만들어준다 */
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));
	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY);
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_fX - (ViewportDesc.Width * 0.5f), -m_fY + (ViewportDesc.Height * 0.5f), 0.f, 1.f));

	return S_OK;
}

void CUI::Priority_Update(_float fTimeDelta)
{
	if (!m_isVisible)
		return;

	if (UI_TYPE::UI3D == m_eType)
	{
		/* 부모 행렬을 기준으로 스크린상의 좌표를 계산한다. */
		_matrix CombinedWorldMatrix = XMLoadFloat4x4(m_pParentMatrix);

		_matrix ViewProjMat = m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW) *
			m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ);

		_vector vParentPosition = CombinedWorldMatrix.r[3];

		vParentPosition = XMVectorSetY(vParentPosition, XMVectorGetY(vParentPosition) + m_fOffset);

		_float4 vParentPos;
		XMStoreFloat4(&vParentPos, XMVector3TransformCoord(vParentPosition, ViewProjMat));

		// 화면 중심 기준 직교 공간 좌표로 변환
		_float4 vOutScreenPos;
		vOutScreenPos.x = vParentPos.x * (g_iWinSizeX * 0.5f);
		vOutScreenPos.y = vParentPos.y * (g_iWinSizeY * 0.5f);
		vOutScreenPos.z = vParentPos.z;
		vOutScreenPos.w = vParentPos.w;

		m_pTransformCom->Set_State(STATE::POSITION, XMLoadFloat4(&vOutScreenPos));
		m_pTransformCom->Scaling(_float3(m_fSizeX, m_fSizeY, 1.f));
	}
}

LIFE CUI::Update(_float fTimeDelta)
{
	if (!m_isVisible)
		return LIFE::NONE;

	if (m_eType == UI_TYPE::UI3D)
	{
		XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix());
	}
	else
	{
		XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));
	}

	return LIFE::NONE;
}

void CUI::Late_Update(_float fTimeDelta)
{
	if(m_isVisible)
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CUI::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTextureIndex)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(m_eUIPass)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CUI::Set_PositionY(_float fY, _float fYOffset)
{
	m_fY = fY;

	D3D11_VIEWPORT ViewportDesc{};
	_uint iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	m_pTransformCom->Set_State(STATE::POSITION,
		XMVectorSet(m_fX - (ViewportDesc.Width * 0.5f), -m_fY + (ViewportDesc.Height * 0.5f) + fYOffset, 0.f, 1.f));
}

_vector CUI::Get_State(STATE eState)
{
	return m_pTransformCom->Get_State(eState);
}

_matrix CUI::Get_CombinedMatrix()
{
	return XMLoadFloat4x4(&m_CombinedWorldMatrix);
}

void CUI::Set_State(STATE eState, _fvector vState)
{
	m_pTransformCom->Set_State(eState, vState);
}


HRESULT CUI::Ready_Components(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(*m_pLevelID), pDesc->strPrototypeTag,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI::Bind_ShaderResources()
{
	/* 셰이더에게 본인의 월드행렬을 넘겨줌 */
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;


	return S_OK;
}

CUI* CUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_TYPE eType)
{
	CUI* pInstance = new CUI(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType)))
	{
		MSG_BOX("Failed to Created : CUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI::Clone(void* pArg)
{
	CUI* pInstance = new CUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI::Free()
{
	__super::Free();
		
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
