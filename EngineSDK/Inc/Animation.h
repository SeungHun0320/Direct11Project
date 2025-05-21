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
	/* 전체 재생 거리. */
	_float					m_fDuration = {};
	_float					m_fTickPerSecond = {};
	_float					m_fCurrentTrackPosition = {};
	vector<_uint>			m_CurrentKeyFrameIndices;

	/* 이 애니메이션을 표현하기위해서 사용하는 뼈의 갯수 */
	_uint					m_iNumChannels = {};
	vector<class CChannel*>	m_Channels;

public:
	static CAnimation* Create(const ANIMATION* pDesc, ifstream& _InFile, const vector<class CBone*>& Bones);
	CAnimation* Clone();
	virtual void Free();
};

END