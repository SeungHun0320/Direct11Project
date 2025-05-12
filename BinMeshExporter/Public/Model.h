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
	/* 불러온 파일을 읽어들이는 임포터 */
	Assimp::Importer	m_Importer;

	/* 모델에 대한 모든 정보를 담고 있는 구조체 */
	const aiScene* m_pAIScene = { nullptr };

	/* 메쉬 갯수 */
	_uint					m_iNumMeshes = {};
	/* 메쉬들을 담아두는 벡터 */
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

