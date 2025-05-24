#pragma once
#include "Base.h"

/* 특정 애니메이션을 구동하기위해서 필요한, 뼈의 시간당 상태정보*/
/* 특정 애니메이션을 구동하기위해서 필요한, 뼈의 시간당 상태정보*/
/* 특정 애니메이션을 구동하기위해서 필요한, 뼈의 시간당 상태정보*/
/* 특정 애니메이션을 구동하기위해서 필요한, 뼈의 시간당 상태정보*/
/* 특정 애니메이션을 구동하기위해서 필요한, 뼈의 시간당 상태정보*/

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
	/* 키 프레임의 갯수 */
	_uint				m_iNumKeyFrames = {};
	/* 키 프레임 */
	/* 크, 자, 이, 상태를 취해야하는 재생 위치를 구조체로 들고 있음 */
	vector<KEYFRAME>	m_KeyFrames;

	/* 채널이 영향을 줄 뼈 인덱스 */
	_uint				m_iBoneIndex = {};

public:
	static CChannel* Create(const CHANNEL* pDesc, const vector<class CBone*>& Bones);
	virtual void Free() override;
};

END