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
	/* ��ü ��� �Ÿ�. */
	_float					m_fDuration = {};
	/* ��� �ӵ�*/
	_float					m_fTickPerSecond = {};
	/* ���� ����ǰ� �ִ� ��ġ */
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