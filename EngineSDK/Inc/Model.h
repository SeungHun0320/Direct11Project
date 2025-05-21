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

	void Set_Animation(_uint iIndex, _bool isLoop = true) {
		m_iCurrentAnimIndex = iIndex;
		m_isLoop = isLoop;
	}

	HRESULT Bind_Material(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex, TEX_TYPE eType, _uint iTextureIndex = 0);
	HRESULT Bind_Bone_Matrices(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex);

public:
	virtual HRESULT Initialize_Prototype(MODEL eType, const _wstring& strModelFilePath, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Render(_uint iMeshIndex);

public:
	_bool Play_Animation(_float fTimeDelta);

public: /* ������,,,? */
	_float3 Compute_PickedPosition_Local(_fmatrix WorldMatrixInverse);
	_bool Compute_PickedPosition_World(const _float4x4* pWorldMatrix, _float3& fOutPos, _float& fDist);

private:
	class CGameInstance* m_pGameInstance = { nullptr };

private:
	/* � ������ �б� �� �� Ÿ�� (NONANIM, ANIM) */
	MODEL					m_eType = {};
	/* ������ǥ������ �ʱ� ���¸� �����ֱ� ���� ��Ʈ���� */
	_float4x4				m_PreTransformMatrix = {};

	/* �޽� ����, �޽����� ��Ƶδ� ���� */
	_uint					m_iNumMeshes = {};
	vector<class CMesh*>	m_Meshes;

	/* ���׸��� ����, ���׸������ ��Ƶδ� ���� */
	_uint						m_iNumMaterials = {};
	vector<class CMaterial*>	m_Materials;

	/* ������ ��� ���� */
	vector<class CBone*>			m_Bones;

	/* �ִϸ��̼��� ���� ���� ������, ���� �ִϸ��̼� �ε����� ������ */
	_bool						m_isLoop{};
	_uint						m_iCurrentAnimIndex = { };
	/* �ִϸ��̼� ����, �ִϸ��̼��� ��� ���� */
	_uint						m_iNumAnimations = {};
	vector<class CAnimation*>	m_Animations;

private:
	HRESULT Ready_Bones(ifstream& _InFile);
	HRESULT Ready_NonAnim_Meshes(ifstream& _InFile);
	HRESULT Ready_Anim_Meshes(ifstream& _InFile);
	HRESULT Ready_Material(ifstream& _InFile);
	HRESULT Ready_Animations(ifstream& _InFile);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eType, const _wstring& strModelFilePath, _fmatrix PreTransformMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
