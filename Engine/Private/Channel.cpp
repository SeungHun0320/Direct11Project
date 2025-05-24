#include "Channel.h"
#include "Bone.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(const CHANNEL* pDesc, const vector<class CBone*>& Bones)
{
	m_iNumKeyFrames = pDesc->iNumKeyFrames;
	m_iBoneIndex = pDesc->iBoneIndex;

	m_KeyFrames = pDesc->KeyFrames;

	return S_OK;
}

void CChannel::Update_TransformationMatrix(_uint* pCurrentKeyFrameIndex, _float fCurrentTrackPosition, const vector<class CBone*>& Bones)
{
	/* ���� Ʈ���������� 0 �̶��, ���� Ű������ �ε����� 0 */
	if (0.0f == fCurrentTrackPosition)
		*pCurrentKeyFrameIndex = 0;

	_matrix			TransformationMatrix{};

	/* ������ Ű ������ */
	KEYFRAME		LastKeyFrame = m_KeyFrames.back();

	_vector			vScale, vRotation, vPosition;

	if (fCurrentTrackPosition >= LastKeyFrame.fTrackPosition)
	{
		vScale = XMLoadFloat3(&LastKeyFrame.vScale);
		vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);
		vPosition = XMVectorSetW(XMLoadFloat3(&LastKeyFrame.vTranslation), 1.f);
	}
	else
	{
		while (*pCurrentKeyFrameIndex + 1 < m_KeyFrames.size() &&
			fCurrentTrackPosition >= m_KeyFrames[*pCurrentKeyFrameIndex + 1].fTrackPosition)
			++*pCurrentKeyFrameIndex;
		

		_float			fRatio = (fCurrentTrackPosition - m_KeyFrames[*pCurrentKeyFrameIndex].fTrackPosition) /
			(m_KeyFrames[*pCurrentKeyFrameIndex + 1].fTrackPosition - m_KeyFrames[*pCurrentKeyFrameIndex].fTrackPosition);

		_vector			vSourScale, vDestScale;
		_vector			vSourRotation, vDestRotation;
		_vector			vSourTranslation, vDestTranslation;

		vSourScale = XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex].vScale);
		vDestScale = XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex + 1].vScale);

		vSourRotation = XMLoadFloat4(&m_KeyFrames[*pCurrentKeyFrameIndex].vRotation);
		vDestRotation = XMLoadFloat4(&m_KeyFrames[*pCurrentKeyFrameIndex + 1].vRotation);

		vSourTranslation = XMVectorSetW(XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex].vTranslation), 1.f);
		vDestTranslation = XMVectorSetW(XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex + 1].vTranslation), 1.f);

		vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
		vPosition = XMVectorLerp(vSourTranslation, vDestTranslation, fRatio);
	}

	// TransformationMatrix = XMMatrixScaling() * XMMatrixRotationQuaternion() * XMMatrixTranslation();
	TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);
}

void CChannel::Update_TransformationMatrix(vector<_matrix>& OutTransformationMatrices, _uint* pCurrentKeyFrameIndex, _float fCurrentTrackPosition)
{
	/* ���� Ʈ���������� 0 �̶��, ���� Ű������ �ε����� 0 */
	if (0.0f == fCurrentTrackPosition)
		*pCurrentKeyFrameIndex = 0;

	/* ������ Ű ������ */
	KEYFRAME		LastKeyFrame = m_KeyFrames.back();

	_vector			vScale, vRotation, vPosition;

	if (fCurrentTrackPosition >= LastKeyFrame.fTrackPosition)
	{
		vScale = XMLoadFloat3(&LastKeyFrame.vScale);
		vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);
		vPosition = XMVectorSetW(XMLoadFloat3(&LastKeyFrame.vTranslation), 1.f);
	}
	else
	{
		while (*pCurrentKeyFrameIndex + 1 < m_KeyFrames.size() &&
			fCurrentTrackPosition >= m_KeyFrames[*pCurrentKeyFrameIndex + 1].fTrackPosition)
			++*pCurrentKeyFrameIndex;

		_float			fRatio = (fCurrentTrackPosition - m_KeyFrames[*pCurrentKeyFrameIndex].fTrackPosition) /
			(m_KeyFrames[*pCurrentKeyFrameIndex + 1].fTrackPosition - m_KeyFrames[*pCurrentKeyFrameIndex].fTrackPosition);

		_vector			vSourScale, vDestScale;
		_vector			vSourRotation, vDestRotation;
		_vector			vSourTranslation, vDestTranslation;

		vSourScale = XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex].vScale);
		vDestScale = XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex + 1].vScale);

		vSourRotation = XMLoadFloat4(&m_KeyFrames[*pCurrentKeyFrameIndex].vRotation);
		vDestRotation = XMLoadFloat4(&m_KeyFrames[*pCurrentKeyFrameIndex + 1].vRotation);

		vSourTranslation = XMVectorSetW(XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex].vTranslation), 1.f);
		vDestTranslation = XMVectorSetW(XMLoadFloat3(&m_KeyFrames[*pCurrentKeyFrameIndex + 1].vTranslation), 1.f);

		vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
		vPosition = XMVectorLerp(vSourTranslation, vDestTranslation, fRatio);
	}

	_matrix TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);
	OutTransformationMatrices[m_iBoneIndex] = TransformationMatrix;

}

void CChannel::Update_First_TransformationMatrix(vector<_matrix>& OutTransformationMatrices)
{
	/* ù��° Ű ������ */
	KEYFRAME		FirstKeyFrame = m_KeyFrames.front();

	_vector			vScale, vRotation, vPosition;

	vScale = XMLoadFloat3(&FirstKeyFrame.vScale);
	vRotation = XMLoadFloat4(&FirstKeyFrame.vRotation);
	vPosition = XMVectorSetW(XMLoadFloat3(&FirstKeyFrame.vTranslation), 1.f);

	_matrix TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);
	OutTransformationMatrices[m_iBoneIndex] = TransformationMatrix;

}

CChannel* CChannel::Create(const CHANNEL* pDesc, const vector<class CBone*>& Bones)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pDesc, Bones)))
	{
		MSG_BOX("Failed to Created : CChannel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChannel::Free()
{
	__super::Free();

	m_KeyFrames.clear();
}
