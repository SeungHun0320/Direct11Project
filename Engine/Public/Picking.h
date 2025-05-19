#pragma once

#include "Base.h"

BEGIN(Engine)

class CPicking final : public CBase
{
private:
	CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	~CPicking() = default;
public:
	const _float3& Get_MousePos() {
		return m_vMousePos;
	}
	
	const _float3& Get_LocalMousePos() {
		return m_vLocalMousePos;
	}

public:
	HRESULT Initialize(HWND hWnd, _uint iWinSizeX, _uint iWinSizeY);
	void Update();

	/* 기존의 그냥 피킹함수를 로컬과 월드에서 각각 해주기 위해, InWorld와 InLocal로 함수를 나눠줬다 */
	_bool Picking_InWorld(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);
	_bool Picking_InLocal(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);

	/* 월드에서 피킹된 녀석의 거리까지 반환해줌 */
	_bool Picking_InWorldEx(_float3& vPickedPos, _float& fDist, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);
	/* 로컬에서 피킹된 녀석의 거리까지 반환해줌 */
	_bool Picking_InLocalEx(_float3& vPickedPos, _float& fDist, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);

	/* 마우스의 방향과 좌표를 월드에서 로컬로 보내주기 위한 함수 */
	void Transform_ToLocalSpace(_fmatrix WorldMatrixInverse);

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	class CGameInstance*	m_pGameInstance = { nullptr };

private:
	HWND			m_hWnd = {};
	_uint			m_iWinSizeX{}, m_iWinSizeY{};

	_float3			m_vMouseRay = {};
	_float3			m_vMousePos = {};

	/* 로컬좌표로 옮겨온 마우스의 방향과 위치를 저장하기 위한 변수 */
	_float3			m_vLocalMouseRay = {};
	_float3			m_vLocalMousePos = {};

public:
	static CPicking* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd, _uint iWinSizeX, _uint iWinSizeY);
	virtual void Free()override;
};

END