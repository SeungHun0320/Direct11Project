#include "Renderer.h"
#include "GameObject.h"
#include "GameInstance.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance {CGameInstance::Get_Instance()}
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CRenderer::Initialize()
{
	/* 렌더러를 생성할때 미리 렌더타겟들을 생성해놓는다 */
	_uint			iNumViewPorts = { 1 };
	D3D11_VIEWPORT	ViewPortDesc{};

	m_pContext->RSGetViewports(&iNumViewPorts, &ViewPortDesc);

	/* 32비트로 표현 8 8 8 8 */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Diffuse"),
		static_cast<_uint>(ViewPortDesc.Width), static_cast<_uint>(ViewPortDesc.Height), DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	/* DXGI_FORMAT_R16G16B16A16_UNORM :  0 ~ 65535의 값을 0.f ~ 1.f로 바꿔서 사용함 나중에 노말 계산할 때 잘 알아두기 */
	/* 노말은 -1.f ~ 1.f까지인데, UNORM으로 받아왔기 때문에 -1.f이 자동으로 0.f로 바뀌어서 저장되기 때문에 셰이더에서 0.f ~ 1.f로 치환한뒤에 Out에 저장해줘야 함*/
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Normal"),
		static_cast<_uint>(ViewPortDesc.Width), static_cast<_uint>(ViewPortDesc.Height), DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Shade"),
		static_cast<_uint>(ViewPortDesc.Width), static_cast<_uint>(ViewPortDesc.Height), DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.0f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	/* 깊이를 계산해서 렌더타겟에 저장한다 원래 16으로 받아왔지만, 0~1 사이의 값을 완벽하게 채울 수 없기 때문에 어쩔 수 없이 32로 포맷을 바꿔줌 */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Depth"),
		static_cast<_uint>(ViewPortDesc.Width), static_cast<_uint>(ViewPortDesc.Height), DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.0f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* 스펙큘러는 합연산이기때문에 백버퍼를 검정색으로 그려준다 */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Specular"),
		static_cast<_uint>(ViewPortDesc.Width), static_cast<_uint>(ViewPortDesc.Height), DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Depth"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Lights"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Lights"), TEXT("Target_Specular"))))
		return E_FAIL;

	if (FAILED(Ready_Resources()))
		return E_FAIL;

	/* 화면에 꽉 채워서 그려야 하기때문에 뷰포트사이즈를 통해 크기를 맞춰줌 */
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(ViewPortDesc.Width, ViewPortDesc.Height, 1.f));
	/* 직교 투영용 행렬을 만들어줌 */
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewPortDesc.Width, ViewPortDesc.Height, 0.0f, 1.f));

#ifdef _DEBUG
	_float fRTWidth = ViewPortDesc.Width * 0.2f;
	_float fRTHeight = ViewPortDesc.Height * 0.2f;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Diffuse"), fRTWidth * 0.5f, fRTHeight * 0.5f, fRTWidth, fRTHeight)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Normal"), fRTWidth * 0.5f, fRTHeight * 1.5f, fRTWidth, fRTHeight)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Depth"), fRTWidth * 0.5f, fRTHeight * 2.5f, fRTWidth, fRTHeight)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Shade"), fRTWidth * 1.5f, fRTHeight * 0.5f, fRTWidth, fRTHeight)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Specular"), fRTWidth * 1.5f, fRTHeight * 1.5f, fRTWidth, fRTHeight)))
		return E_FAIL;

