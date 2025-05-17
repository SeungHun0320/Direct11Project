#pragma once

#include "Map.h"

BEGIN(Client)

class CMain final : public CMap
{
public:
	typedef struct tagMainDesc : public CMap::DESC
	{

	}DESC;
private:
	CMain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMain(const CMain& Prototype);
	virtual ~CMain() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components(void* pArg);

public:
	static CMain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free()override;
};

END