#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CMesh final : public CVIBuffer
{
public:
	typedef struct tagMeshDesc {
		_uint iNumVertices{};
		_uint iNumIndices{};
		_uint iMaterialIndex{};
		vector<_uint> Indicies;
	}COMMON;

	typedef struct tagNonAnimMeshDesc : public COMMON
	{
		vector<VTXMESH> Vertices;
	}NONANIMMESH;

	typedef struct tagAnimMeshDesc : public COMMON
	{
		vector<VTXANIMMESH> Vertices;

	}ANIMMESH;

private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& Prototype);
	virtual ~CMesh() = default;

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

public:
	virtual HRESULT Initialize_Prototype(MODEL eType, const COMMON* pDesc, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg);

private:
	_uint			m_iMaterialIndex = {};

private:
	HRESULT Ready_NonAnim_Mesh(const COMMON* pDesc, _fmatrix PreTransformMatrix);
	HRESULT Ready_Anim_Mesh(const COMMON* pDesc);

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eType, const COMMON* pDesc, _fmatrix PreTransformMatrix);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END

