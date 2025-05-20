#include "Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initialize(const ANIMATION* pDesc, ifstream& _InFile, const vector<class CBone*>& Bones)
{
    m_iNumChannels = pDesc->iNumChannels;

    m_fTickPerSecond = pDesc->fTickPerSecond;
    m_fDuration = pDesc->fDuration;

    for (_uint i = 0; i < m_iNumChannels; i++)
    {
        CChannel::CHANNEL tDesc{};
        _InFile.read(reinterpret_cast<_char*>(&tDesc.iNumKeyFrames), sizeof(_uint));
        _InFile.read(reinterpret_cast<_char*>(&tDesc.iBoneIndex), sizeof(_uint));
        for (_uint j = 0; j < tDesc.iNumKeyFrames; j++)
        {
            KEYFRAME KeyFrame{};

            _InFile.read(reinterpret_cast<_char*>(&KeyFrame.vScale), sizeof(_float3));
            _InFile.read(reinterpret_cast<_char*>(&KeyFrame.vRotation), sizeof(_float4));
            _InFile.read(reinterpret_cast<_char*>(&KeyFrame.vTranslation), sizeof(_float3));
            _InFile.read(reinterpret_cast<_char*>(&KeyFrame.fTrackPosition), sizeof(_float));


            tDesc.KeyFrames.push_back(KeyFrame);
        }

        CChannel* pChannel = CChannel::Create(&tDesc, Bones);
        if (nullptr == pChannel)
            return E_FAIL;

        m_Channels.push_back(pChannel);
    }

	return S_OK;
}

void CAnimation::Update_Bones(_float fTimeDelta, const vector<CBone*>& Bones, _bool isLoop)
{
    m_fCurrentTrackPosition += m_fTickPerSecond * fTimeDelta;

    if (m_fCurrentTrackPosition >= m_fDuration)
    {
        m_fCurrentTrackPosition = m_fDuration;

        if (true == isLoop)
            m_fCurrentTrackPosition = 0.f;
    }

    for (auto& pChannel : m_Channels)
    {
        pChannel->Update_TransformationMatrix(m_fCurrentTrackPosition, Bones);
    }
}

CAnimation* CAnimation::Create(const ANIMATION* pDesc, ifstream& _InFile, const vector<class CBone*>& Bones)
{
    CAnimation* pInstance = new CAnimation();

    if (FAILED(pInstance->Initialize(pDesc, _InFile, Bones)))
    {
        MSG_BOX("Failed to Created : CMaterial");
        Safe_Release(pInstance);
    }

    return pInstance;
}
void CAnimation::Free()
{
	__super::Free();

    for (auto& pChannel : m_Channels)
        Safe_Release(pChannel);

    m_Channels.clear();
}
