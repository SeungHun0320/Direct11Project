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
	D3D11_VIEWPORT	ViewportDesc{};

	m_pContext->RSGetViewports(&iNumViewPorts, &ViewportDesc);

	m_iOriginWidth = static_cast<_uint>(ViewportDesc.Width);
	m_iOriginHeight = static_cast<_uint>(ViewportDesc.Height);

	/* 32비트로 표현 8 8 8 8 */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Diffuse"), m_iOriginWidth, m_iOriginHeight, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	/* DXGI_FORMAT_R16G16B16A16_UNORM :  0 ~ 65535의 값을 0.f ~ 1.f로 바꿔서 사용함 나중에 노말 계산할 때 잘 알아두기 */
	/* 노말은 -1.f ~ 1.f까지인데, UNORM으로 받아왔기 때문에 -1.f이 자동으로 0.f로 바뀌어서 저장되기 때문에 셰이더에서 0.f ~ 1.f로 치환한뒤에 Out에 저장해줘야 함*/
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Normal"),	m_iOriginWidth, m_iOriginHeight, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Shade"), m_iOriginWidth, m_iOriginHeight, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	/* 깊이를 계산해서 렌더타겟에 저장한다 원래 16으로 받아왔지만, 0~1 사이의 값을 완벽하게 채울 수 없기 때문에 어쩔 수 없이 32로 포맷을 바꿔줌 */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Depth"), m_iOriginWidth, m_iOriginHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 1.f, 0.f, 0.f))))
		return E_FAIL;

	/* 스펙큘러는 합연산이기때문에 백버퍼를 검정색으로 그려준다 */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Specular"), m_iOriginWidth, m_iOriginHeight, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_PickPos"), m_iOriginWidth, m_iOriginHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_GlobalShadow"), g_iMaxWidth, g_iMaxHeight, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Shadow"), g_iMaxWidth, g_iMaxHeight, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Final"), m_iOriginWidth, m_iOriginHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Effects"), m_iOriginWidth, m_iOriginHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
											 			   
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_BlurX"), m_iOriginWidth, m_iOriginHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;	
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_BlurY"), m_iOriginWidth, m_iOriginHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Depth"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_PickPos"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Lights"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Lights"), TEXT("Target_Specular"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GlobalShadow"), TEXT("Target_GlobalShadow"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_ShadowObjects"), TEXT("Target_Shadow"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Final"), TEXT("Target_Final"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Effects"), TEXT("Target_Effects"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_BlurX"), TEXT("Target_BlurX"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_BlurY"), TEXT("Target_BlurY"))))
		return E_FAIL;

	if (FAILED(Ready_Resources()))
		return E_FAIL;

	if (FAILED(Ready_DepthStencilView(g_iMaxWidth, g_iMaxHeight)))
		return E_FAIL;

	/* 화면에 꽉 채워서 그려야 하기때문에 뷰포트사이즈를 통해 크기를 맞춰줌 */
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(ViewportDesc.Width, ViewportDesc.Height, 1.f));
	/* 직교 투영용 행렬을 만들어줌 */
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.0f, 1.f));

