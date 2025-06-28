#pragma once

#include "Component.h"

#include "VIBuffer_Mesh_Instance.h"

BEGIN(Engine)

class ENGINE_DLL CModel_Instance : public CComponent
{
protected:
	CModel_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel_Instance(const CModel_Instance& Prototype);
	~CModel_Instance() = default;

public:
	_uint Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	HRESULT Bind_Material(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex, TEX_TYPE eType, _uint iTextureIndex = 0);

public:
	virtual HRESULT Initialize_Prototype(const _wstring& strModelFilePath, const CVIBuffer_Mesh_Instance::DESC* pArg, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Render(_uint iMeshIndex);

private: /* 메쉬 갯수, 메쉬들을 담아두는 벡터 */
	_uint							    m_iNumMeshes = {};
	vector<CVIBuffer_Mesh_Instance*>	m_MeshInstances;

private: /* 머테리얼 갯수, 머테리얼들을 담아두는 벡터 */
	_uint						m_iNumMaterials = {};
	vector<class CMaterial*>	m_Materials;

protected:
	HRESULT Ready_Meshes(ifstream& _InFile, const CVIBuffer_Mesh_Instance::DESC* pArg, _fmatrix PreTransformMatrix);
	HRESULT Ready_Material(ifstream& _InFile);

public:
	static CModel_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _wstring& strModelFilePath, const CVIBuffer_Mesh_Instance::DESC* pArg, _fmatrix PreTransformMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END