#endif

	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject* pRenderObject)
{
	if (eRenderGroup >=	RENDERGROUP::RG_END ||
		eRenderGroup < RENDERGROUP::RG_PRIORITY ||
		nullptr == pRenderObject)
		return E_FAIL;

	m_RenderObjects[ENUM_CLASS(eRenderGroup)].push_back(pRenderObject);

	Safe_AddRef(pRenderObject);

	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderer::Add_DebugComponent(CComponent* pDebugCom)
{
	if (nullptr == pDebugCom)
		return E_FAIL;

	m_DebugComponents.push_back(pDebugCom);

	Safe_AddRef(pDebugCom);

	return S_OK;
}
#endif

HRESULT CRenderer::Draw()
{
	if (FAILED(Render_Priority()))
		return E_FAIL;
	if (FAILED(Render_NonBlend()))
		return E_FAIL;
	
	/* 빛연산은 객체들의 노말을 갖고와서 해야하기 때문에, 
	   객체들이 다 그려지고 나서 연산을 한다 */
	if (FAILED(Render_Lights()))
		return E_FAIL;

	/* 당연히 백퍼버는 객체들의 디퓨즈 + 빛 연산후의 셰이드 값이니까 여기에 그려주는게 맞음 */
	if (FAILED(Render_BackBuffer()))
		return E_FAIL;

	/* 조명 연산이 안먹히는 친구들을 여기에 넣어줌 like파티클(메시파티클은 아니야!!!!!!!!) */
	if (FAILED(Render_NonLight()))
		return E_FAIL;

	if (FAILED(Render_Blend()))
		return E_FAIL;
	if (FAILED(Render_UI()))
		return E_FAIL;
	
#ifdef _DEBUG
	if (FAILED(Render_Debug()))
		return E_FAIL;
#endif

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pGameObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_PRIORITY)])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_PRIORITY)].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NonBlend()
{
	/* Diffuse + Normal */
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_GameObjects"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_NONBLEND)])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_NONBLEND)].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

//
//_bool Compare(CGameObject* pSour, CGameObject* pDest)
//{
//	return dynamic_cast<CBlendObject*>(pSour)->Get_Depth() > dynamic_cast<CBlendObject*>(pDest)->Get_Depth();
//}

HRESULT CRenderer::Render_Blend()
{
	/*m_RenderObjects[RG_BLEND].sort([](CGameObject* pSour, CGameObject* pDest)->_bool
	{
		return dynamic_cast<CBlendObject*>(pSour)->Get_Depth() > dynamic_cast<CBlendObject*>(pDest)->Get_Depth();
	});*/

	for (auto& pGameObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_BLEND)])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_BLEND)].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& pGameObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_UI)])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_UI)].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Lights()
{
	/* Shade */
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Lights"))))
		return E_FAIL;

	/* 논블렌드를 그릴 때 계산한 노말을 라이트를 그릴때 갖고옴 */
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Normal"), "g_NormalTexture", m_pShader)))
		return E_FAIL;

	/* 논블렌드를 그릴 때 계산한 깊이를 라이트를 그릴때 갖고옴 */
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Depth"), "g_DepthTexture", m_pShader)))
		return E_FAIL;

	/* 이제 실제로 버퍼에 조명을 그려줘야 함 */
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrixInv", m_pGameInstance->Get_Transform_Float4x4_Inv(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrixInv", m_pGameInstance->Get_Transform_Float4x4_Inv(D3DTS::PROJ))))
		return E_FAIL;

	if(FAILED(m_pShader->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_RawValue("g_fFar", m_pGameInstance->Get_Far_Ptr(), sizeof(_float))))
		return E_FAIL;

	/* 셰이더와 버퍼를 넘겨서 빛들이 직접 본인들의 정보를 넘겨서 그릴수 있도록 함 */
	if (FAILED(m_pGameInstance->Render_Lights(m_pShader, m_pVIBuffer)))
		return E_FAIL;

	/* 장치에 백버퍼로 복구한다. */
	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_BackBuffer()
{
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Diffuse"), "g_DiffuseTexture", m_pShader)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Shade"), "g_ShadeTexture", m_pShader)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Specular"), "g_SpecularTexture", m_pShader)))
		return E_FAIL;

	/* 장치에 이미 셋 되어있기 때문에 할 필요 없을 수도 있겠지만? */
	/* 백버퍼에 그리기 전 다른 후처리가 적용될 때 바뀔 수도 있으니까 */
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	/* 디퍼드 쉐이더 패스 */
	m_pShader->Begin(3);

	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	return S_OK;
}

HRESULT CRenderer::Render_NonLight()
{
	for (auto& pGameObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_NONLIGHT)])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_NONLIGHT)].clear();

	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderer::Render_Debug()
{
	/* 디버그 컴포넌트를 각 객체들이 맡아서 그리고 있었기 때문에, */
	/* 빛연산이 필요하지 않은 컴포넌트가 빛연산이 되버리는게 문제가 됨 */
	/* 그래서 렌더러에 컴포넌트를 따로 그려주도록 처리한다 */
	for (auto& pDebugCom : m_DebugComponents)
	{
		if (nullptr != pDebugCom)
		{
			pDebugCom->Render();
			Safe_Release(pDebugCom);
		}
	}
	m_DebugComponents.clear();

	/* 직교투영용 행렬을 셰이더에 던진다..
	월드를 안던지는 이유?? 렌더타겟마다 다른 위치에 그려져야 하기 때문에
	셰이더와 버퍼를 렌더타겟까지 끌고가서 렌더타겟의 월드행렬을 셰이더에 던지고 그려준다 */
	m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

	m_pGameInstance->Render_MRT_Debug(TEXT("MRT_GameObjects"), m_pShader, m_pVIBuffer);
	m_pGameInstance->Render_MRT_Debug(TEXT("MRT_Lights"), m_pShader, m_pVIBuffer);

	return S_OK;
}
#endif

HRESULT CRenderer::Ready_Resources()
{
	/* 버퍼 생성, 렌더타겟을 나눠서 그릴 때 백그라운드를 하나 만들어서 거기에 그림 */
	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	/* 당연히 셰이더도 있어야겠지요,, */
	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

	return S_OK;
}

CRenderer* CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderer* pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderer::Free()
{
	__super::Free();

	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pShader);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	for (auto& ObjectList : m_RenderObjects)
	{
		for (auto& pGameObject : ObjectList)
			Safe_Release(pGameObject);
		ObjectList.clear();
	}
	m_RenderObjects->clear();

	for (auto& pDebugCom : m_DebugComponents)
		Safe_Release(pDebugCom);
	m_DebugComponents.clear();
}
