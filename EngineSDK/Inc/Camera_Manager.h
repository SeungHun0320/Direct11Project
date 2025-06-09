#pragma once

#include "Base.h"
#include "Camera.h"

BEGIN(Engine)

class CCamera_Manager final :public CBase
{
private:
	CCamera_Manager();
	virtual ~CCamera_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	HRESULT Add_Camera(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strCameraTag);
	void    Clear(_uint iLevelIndex);

public:
	void    Set_CameraMode(_uint iLevelIndex, const _wstring& strCameraTag, _uint iModeIndex);
	_vector Get_CameraState(_uint iLevelIndex, const _wstring& strCameraTag, STATE eState);

public:
	void Shake_Camera(_uint iLevelIndex, const _wstring& strCameraTag, _float fIntensity = 1.f, _float fDuration = 1.f, _float fShakeFreqPos = 100.f, _float fShakeFreqRot = 40.f);

public:
	class CCamera* Find_Camera(_uint iLevelIndex, const _wstring& strCameraTag);

private:
	map<const _wstring, class CCamera*>* m_pCameras;
	typedef map<const _wstring, class CCamera*>  CAMERAS;

private:
	_uint					m_iNumLevels = {};

private:
	class CGameInstance* m_pGameInstance = { nullptr };

public:
	static CCamera_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END