#include "PixelPicking.h"
#include "GameInstance.h"

CPixelPicking::CPixelPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CPixelPicking::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;

	_uint			iNumViewPort{ 1 };
	D3D11_VIEWPORT  ViewPortDesc{};

	m_pContext->RSGetViewports(&iNumViewPort, &ViewPortDesc);

	m_iWidth = static_cast<_uint>(ViewPortDesc.Width);
	m_iHeight = static_cast<_uint>(ViewPortDesc.Height);

	D3D11_TEXTURE2D_DESC		TextureDesc{};
	TextureDesc.Width = m_iWidth;
	TextureDesc.Height = m_iHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.Usage = D3D11_USAGE_STAGING;
	TextureDesc.BindFlags = 0;
	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D)))
		return E_FAIL;

	m_pWorldPositions = new _float4[m_iWidth * m_iHeight];
	ZeroMemory(m_pWorldPositions, sizeof(_float4) * m_iWidth * m_iHeight);

	return S_OK;
}

void CPixelPicking::Update()
{
	if (FAILED(m_pGameInstance->Copy_RT_Resource(TEXT("Target_PickPos"), m_pTexture2D)))
		return;

	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pTexture2D, 0, D3D11_MAP_READ, 0, &SubResource);

	memcpy(m_pWorldPositions, SubResource.pData, sizeof(_float4) * m_iWidth * m_iHeight);

	m_pContext->Unmap(m_pTexture2D, 0);
}

_bool CPixelPicking::Picking(_float4* pOut)
{
	POINT			ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	_uint			iIndex = ptMouse.y * m_iWidth + ptMouse.x;

	// *pOut = m_pWorldPostions[iIndex].w > 0.f ? m_pWorldPostions[iIndex] : *pOut;

	*pOut = m_pWorldPositions[iIndex];

	return static_cast<_bool>(m_pWorldPositions[iIndex].w);
}

CPixelPicking* CPixelPicking::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd)
{
	CPixelPicking* pInstance = new CPixelPicking(pDevice, pContext);

	if (FAILED(pInstance->Initialize(hWnd)))
	{
		MSG_BOX("Failed to Created : CPixelPicking");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPixelPicking::Free()
{
	__super::Free();

	Safe_Delete_Array(m_pWorldPositions);

	Safe_Release(m_pGameInstance);

	Safe_Release(m_pTexture2D);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
