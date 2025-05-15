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
	HRESULT Initialize(const BONE* pDesc, _int iParentBoneIndex);
	void Update_TransformationMatrix(_fmatrix TransformationMartix);
	void Update_CombinedTransformationMatrix(const vector<CBone*>& Bones);

private:
	_string m_strName;

	/* (A : �� �� ��ü�� �������� ��ȯ������ ǥ���� ���) */
	_float4x4				m_TransformationMatrix = {};

	/* (A : �� �� ��ü�� �������� ��ȯ������ ǥ���� ���) * �θ����. = A: �θ� �������� ȸ���Ѵ�. */
	_float4x4				m_CombinedTransformationMatrix = {};

	_int					m_iParentBoneIndex = { -1 };

public:
	static CBone* Create(const BONE* pDesc, _int iParentBoneIndex);
	virtual void Free() override;
};

END