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
	virtual HRESULT Initialize_Prototype(MODEL eType, const _wstring& strModelFilePath, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Render(_uint iMeshIndex);

public: /* ������,,,? */
	_float3 Compute_PickedPosition_Local(_fmatrix WorldMatrixInverse);
	_float3 Compute_PickedPosition_World(const _float4x4* pWorldMatrix);
	_float3 Compute_PickedPosition_World_Snap(const _float4x4* pWorldMatrix);

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

private:
	HRESULT Ready_Bones(ifstream& _InFile);
	HRESULT Ready_NonAnim_Meshes(ifstream& _InFile);
	HRESULT Ready_Anim_Meshes(ifstream& _InFile);
	HRESULT Ready_Material(ifstream& _InFile);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eType, const _wstring& strModelFilePath, _fmatrix PreTransformMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
