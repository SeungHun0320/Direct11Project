#pragma once

#include "MeshExporter_Defines.h"
#include "Component.h"

BEGIN(MeshExporter)

class CModel : public CComponent
{
protected:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& Prototype);
	~CModel() = default;

public:
	virtual HRESULT Initialize_Prototype(const _char* pModelFilePath);
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Render();

private:
	/* �ҷ��� ������ �о���̴� ������ */
	Assimp::Importer	m_Importer;

	/* �𵨿� ���� ��� ������ ��� �ִ� ����ü */
	const aiScene* m_pAIScene = { nullptr };

	/* �޽� ���� */
	_uint					m_iNumMeshes = {};
	/* �޽����� ��Ƶδ� ���� */
	vector<class CMesh*>	m_Meshes;

	_uint						m_iNumMaterials = {};
	vector<class CMaterial*>	m_Materials;

private:
	HRESULT Ready_Meshes();

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END

