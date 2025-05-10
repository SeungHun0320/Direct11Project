#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel : public CComponent
{
public:
	typedef struct tagMeshDataDesc
	{

	}MESH_DESC;

protected:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& Prototype);
	~CModel() = default;

public:
	virtual HRESULT Initialize_Prototype(const _wstring& strModelFilePath);
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Render();

private:
	/* �޽� ���� */
	_uint					m_iNumMeshes = {};
	/* �޽����� ��Ƶδ� ���� */
	vector<class CMesh*>	m_Meshes;

private:
	HRESULT Ready_Meshes(const _wstring& strModelFilePath);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _wstring& strModelFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
