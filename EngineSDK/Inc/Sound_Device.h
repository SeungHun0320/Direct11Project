#pragma once
#include "Base.h"

#include "Fmod/fmod.hpp"
#include "Fmod/fmod_studio.hpp"

BEGIN(Engine)

class CSound_Device : public CBase
{
private:
    CSound_Device();
    virtual ~CSound_Device() = default;

public:
    HRESULT Initialize();
    void Update();

    // �����Ͷ�? �Ҹ��� �Ѱ�ġ�� �Ѿ �Ҹ��� �� ������ �������ִ� ����Ʈ
    void Set_Master_Limiter();
    // ���� ��Ʈ�ѷ��� �ε� ���� �Լ�
    HRESULT LoadSound(const string& Path, _bool is3D, _bool loop, _bool stream, unordered_map<string, class CSound_Core*>* _Out_ pOut = nullptr);
    void Set_Listener_Position(_fmatrix Matrix, _fvector vel);
    class CSound_Core* Get_Single_Sound(const string& strKey);
    void Set_Master_Volume(_float volume);
private:
    FMOD::DSP* m_pMasterLimiter = nullptr;
    FMOD::System* m_pCoreSystem = nullptr;
    FMOD_3D_ATTRIBUTES m_ListenerAttr{};
    unordered_map<string, class CSound_Core*> m_SingleSounds;

public:
    static CSound_Device* Create();
    virtual void Free();
};

END
