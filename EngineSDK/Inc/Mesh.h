#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CMesh final : public CVIBuffer
{
public:
	typedef struct tagMeshDesc
	{
		_uint iNumVertices{};
		_uint iNumIndices{};
		_uint iMaterialIndex{};
		vector<VTXMESH> Vertices;
		vector<_uint> Indicies;

	}MESH_DESC;

private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& Prototype);
	virtual ~CMesh() = default;

public:
	virtual HRESULT Initialize_Prototype(MESH_DESC* pDesc);
	virtual HRESULT Initialize(void* pArg);

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MESH_DESC* pDesc);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END

