#pragma once

#include "VIBuffer_Instance.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Mesh_Particle_Instance  final : public CVIBuffer_Instance
{
public:
	typedef struct tagNonAnimMeshDesc
	{
		_uint iNumVertices{};
		_uint iNumIndices{};
		_uint iMaterialIndex{};
		vector<_uint> Indicies;
		vector<VTXMESH> Vertices;
	}MESH_DESC;

	typedef struct tagMeshInstance : public CVIBuffer_Instance::DESC
	{
		_float3 vPivot{};
		_float2 vLifeTime{};
		_float2 vSpeed{};
		_bool   isLoop{};
	}DESC;
public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

private:
	CVIBuffer_Mesh_Particle_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Mesh_Particle_Instance(const CVIBuffer_Mesh_Particle_Instance& Prototype);
	virtual ~CVIBuffer_Mesh_Particle_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype(const MESH_DESC* pMeshArg, const DESC* pArg, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg);

	virtual void Drop(_float fTimeDelta) override;
	virtual void Spread(_float fTimeDelta) override;
	virtual void Shrink(_float fTimeDelta) override;

protected:
	_uint						m_iMaterialIndex = {};
	VTXMESH_PARTICLE_INSTANCE*  m_pVertexInstances = { nullptr };

protected:
	_float3  m_vPivot = {};
	_float*  m_pSpeeds = { nullptr };
	_bool    m_isLoop = { false };
	_uint    m_iNextEmitIndex = {};

public:
	static CVIBuffer_Mesh_Particle_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const MESH_DESC* pMeshArg, const DESC* pArg, _fmatrix PreTransformMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END