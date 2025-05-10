#pragma once
#include "MeshExporter_Defines.h"
#include "Level.h"

BEGIN(MeshExporter)

class CLevel_MeshExporter final : public CLevel
{
public:
	typedef struct tagNonAnimMesh
	{
		_uint iMaterialIndex = 0;
		vector<VTXMESH> Vertices;
		vector<_uint> Indices;

	}NONANIMMESH;
private:
	CLevel_MeshExporter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_MeshExporter() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	string m_strFileName;

	vector<NONANIMMESH*> m_vecNonAnimMeshes;

private:
	HRESULT Ready_ImGui();
	HRESULT Ready_DockSpace();

	HRESULT Render_Begin_ImGui();
	HRESULT Render_End_ImGui();

	HRESULT FileDialog();
	HRESULT FBXFolder_to_Bin_Exported(const string& strFilePath);
	HRESULT FBX_to_Bin_Exproted(const string& strFilePath);

public:
	static CLevel_MeshExporter* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END