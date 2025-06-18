#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_TPS : public CCamera
{
public:
	typedef struct tagCameraTPSDesc : public CCamera::DESC
	{
		_float3 vOffset{};
		_float fSensor{};
		_float fDeadZoneX{};
		_float fDeadZoneZ{};
		LEVEL  eLevelID{};
		class CGameObject* pTarget{ nullptr };
	}DESC;
private:
	CCamera_TPS(ID3D11Device* pDeivce, ID3D11DeviceContext* pContext);
	CCamera_TPS(const CCamera_TPS& Prototype);
	~CCamera_TPS() = default;

public:
	CAM_MODE Get_CamMode() const {
		return m_eMode;
	}

	virtual void Set_Mode(_uint iModeIndex) override {
		m_eMode = static_cast<CAM_MODE>(iModeIndex);
	}

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Clear_BossRef();
	void Add_BossRef(_wstring strBossName);

private:
	void Update_LockOnCamera(_float fTimeDelta);
	void Update_BossCamera(_float fTimeDelta);

private:
	HRESULT Ready_For_BossCamera(const _wstring& strBossName);

private:
	CAM_MODE  m_eMode = { CAM_MODE::CAM_END };

private: /* 일반 TPS 카메라 */
	LEVEL		m_eLevelID = { LEVEL::LEVEL_END };

	_float		m_fDeadZoneX = {};
	_float		m_fDeadZoneZ = {};
	_float3     m_vOffset = {};
	_float	    m_fSensor = {};
	_float		m_fAngle = {50.f};
	
	_float3 m_vCurrentFocusPos = {};
	_float3 m_vTargetFocusPos = {};

private: /* 보스 카메라용으로 */
	_float m_fBossAngle = {};
	_bool  m_bBossMode = { false };
	_bool  m_bAdjustableCamDir = { false };


private: /* 일반 카메라 */
	CGameObject*  m_pTarget = { nullptr };
	CTransform*   m_pTargetTransformCom = { nullptr };

private:
	CGameObject*  m_pBoss = { nullptr };
	CTransform*	  m_pBossTransformCom = { nullptr };

public:
	static CCamera_TPS* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;
};

END