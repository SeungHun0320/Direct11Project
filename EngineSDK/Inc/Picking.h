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

	/* ������ �׳� ��ŷ�Լ��� ���ð� ���忡�� ���� ���ֱ� ����, InWorld�� InLocal�� �Լ��� ������� */
	_bool Picking_InWorld(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);
	_bool Picking_InLocal(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);

	/* ���忡�� ��ŷ�� �༮�� �Ÿ����� ��ȯ���� */
	_bool Picking_InWorldEx(_float3& vPickedPos, _float& fDist, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);
	/* ���ÿ��� ��ŷ�� �༮�� �Ÿ����� ��ȯ���� */
	_bool Picking_InLocalEx(_float3& vPickedPos, _float& fDist, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);

	/* ���콺�� ����� ��ǥ�� ���忡�� ���÷� �����ֱ� ���� �Լ� */
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

	/* ������ǥ�� �Űܿ� ���콺�� ����� ��ġ�� �����ϱ� ���� ���� */
	_float3			m_vLocalMouseRay = {};
	_float3			m_vLocalMousePos = {};

public:
	static CPicking* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd, _uint iWinSizeX, _uint iWinSizeY);
	virtual void Free()override;
};

END