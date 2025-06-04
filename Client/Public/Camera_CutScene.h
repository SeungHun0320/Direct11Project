#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_CutScene final : public CCamera
{
public:
	typedef struct tagCameraCutSceneDesc : public CCamera::DESC
	{

	}DESC;
private:
	CCamera_CutScene(ID3D11Device* pDeivce, ID3D11DeviceContext* pContext);
	CCamera_CutScene(const CCamera_CutScene& Prototype);
	~CCamera_CutScene() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	static CCamera_CutScene* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;
};

END