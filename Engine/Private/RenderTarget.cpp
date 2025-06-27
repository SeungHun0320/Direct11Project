#include "RenderTarget.h"
#include "GameInstance.h"

CRenderTarget::CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice  { pDevice }
	, m_pContext { pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CRenderTarget::Initialize(_uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
	/* 클리어 컬러를 멤버에 담아둠 */
	m_vClearColor = vClearColor;

	/* 텍스쳐2D를 생성한다 렌더타겟이든, 셰이더 리소스뷰든 생성시에 텍스쳐2D가 필요하기때문에 먼저 생성해줌 */
	D3D11_TEXTURE2D_DESC TextureDesc{};
	TextureDesc.Width = iWidth;
	TextureDesc.Height = iHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = ePixelFormat;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;
	/* 정적으로 만들어줘야겠지요 ? */
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	/* 렌더타겟이랑 리소스뷰 둘다 바인딩 할 수 있도록 플래그를 잡아줌 */
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	/* 서브리소스들은 전혀 필요없음 텍스쳐2D를 먼저 할당하고, 할당된 텍스쳐2D를 이용해 렌더타겟과 리소스뷰를 만든다 */
	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateRenderTargetView(m_pTexture2D, nullptr, &m_pRTV)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateShaderResourceView(m_pTexture2D, nullptr, &m_pSRV)))
		return E_FAIL;


	return S_OK;
}

HRESULT CRenderTarget::Bind_ShaderResource(const _char* pContantName, CShader* pShader)
{
	return pShader->Bind_SRV(pContantName, m_pSRV);
}

void CRenderTarget::Clear()
{
	/* 생성할때 지정해준 클리어컬러로 렌더타겟을 클리어해준다 */
	m_pContext->ClearRenderTargetView(m_pRTV, reinterpret_cast<const _float*>(&m_vClearColor));
}

HRESULT CRenderTarget::Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY)
{
	_uint				iNumViewports = { 1 };
	D3D11_VIEWPORT		ViewPortDesc{};

	m_pContext->RSGetViewports(&iNumViewports, &ViewPortDesc);

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	m_WorldMatrix._11 = fSizeX;
	m_WorldMatrix._22 = fSizeY;
	m_WorldMatrix._41 = fX - ViewPortDesc.Width * 0.5f;
	m_WorldMatrix._42 = -fY + ViewPortDesc.Height * 0.5f; 

	return S_OK;
}

HRESULT CRenderTarget::Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	/* 본인이 들고있는 월드행렬을 던져줌 (스케일, 위치) */
	if (FAILED(pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;

	if (FAILED(pShader->Bind_SRV("g_RenderTargetTexture", m_pSRV)))
		return E_FAIL;

	/* 디버그용은 0번 패스*/
	if (FAILED(pShader->Begin(0)))
		return E_FAIL;

	if (FAILED(pVIBuffer->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(pVIBuffer->Render()))
		return E_FAIL;

	return S_OK;
}

CRenderTarget* CRenderTarget::Craete(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
	CRenderTarget* pInstance = new CRenderTarget(pDevice, pContext);

	if (FAILED(pInstance->Initialize(iWidth, iHeight, ePixelFormat, vClearColor)))
	{
		MSG_BOX("Failed to Created : CRenderTarget");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderTarget::Free()
{
	__super::Free();

	Safe_Release(m_pSRV);
	Safe_Release(m_pRTV);
	Safe_Release(m_pTexture2D);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
