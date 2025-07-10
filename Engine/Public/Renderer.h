#pragma once

#include "Base.h"

BEGIN(Engine)

class CRenderer final : public CBase
{
private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	HRESULT Initialize();
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	HRESULT Draw();

#ifdef _DEBUG
public: /* 디버그용 컴포넌트를 디버그용 렌더러쪽에 넣어준다. */
	HRESULT Add_DebugComponent(class CComponent* pDebugCom);
#endif

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };

private: /* 렌더 타겟을 나눴을때 그려주기 위한 직교투영 백그라운드 */
	class CVIBuffer_Rect*		m_pVIBuffer = { nullptr };
	class CShader*				m_pShader = { nullptr };
	/* 셰이더 전역변수에 값을 던져주기 위한 직교투영용 행렬들 */
	/* 월드매트릭스는 백그라운드를 그려주기 위해서 */
	_float4x4					m_WorldMatrix{}, m_ViewMatrix{}, m_ProjMatrix{};
	ID3D11DepthStencilView*     m_pShadowDSV = { nullptr };
	_uint						m_iOriginWidth{}, m_iOriginHeight{};

private:
	list<class CGameObject*>	m_RenderObjects[ENUM_CLASS(RENDERGROUP::RG_END)];

#ifdef _DEBUG
private:
	list<class CComponent*>     m_DebugComponents;
#endif

private:
	HRESULT Render_Priority();
	HRESULT Render_NonBlend();
	HRESULT Render_Blend();
	HRESULT Render_UI();

	HRESULT Render_Lights();
	HRESULT Render_BackBuffer();
	HRESULT Render_NonLight();
	HRESULT Render_Shadow();

	HRESULT Render_Blur();
	HRESULT Render_Final();

#ifdef _DEBUG
private:
	HRESULT Render_Debug();
#endif

private:
	HRESULT Ready_Resources();
	HRESULT Ready_DepthStencilView(_uint iWidth, _uint iHeight);
	HRESULT Change_ViewportDesc(_uint iWidth, _uint iHeight);

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END