#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct tagCameraDesc : public CGameObject::DESC
	{
		_float3			vEye;
		_float3			vAt;
		_float			fFov, fNear, fFar;

	}DESC;

protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& Prototype);
	~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Set_Mode(_uint iModeIndex) {};
	void Shake_Camera(_float fIntensity, _float fDuration, _float fShakeFreqPos, _float fShakeFreqRot);

protected:
	_float				m_fFov = { };
	_float				m_fNear = { };
	_float				m_fFar = { };
	_float				m_fAspect = {};

protected:
	_float m_fShakeIntensity = {};
	_float m_fShakeDuration = {};
	_float m_fShakeFreqPos = {};
	_float m_fShakeFreqRot = {};

	_float m_fShakeTime = {};
	_bool  m_isShake = TRUE;

protected:
	_float3  m_vCurrentShakePos = {};
	_float3  m_vCurrentShakeRot = {};

protected:
	_bool			m_isActive{};

protected:
	void Update_Camera_Shake(_float fTimedelta);

protected:
	void Bind_Matrices();

public:
	virtual CGameObject* Clone(void* pArg)PURE;
	virtual void Free() override;
};

END