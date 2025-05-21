#pragma once

#include "Base.h"

BEGIN(Engine)

class CAnimation final : public CBase
{
	
public:
	typedef struct tagAnimationDesc
	{
		_float fDuration{};
		_float fTickPerSecond{};
		_float fCurrentTrackPosition{};

		_uint iNumChannels{};

	}ANIMATION;

private:
	CAnimation();
	CAnimation(const CAnimation& Prototype);
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize(const ANIMATION* pDesc, ifstream& _InFile, const vector<class CBone*>& Bones);
	_bool Update_Bones(_float fTimeDelta, const vector<CBone*>& Bones, _bool isLoop);
private:
	/* ��ü ��� �Ÿ�. */
	_float					m_fDuration = {};
	_float					m_fTickPerSecond = {};
	_float					m_fCurrentTrackPosition = {};
	vector<_uint>			m_CurrentKeyFrameIndices;

	/* �� �ִϸ��̼��� ǥ���ϱ����ؼ� ����ϴ� ���� ���� */
	_uint					m_iNumChannels = {};
	vector<class CChannel*>	m_Channels;

public:
	static CAnimation* Create(const ANIMATION* pDesc, ifstream& _InFile, const vector<class CBone*>& Bones);
	CAnimation* Clone();
	virtual void Free();
};

END