#pragma once

#include "Base.h"

/* ��. (aiNode, aiBone, aiNodeAnim) */
/* aiNode�� �̿��ؼ� ������ �� ����. */

BEGIN(Engine)

class CBone final : public CBase
{
public:
	typedef struct tagBoneDesc
	{
		_string strName;
		XMFLOAT4X4 TransformationMatrix;
		_int iParentBoneIndex;

	}BONE;
private:
	CBone();
	virtual ~CBone() = default;

public:
	const _float4x4* Get_CombinedTransformationMatrix() const {
		return &m_CombinedTransformationMatrix;
	}

	const _float4x4* Get_TransformationMatrix() const {
		return &m_TransformationMatrix;
	}

	const _uint Get_ParentBoneIndex() {
		return m_iParentBoneIndex;
	}

	void Set_ParentBoneIndex(_uint iParentBoneIndex = 2) {
		m_iParentBoneIndex = iParentBoneIndex;
	}

	_bool Compare_Name(const _string& strName) {
		return m_strName == strName;
	}

	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}

public:
	HRESULT Initialize(const BONE* pDesc);
	void Update_CombinedTransformationMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix);

private:
	_string m_strName;

	/* (A : �� �� ��ü�� �������� ��ȯ������ ǥ���� ���) */
	_float4x4				m_TransformationMatrix = {};

	/* (A : �� �� ��ü�� �������� ��ȯ������ ǥ���� ���) * �θ����. = A: �θ� �������� ȸ���Ѵ�. */
	_float4x4				m_CombinedTransformationMatrix = {};

	_int					m_iParentBoneIndex = { -1 };

public:
	static CBone* Create(const BONE* pDesc);
	CBone* Clone();
	virtual void Free() override;
};

END