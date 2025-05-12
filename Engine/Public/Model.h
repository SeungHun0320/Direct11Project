#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel : public CComponent
{
protected:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& Prototype);
	~CModel() = default;

public:
	_uint Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	HRESULT Bind_Material(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex, TEX_TYPE eType, _uint iTextureIndex = 0);

public:
	virtual HRESULT Initialize_Prototype(const _wstring& strModelFilePath);
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Render(_uint iMeshIndex);

private:
	/* 메쉬 갯수, 메쉬들을 담아두는 벡터 */
	_uint					m_iNumMeshes = {};
	vector<class CMesh*>	m_Meshes;

	/* 머테리얼 갯수, 머테리얼들을 담아두는 벡터 */
	_uint						m_iNumMaterials = {};
	vector<class CMaterial*>	m_Materials;

private:
	HRESULT Ready_Meshes(ifstream& _InFile);
	HRESULT Ready_Material(ifstream& _InFile);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _wstring& strModelFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
