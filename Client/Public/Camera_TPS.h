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
		LEVEL  eLevelID{};
		class CGameObject* pTarget{ nullptr };
	}DESC;
private:
	CCamera_TPS(ID3D11Device* pDeivce, ID3D11DeviceContext* pContext);
	CCamera_TPS(const CCamera_TPS& Prototype);
	~CCamera_TPS() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	_float3             m_vOffset = {};
	_float				m_fSensor{};
	LEVEL				m_eLevelID{ LEVEL::LEVEL_END };

private:
	class CGameObject*  m_pTarget = { nullptr };

public:
	static CCamera_TPS* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;
};

END