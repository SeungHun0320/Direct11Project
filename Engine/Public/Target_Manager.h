#pragma once

#include "Base.h"

/* 렌더타겟들을 만들고, 관리해주는 매니저, 렌더러에서 갖다 쓸 예정 */

BEGIN(Engine)

class CTarget_Manager final : public CBase
{
private:
	CTarget_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTarget_Manager() = default;

public:
	HRESULT Initialize();
	/* 매니저가 직접 렌더타겟을 만들고, 관리해준다. */
	HRESULT Add_RenderTarget(const _wstring& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	HRESULT Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag);

	/* 특정 타겟들을 장치에 동시에(최대 8개) 바인딩 해준다 */
	HRESULT Begin_MRT(const _wstring& strMRTTag);
	HRESULT End_MRT();
	HRESULT Bind_ShaderResource(const _wstring& strTargetTag, const _char* pContantName, class CShader* pShader);

#ifdef _DEBUG
	HRESULT Ready_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug(const _wstring strMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif

private: /* 렌더타겟을 생성 할 때 보내주기 위한 장치들 */
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	/* 그리고 난뒤에, 다시 원래 백버퍼로 돌려놓기 위해서 저장된 멤버들 */
	/* 백버퍼, 깊이버퍼, 깊이버퍼는 한개를 할당해서 그걸 갖다쓰는게 맞음 */
	/* 여러개를 할당하면 비교해주기가 힘들어지기 때문 */
	ID3D11RenderTargetView* m_pBackBuffer = { nullptr };
	ID3D11DepthStencilView* m_pOriginalDSV  = { nullptr };

private:/* 태그를 통해서 맵으로 관리 할 예정 */
	map<const _wstring, class CRenderTarget*>			m_RenderTargets;
	/* MultiRenderTarget, 여러개의 렌더타겟을 한번에 바인딩 할때 쓰임*/
	map<const _wstring, list<class CRenderTarget*>>		m_MRTs;

private:
	class CRenderTarget*		Find_RenderTarget(const _wstring& strTargetTag);
	list<class CRenderTarget*>* Find_MRT(const _wstring& strMRTTag);

public:
	static CTarget_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END