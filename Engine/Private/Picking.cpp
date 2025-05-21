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

	/*1. 뷰포트 상(윈도우공간)의 마우스 위치를 구하자. */
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	/*2. 투영스페이스로 옮기자. 로컬위치 * 월드행렬 * 뷰행렬 * 투영행렬 * 1/w */
	_float4			vPosition = { };

	vPosition.x = ptMouse.x / (m_iWinSizeX * 0.5f) - 1.f;
	vPosition.y = ptMouse.y / (m_iWinSizeY * -0.5f) + 1.f;
	vPosition.z = 0.0f;
	vPosition.w = 1.f;

	/*3. 뷰스페이스로 옮기자. 로컬위치 * 월드행렬 * 뷰행렬 */
	//_float4x4		ProjMatrix{};
	_matrix ProjMatrix = m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ);
	ProjMatrix = XMMatrixInverse(nullptr, ProjMatrix);

	XMStoreFloat4(&vPosition, XMVector4Transform(XMLoadFloat4(&vPosition), ProjMatrix));

	m_vMousePos = _float3(0.f, 0.f, 0.f);
	m_vMouseRay = _float3(vPosition.x, vPosition.y, vPosition.z);

	/*3. 월드 스페이스로 옮기자. 로컬위치 * 월드행렬*/
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
	/* 월드에 있는 마우스와 마우스 방향으로 피킹된 위치를 구해줌 */
	XMStoreFloat3(&vPickedPos, XMLoadFloat3(&m_vMousePos) + XMLoadFloat3(&m_vMouseRay) * fDist);

	return isPicked;
}

_bool CPicking::Picking_InLocal(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC)
{
	_float		fDist{};

	_bool		isPicked = TriangleTests::Intersects(XMLoadFloat3(&m_vLocalMousePos), XMLoadFloat3(&m_vLocalMouseRay), XMLoadFloat3(&vPointA), XMLoadFloat3(&vPointB), XMLoadFloat3(&vPointC), fDist);
	/* 로컬에 있는 마우스와 마우스 방향으로 피킹된 위치를 구해줌 */
	XMStoreFloat3(&vPickedPos, XMLoadFloat3(&m_vLocalMousePos) + XMLoadFloat3(&m_vLocalMouseRay) * fDist);

	return isPicked;
}

_bool CPicking::Picking_InWorldEx(_float3& vPickedPos, _float& fDist, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC)
{
	_float		fDistance = { FLT_MAX };

	_bool		isPicked = TriangleTests::Intersects(XMLoadFloat3(&m_vMousePos), XMLoadFloat3(&m_vMouseRay), XMLoadFloat3(&vPointA), XMLoadFloat3(&vPointB), XMLoadFloat3(&vPointC), fDistance);
	/* 월드에 있는 마우스와 마우스 방향으로 피킹된 위치를 구해줌 */
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
	/* 로컬에 있는 마우스와 마우스 방향으로 피킹된 위치를 구해줌 */
	if (isPicked)
	{
		XMStoreFloat3(&vPickedPos, XMLoadFloat3(&m_vLocalMousePos) + XMLoadFloat3(&m_vLocalMouseRay) * fDistance);
		fDist = fDistance;
	}

	return isPicked;
}

void CPicking::Transform_ToLocalSpace(_fmatrix WorldMatrixInverse)
{
	/* 월드매트릭스는 각각의 객체가 들고 있고, */
	/* 우린 월드매트릭스의 역행렬을 구해주는 함수를 갖고 있다. */
	/* 월드매트릭스의 역행렬을 가져와서 */
	/* 월드상에 있는 마우스 위치, 방향을 각각 역행렬과 곱해준다면 */
	/* 각 객체의 로컬좌표로 마우스 위치와 방향이 이동될 것이다. */
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
