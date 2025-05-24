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
	_float Get_CurrentTrackPosition() {
		return m_fCurrentTrackPosition;
	}

public:
	HRESULT Initialize(const ANIMATION* pDesc, ifstream& _InFile, const vector<class CBone*>& Bones);
	_bool Update_Bones(_float fTimeDelta, const vector<CBone*>& Bones, _bool isLoop);
	void Reset();

	void Update_TransformationMatrices(vector<_matrix>& OutTransformationMatrices, _float fTimeDelta);
	void Update_First_TransformationMatrices(vector<_matrix>& OutTransformationMatrices);
private:
	/* 전체 재생 거리. */
	_float					m_fDuration = {};
	/* 재생 속도*/
	_float					m_fTickPerSecond = {};
	/* 현재 재생되고 있는 위치 */
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