#ifdef _DEBUG
	_float fRTWidth = ViewportDesc.Width * 0.225f;
	_float fRTHeight = ViewportDesc.Height * 0.225f;
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

	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_GlobalShadow"), ViewportDesc.Width - (fRTWidth * 0.5f), fRTHeight * 1.5f, fRTWidth, fRTHeight)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Shadow"), ViewportDesc.Width - (fRTWidth * 0.5f), fRTHeight * 0.5f, fRTWidth, fRTHeight)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Effects"), fRTWidth * 0.5f, fRTHeight * 3.5f, fRTWidth, fRTHeight)))
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

	if (FAILED(Render_Shadow()))
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

	if (FAILED(Render_Blur()))
		return E_FAIL;

	if (FAILED(Render_Final()))
		return E_FAIL;

	if (FAILED(Render_Blend()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;
	
#ifdef _DEBUG
	//if (FAILED(Render_Debug()))
	//	return E_FAIL;
#endif

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Final"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_PRIORITY)])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_PRIORITY)].clear();


	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

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
	if(FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Final"), false)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Diffuse"), "g_DiffuseTexture", m_pShader)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Shade"), "g_ShadeTexture", m_pShader)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Specular"), "g_SpecularTexture", m_pShader)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Depth"), "g_DepthTexture", m_pShader)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Shadow"), "g_ShadowTexture", m_pShader)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_GlobalShadow"), "g_GlobalShadowTexture", m_pShader)))
		return E_FAIL;

	/* 장치에 이미 셋 되어있기 때문에 할 필요 없을 수도 있겠지만? */
	/* 백버퍼에 그리기 전 다른 후처리가 적용될 때 바뀔 수도 있으니까 */
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

	if (FAILED(m_pShader->Bind_Matrix("g_LightViewMatrix", m_pGameInstance->Get_Light_ViewMatrix())))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_LightProjMatrix", m_pGameInstance->Get_Light_ProjMatrix())))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_RawValue("g_fFar", m_pGameInstance->Get_Far_Ptr(), sizeof(_float))))
		return E_FAIL;

	/* 디퍼드 쉐이더 패스 */
	if (FAILED(m_pShader->Begin(3)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_NonLight()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Effects"))))
		return E_FAIL;

	for (auto& pGameObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_DEPTH_NONLIGHT)])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_DEPTH_NONLIGHT)].clear();


	for (auto& pGameObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_NONLIGHT)])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_NONLIGHT)].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Shadow()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_GlobalShadow"), true, m_pShadowDSV, true)))
		return E_FAIL;

	if (FAILED(Change_ViewportDesc(g_iMaxWidth, g_iMaxHeight)))
		return E_FAIL;

	for (auto& pGameObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_GLOBAL_SHADOW)])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_Shadow();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_GLOBAL_SHADOW)].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_ShadowObjects"), true, m_pShadowDSV)))
		return E_FAIL;

	for (auto& pGameObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_SHADOW)])
	{
		if (nullptr != pGameObject)
			pGameObject->Render_Shadow();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_SHADOW)].clear();

	if(FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	if (FAILED(Change_ViewportDesc(m_iOriginWidth, m_iOriginHeight)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Blur()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_BlurX"))))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	
	_float fViewportWidth = static_cast<_float>(m_iOriginWidth);
	if (FAILED(m_pShader->Bind_RawValue("g_fViewportWidth", &fViewportWidth, sizeof(_float))))
		return E_FAIL;
	
	/* 타겟 최종에서 그린 값을 MRT_BlurX에 적용시킨다 */
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Effects"), "g_EffectTexture", m_pShader)))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(4)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	/*--------------------------------------------------------------------------------------------------------*/

	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_BlurY"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_BlurX"), "g_BlurXTexture", m_pShader)))
		return E_FAIL;

	_float fViewportHeight = static_cast<_float>(m_iOriginHeight);
	if (FAILED(m_pShader->Bind_RawValue("g_fViewportHeight", &fViewportHeight, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(5)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Final()
{

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_BlurY"), "g_BlurXTexture", m_pShader)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Final"), "g_FinalTexture", m_pShader)))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(6)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;


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
	m_pGameInstance->Render_MRT_Debug(TEXT("MRT_ShadowObjects"), m_pShader, m_pVIBuffer);
	m_pGameInstance->Render_MRT_Debug(TEXT("MRT_GlobalShadow"), m_pShader, m_pVIBuffer);
	m_pGameInstance->Render_MRT_Debug(TEXT("MRT_Effects"), m_pShader, m_pVIBuffer);

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

HRESULT CRenderer::Ready_DepthStencilView(_uint iWidth, _uint iHeight)
{
	ID3D11Texture2D* pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	/* 깊이 버퍼의 픽셀은 백버퍼의 픽셀과 갯수가 동일해야만 깊이 텍스트가 가능해진다. */
	/* 픽셀의 수가 다르면 아에 렌더링을 못함. */

	/* 그래서 그림자용 깊이버퍼를 새로 만들어준다. */
	TextureDesc.Width = iWidth;
	TextureDesc.Height = iHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;


	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pShadowDSV)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);

	return S_OK;
}

HRESULT CRenderer::Change_ViewportDesc(_uint iWidth, _uint iHeight)
{
	/* 백퍼버를 크게 그린다면, 뷰포트상에서 밀려서 그려지기 때문에 뷰포트도 사이즈를 맞춰줘야 한다. */
	D3D11_VIEWPORT			ViewportDesc{};
	_uint					iNumViewports = { 1 };

	ViewportDesc.TopLeftX = 0;
	ViewportDesc.TopLeftY = 0;
	ViewportDesc.Width = static_cast<_float>(iWidth);
	ViewportDesc.Height = static_cast<_float>(iHeight);
	ViewportDesc.MinDepth = 0.f;
	ViewportDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(iNumViewports, &ViewportDesc);

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

	Safe_Release(m_pShadowDSV);

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

#ifdef _DEBUG
	for (auto& pDebugCom : m_DebugComponents)
		Safe_Release(pDebugCom);
	m_DebugComponents.clear();

#endif

}
