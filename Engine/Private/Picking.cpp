#include "Picking.h"
#include "GameInstance.h"

#include "GameObject.h"

CPicking::CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice {pDevice}
	, m_pContext {pContext}
	, m_pGameInstance {CGameInstance::Get_Instance()}
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CPicking::Initialize(HWND hWnd, _uint iWinSizeX, _uint iWinSizeY)
{
	m_hWnd = hWnd;
	m_iWinSizeX = iWinSizeX;
	m_iWinSizeY = iWinSizeY;
    return S_OK;
}

void CPicking::Update()
{
	POINT		ptMouse = {};

	/*1. ����Ʈ ��(���������)�� ���콺 ��ġ�� ������. */
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	/*2. ���������̽��� �ű���. ������ġ * ������� * ����� * ������� * 1/w */
	_float4			vPosition = { };

	vPosition.x = ptMouse.x / (m_iWinSizeX * 0.5f) - 1.f;
	vPosition.y = ptMouse.y / (m_iWinSizeY * -0.5f) + 1.f;
	vPosition.z = 0.0f;
	vPosition.w = 1.f;

	/*3. �佺���̽��� �ű���. ������ġ * ������� * ����� */
	//_float4x4		ProjMatrix{};
	_matrix ProjMatrix = m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ);
	ProjMatrix = XMMatrixInverse(nullptr, ProjMatrix);

	XMStoreFloat4(&vPosition, XMVector4Transform(XMLoadFloat4(&vPosition), ProjMatrix));

	m_vMousePos = _float3(0.f, 0.f, 0.f);
	m_vMouseRay = _float3(vPosition.x, vPosition.y, vPosition.z);

	/*3. ���� �����̽��� �ű���. ������ġ * �������*/
	//_float4x4		ViewMatrix{};
	_matrix ViewMatrix = m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW);
	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);

	XMStoreFloat3(&m_vMousePos, XMVector3TransformCoord(XMLoadFloat3(&m_vMousePos), ViewMatrix));
	XMStoreFloat3(&m_vMouseRay, XMVector3TransformNormal(XMLoadFloat3(&m_vMouseRay), ViewMatrix));

 	XMStoreFloat3(&m_vMouseRay, XMVector3Normalize(XMLoadFloat3(&m_vMouseRay)));
}

_bool CPicking::Picking_InWorld(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC)
{
	_float		fDist{};

	_bool		isPicked = TriangleTests::Intersects(XMLoadFloat3(&m_vMousePos), XMLoadFloat3(&m_vMouseRay), XMLoadFloat3(&vPointA), XMLoadFloat3(&vPointB), XMLoadFloat3(&vPointC), fDist);
	/* ���忡 �ִ� ���콺�� ���콺 �������� ��ŷ�� ��ġ�� ������ */
	XMStoreFloat3(&vPickedPos, XMLoadFloat3(&m_vMousePos) + XMLoadFloat3(&m_vMouseRay) * fDist);

	return isPicked;
}

_bool CPicking::Picking_InLocal(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC)
{
	_float		fDist{};

	_bool		isPicked = TriangleTests::Intersects(XMLoadFloat3(&m_vLocalMousePos), XMLoadFloat3(&m_vLocalMouseRay), XMLoadFloat3(&vPointA), XMLoadFloat3(&vPointB), XMLoadFloat3(&vPointC), fDist);
	/* ���ÿ� �ִ� ���콺�� ���콺 �������� ��ŷ�� ��ġ�� ������ */
	XMStoreFloat3(&vPickedPos, XMLoadFloat3(&m_vLocalMousePos) + XMLoadFloat3(&m_vLocalMouseRay) * fDist);

	return isPicked;
}

_bool CPicking::Picking_InWorldEx(_float3& vPickedPos, _float& fDist, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC)
{
	_float		fDistance = { FLT_MAX };

	_bool		isPicked = TriangleTests::Intersects(XMLoadFloat3(&m_vMousePos), XMLoadFloat3(&m_vMouseRay), XMLoadFloat3(&vPointA), XMLoadFloat3(&vPointB), XMLoadFloat3(&vPointC), fDistance);
	/* ���忡 �ִ� ���콺�� ���콺 �������� ��ŷ�� ��ġ�� ������ */
	if (isPicked)
	{
		XMStoreFloat3(&vPickedPos, XMLoadFloat3(&m_vMousePos) + XMLoadFloat3(&m_vMouseRay) * fDistance);
		fDist = fDistance;
	}
	else
	{
		fDist = FLT_MAX;
	}

	return isPicked;
}

_bool CPicking::Picking_InLocalEx(_float3& vPickedPos, _float& fDist, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC)
{
	_float		fDistance{};

	_bool		isPicked = TriangleTests::Intersects(XMLoadFloat3(&m_vLocalMousePos), XMLoadFloat3(&m_vLocalMouseRay), XMLoadFloat3(&vPointA), XMLoadFloat3(&vPointB), XMLoadFloat3(&vPointC), fDist);
	/* ���ÿ� �ִ� ���콺�� ���콺 �������� ��ŷ�� ��ġ�� ������ */
	if (isPicked)
	{
		XMStoreFloat3(&vPickedPos, XMLoadFloat3(&m_vLocalMousePos) + XMLoadFloat3(&m_vLocalMouseRay) * fDistance);
		fDist = fDistance;
	}

	return isPicked;
}

void CPicking::Transform_ToLocalSpace(_fmatrix WorldMatrixInverse)
{
	/* �����Ʈ������ ������ ��ü�� ��� �ְ�, */
	/* �츰 �����Ʈ������ ������� �����ִ� �Լ��� ���� �ִ�. */
	/* �����Ʈ������ ������� �����ͼ� */
	/* ����� �ִ� ���콺 ��ġ, ������ ���� ����İ� �����شٸ� */
	/* �� ��ü�� ������ǥ�� ���콺 ��ġ�� ������ �̵��� ���̴�. */
	XMStoreFloat3(&m_vLocalMousePos, XMVector3TransformCoord(XMLoadFloat3(&m_vMousePos), WorldMatrixInverse));
	XMStoreFloat3(&m_vLocalMouseRay, XMVector3TransformNormal(XMLoadFloat3(&m_vMouseRay), WorldMatrixInverse));
}

CPicking* CPicking::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd, _uint iWinSizeX, _uint iWinSizeY)
{
	CPicking* pInstance = new CPicking(pDevice, pContext);

	if (FAILED(pInstance->Initialize(hWnd, iWinSizeX, iWinSizeY)))
	{
		MSG_BOX("Failed to Created : CPicking");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPicking::Free()
{
    __super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
}
