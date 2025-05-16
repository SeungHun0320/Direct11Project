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

public:
	HRESULT Initialize(const BONE* pDesc);
	void Update_TransformationMatrix(_fmatrix TransformationMartix);
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
	virtual void Free() override;
};

END