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
		_uint iNumOffsetMatrices{};
		_uint iNumBoneIndices{};
		_uint iNumBones{};
		vector<_uint> BoneIndices;
		vector<VTXANIMMESH> Vertices;
		vector<_float4x4> OffsetMatrices;

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

	HRESULT Bind_Bone_Matrices(class CShader* pShader, const _char* pConstantName, const vector<class CBone*>& Bones);

private:
	_uint			m_iMaterialIndex = {};

	_uint           m_iNumBones = {};

	vector<_int>			m_BoneIndices;

	_float4x4				m_BoneMatrices[g_iMaxNumBones] = {};

	vector<_float4x4>		m_OffsetMatrices;

private:
	HRESULT Ready_NonAnim_Mesh(const COMMON* pDesc, _fmatrix PreTransformMatrix);
	HRESULT Ready_Anim_Mesh(const COMMON* pDesc);

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eType, const COMMON* pDesc, _fmatrix PreTransformMatrix);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END

