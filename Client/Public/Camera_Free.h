#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Free : public CCamera
{
public:
	typedef struct tagCameraFreeDesc : public CCamera::DESC
	{
		_float fSensor;
		LEVEL  eLevelID;
	}DESC;
private:
	CCamera_Free(ID3D11Device* pDeivce, ID3D11DeviceContext* pContext);
	CCamera_Free(const CCamera_Free& Prototype);
	~CCamera_Free() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	void Key_Input(_float fTimeDelta);

private:
	_float				m_fSensor{};
	LEVEL				m_eLevelID{LEVEL::LEVEL_END};

public:
	static CCamera_Free* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;
};

END