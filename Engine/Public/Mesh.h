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

	}MESH;

private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& Prototype);
	virtual ~CMesh() = default;

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

public:
	virtual HRESULT Initialize_Prototype(const MESH* pDesc);
	virtual HRESULT Initialize(void* pArg);

private:
	_uint			m_iMaterialIndex = {};

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const MESH* pDesc);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END

