#pragma once

#include "Base.h"

BEGIN(Engine)

class CPixelPicking final : public CBase
{
private:
	CPixelPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	~CPixelPicking() = default;

public:
	HRESULT Initialize(HWND hWnd);
	void	Update();
	_bool	Picking(_float4* pOut);

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	ID3D11Texture2D*		m_pTexture2D = { nullptr };
	class CGameInstance*	m_pGameInstance = { nullptr };

	HWND		m_hWnd = {};
	_uint		m_iWidth{}, m_iHeight{};

	_float4*	m_pWorldPositions = { nullptr };

public:
	static CPixelPicking* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd);
	virtual void Free() override;
};

END