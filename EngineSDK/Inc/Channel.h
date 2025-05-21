#pragma once
#include "Base.h"

/* Ư�� �ִϸ��̼��� �����ϱ����ؼ� �ʿ���, ���� �ð��� ��������*/

BEGIN(Engine)

class CChannel final : public CBase
{
public:
	typedef struct tagChanneltag
	{
		_uint				iNumKeyFrames{};
		_uint				iBoneIndex{};

		vector<KEYFRAME>	KeyFrames;
	}CHANNEL;
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(const CHANNEL* pDesc, const vector<class CBone*>& Bones);
	void Update_TransformationMatrix(_uint* pCurrentKeyFrameIndex, _float fCurrentTrackPosition, const vector<class CBone*>& Bones);

private:
	_uint				m_iNumKeyFrames = {};
	vector<KEYFRAME>	m_KeyFrames;

	_uint				m_iBoneIndex = {};

public:
	static CChannel* Create(const CHANNEL* pDesc, const vector<class CBone*>& Bones);
	virtual void Free() override;
};

END