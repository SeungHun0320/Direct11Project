#pragma once

#include "MeshExporter_Defines.h"
#include "VIBuffer.h"

BEGIN(MeshExporter)

class CMesh final : public CVIBuffer
{
public:
	typedef struct tagMeshDesc
	{
		_uint iMaterialIndex{};
		vector<VTXMESH> vertices;
		vector<_uint> indices;

	}MESH_DESC;

private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& Prototype);
	virtual ~CMesh() = default;

public:
	virtual HRESULT Initialize_Prototype(const aiMesh* pAIMesh, _uint iNumMeshes);
	virtual HRESULT Initialize(void* pArg);

private:
	vector<MESH_DESC*> m_vecMeshes;
private:
	HRESULT Save_Meshes(const _wstring& strFileName);

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMesh* pAIMesh, _uint iNumMeshes);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END

