#pragma once

#include "Base.h"

BEGIN(Engine)

class CRenderTarget final : public CBase
{
private:
	CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderTarget() = default;

public:
	ID3D11RenderTargetView* Get_RTV() const { return m_pRTV; }

public:
	HRESULT Initialize(_uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	void Clear();

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	/* 렌더타겟과 셰이더리소스뷰를 만들기 위한 텍스쳐2D */
	ID3D11Texture2D*			m_pTexture2D = { nullptr };
	/* 렌더타겟 */
	ID3D11RenderTargetView*		m_pRTV = { nullptr };
	/* 셰이더 리소스 뷰 */
	ID3D11ShaderResourceView*   m_pSRV = { nullptr };

private: /* 각 버퍼마다 클리어 컬러가 다름(각 버퍼마다 다르게 설정해줘야함, 디퓨즈면 마젠타, 스펙큘러면 검정색 (덧셈), 앰비언트면 하얀색(곱셈) 등등 */
	_float4 m_vClearColor = {};

public:
	/* 기본적으로 장치, 가로, 세로, 렌더타겟 생성용 포맷, 클리어 컬러  */
	static CRenderTarget* Craete(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	virtual void Free()override;
};

END