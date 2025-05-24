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

	_float3 Get_RootPosition() {
		return m_vPreRootPosition;
	}

	void Set_Animation(_uint iIndex, _bool isLoop = true, _float fBlendDuration = 0.f, _bool isBlended = false) {
		m_iCurrentAnimIndex = iIndex;
		m_isLoop = isLoop;
		m_isBlended = isBlended;
	}

	void Change_Animation(_uint iNextIndex, _bool isLoop = true, _float fBlendDuration = 0.f, _bool isBlended = true, _bool isRoot = false) {
		if (false == isBlended)
			m_iCurrentAnimIndex = iNextIndex;

		m_isLoop = isLoop;
		m_iNextAnimIndex = iNextIndex;
		m_isBlended = isBlended;
		m_fBlendTickPerSecond = 0.f;
		m_fBlendDuration = fBlendDuration;
		m_isRoot = isRoot;
	}

	HRESULT Bind_Material(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex, TEX_TYPE eType, _uint iTextureIndex = 0);
	HRESULT Bind_Bone_Matrices(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex);

public:
	virtual HRESULT Initialize_Prototype(MODEL eType, const _wstring& strModelFilePath, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Render(_uint iMeshIndex);

public:
	_matrix MatrixLerp(_fmatrix CurrentTransfomaionMatrix, _cmatrix NextTransformMatrix, _float fRatio);

public:
	_bool Play_Animation(_float fTimeDelta);
	void  Animation_Blend(_float fTimeDelta);

	/* 루트 포지션을 계산해주는 함수 */
	_vector  Compute_RootPosition();
	void     Update_RootPosition();

public: /* 맵툴용,,,? */
	_float3 Compute_PickedPosition_Local(_fmatrix WorldMatrixInverse);
	_bool Compute_PickedPosition_World(const _float4x4* pWorldMatrix, _float3& fOutPos, _float& fDist);

private:
	class CGameInstance* m_pGameInstance = { nullptr };

private:
	/* 어떤 모델인지 분기 해 줄 타입 (NONANIM, ANIM) */
	MODEL					m_eType = {};
	/* 로컬좌표에서의 초기 상태를 정해주기 위한 매트릭스 */
	_float4x4				m_PreTransformMatrix = {};

private: /* 메쉬 갯수, 메쉬들을 담아두는 벡터 */
	_uint					m_iNumMeshes = {};
	vector<class CMesh*>	m_Meshes;

private: /* 머테리얼 갯수, 머테리얼들을 담아두는 벡터 */
	_uint						m_iNumMaterials = {};
	vector<class CMaterial*>	m_Materials;

private: /* 본들을 담는 벡터 */
	vector<class CBone*>		m_Bones;
	_uint                       m_iRootBoneIndex = {};

private: /* 애니메이션을 루프 돌릴 것인지, 현재 애니메이션 인덱스가 몇인지 */
	_bool						m_isLoop{};
	_bool                       m_isBlended = {};

	_uint						m_iCurrentAnimIndex = {};
	_uint                       m_iNextAnimIndex = {};	  /* 보간용 변수들 */

	_float  m_fBlendDuration = {};
	_float	m_fBlendTickPerSecond = {};

private:
	_float3 m_vPreRootPosition = {};
	_bool   m_isRoot = {};

private:/* 애니메이션 갯수, 애니메이션을 담는 벡터 */
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
