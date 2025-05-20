#pragma once
#include "MeshExporter_Defines.h"
#include "Level.h"

BEGIN(MeshExporter)

class CLevel_MeshExporter final : public CLevel
{
public:	/* 논애님 메쉬 */
	typedef struct tagNonAnimMesh
	{
		_uint iMaterialIndex = {};
		vector<VTXMESH> Vertices;
		vector<_uint> Indices;

	}NONANIMMESH;

public: /* 애님메쉬 */
	typedef struct tagAnimMesh
	{
		_uint iMaterialIndex = {};
		vector<VTXANIMMESH> Vertices;
		vector<_uint> Indices;
		_uint iNumBones;
		vector<_uint> BoneIndices;
		vector<_float4x4> OffsetMatrices;

	}ANIMMESH;

public:	/* 마테리얼 */
	typedef struct tagTexInfo
	{
		_wstring strTexturePath;
		_uint    iTextureType{};

	}TEX_INFO;

	typedef struct tagMaterial
	{
		vector<TEX_INFO> vecTextures;

	}MATERIAL;

public: /* 본 */
	typedef struct tagBoneInfo
	{
		string strName;
		_int iParentBoneIndex;
		_float4x4 TransformationMatrix;

	}BONE;

public: /* 애니메이션 */
	typedef struct tagChannel
	{
		_uint iBoneIndex;
		_uint iNumKeyFrames;
		vector<KEYFRAME> KeyFrames;
	}CHANNEL;

	typedef struct tagAnimation
	{
		_float fDuration;
		_float fTickPerSecond;
		_float fCurrentTrackPosition;

		vector<CHANNEL> Channels;
	}ANIMATION;



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
	vector<ANIMMESH*>	 m_vecAnimMeshes;
	vector<MATERIAL*>	 m_vecMaterials;
	vector<BONE*>        m_vecBones;
	vector<ANIMATION*>   m_vecAnimations;

private:
	_int m_iType = {};

private:
	HRESULT Ready_ImGui();
	HRESULT Ready_DockSpace();

	HRESULT Render_Begin_ImGui();
	HRESULT Render_End_ImGui();

	HRESULT FileDialog();
	HRESULT FBX_to_Bin_Exproted(const string& strFilePath);

	HRESULT Read_Anim_Model(const string& strModelPath, const string& strOutPath);
	HRESULT Exported_Anim_Model(const string& strOutPath);
	
	HRESULT Read_Non_Anim_Model(const string& strModelPath, const string& strOutPath);
	HRESULT Exported_Non_Anim_Model(const string& strOutPath);

	HRESULT Ready_Bones(_int iParentBoneIndex, const aiNode* pAINode);
	HRESULT Ready_Anim_Meshes(_uint iNumMeshes, aiMesh** ppMeshes);
	HRESULT Ready_Non_Anim_Meshes(_uint iNumMeshes, aiMesh** ppMeshes);
	HRESULT Ready_Material(const _char* pModelFilePath, _uint iNumMaterial, aiMaterial** ppAIMaterial);
	HRESULT Ready_Animations(_uint iNumAnimations, aiAnimation** ppAnimations);

private:
	aiNode* Find_Node_That_UsesMeshIndex(aiNode* pNode, int meshIndex);

public:
	static CLevel_MeshExporter* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END