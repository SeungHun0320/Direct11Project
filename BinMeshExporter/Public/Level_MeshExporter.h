#pragma once
#include "MeshExporter_Defines.h"
#include "Level.h"

BEGIN(MeshExporter)

class CLevel_MeshExporter final : public CLevel
{
	/* ��ִ� �޽� */
public:
	typedef struct tagNonAnimMesh
	{
		_uint iMaterialIndex = {};
		vector<VTXMESH> Vertices;
		vector<_uint> Indices;

	}NONANIMMESH;

	/* ���׸��� */
public:
	typedef struct tagTexInfo
	{
		_wstring strTexturePath;
		_uint    iTextureType{};

	}TEX_INFO;

	typedef struct tagMaterial
	{
		/* �ؽ��� ��� and AI �ؽ��� Ÿ���� �̾Ƽ� �����ϰų�, */
		/* ��� AI_TEXTURE_TYPE�� �ִ� �ؽ������� */
		vector<TEX_INFO> vecTextures;

	}MATERIAL;


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
	vector<MATERIAL*>	 m_vecMaterials;

private:
	_int m_iType = {};

private:
	HRESULT Ready_ImGui();
	HRESULT Ready_DockSpace();

	HRESULT Render_Begin_ImGui();
	HRESULT Render_End_ImGui();

	HRESULT FileDialog();
	HRESULT FBX_to_Bin_Exproted(const string& strFilePath);

	HRESULT Read_Anim_Model();
	HRESULT Exported_Anim_Model();

	HRESULT Read_Non_Anim_Model(const string& strModelPath, const string& strOutPath);
	HRESULT Exported_Non_Anim_Model(const string& strOutPath);

	HRESULT Ready_Non_Anim_Meshes(_uint iNumMeshes, aiMesh** ppMeshes);
	HRESULT Ready_Material(const _char* pModelFilePath, _uint iNumMaterial, aiMaterial** ppAIMaterial);

public:
	static CLevel_MeshExporter* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END