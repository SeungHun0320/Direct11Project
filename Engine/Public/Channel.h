#pragma once
#include "Base.h"

/* Ư�� �ִϸ��̼��� �����ϱ����ؼ� �ʿ���, ���� �ð��� ��������*/
/* Ư�� �ִϸ��̼��� �����ϱ����ؼ� �ʿ���, ���� �ð��� ��������*/
/* Ư�� �ִϸ��̼��� �����ϱ����ؼ� �ʿ���, ���� �ð��� ��������*/
/* Ư�� �ִϸ��̼��� �����ϱ����ؼ� �ʿ���, ���� �ð��� ��������*/
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

	void Update_TransformationMatrix(vector<_matrix>& OutTransformationMatrices, _uint* pCurrentKeyFrameIndex, _float fCurrentTrackPosition);
	void Update_First_TransformationMatrix(vector<_matrix>& OutTransformationMatrices);

private:
	/* Ű �������� ���� */
	_uint				m_iNumKeyFrames = {};
	/* Ű ������ */
	/* ũ, ��, ��, ���¸� ���ؾ��ϴ� ��� ��ġ�� ����ü�� ��� ���� */
	vector<KEYFRAME>	m_KeyFrames;

	/* ä���� ������ �� �� �ε��� */
	_uint				m_iBoneIndex = {};

public:
	static CChannel* Create(const CHANNEL* pDesc, const vector<class CBone*>& Bones);
	virtual void Free() override;
};

END