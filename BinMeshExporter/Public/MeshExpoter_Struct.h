#pragma once

typedef struct tagEngineDesc
{
	HWND			hWnd;
	HINSTANCE		hInst;
	bool			isWindowed;
	unsigned int	iWinSizeX;
	unsigned int	iWinSizeY;
	unsigned int	iNumLevels;
}ENGINE_DESC;

typedef struct tagVertexPositionTexcoord
{
	XMFLOAT3		vPosition;
	XMFLOAT2		vTexcoord;

	static const unsigned int					iNumElements = { 2 };

	static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
}VTXPOSTEX;

typedef struct tagVertexPositionNormalTexcoord
{
	XMFLOAT3		vPosition;
	XMFLOAT3		vNormal;
	XMFLOAT2		vTexcoord;

	static const unsigned int					iNumElements = { 3 };
	static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
}VTXNORTEX;

typedef struct tagVertexMesh
{
	XMFLOAT3		vPosition;
	XMFLOAT3		vNormal;
	XMFLOAT3		vTangent;
	XMFLOAT2		vTexcoord;

	static const unsigned int					iNumElements = { 4 };
	static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
}VTXMESH;
