#pragma once

#include "Monster.h"

BEGIN(Client)

class CBoss abstract : public CMonster
{
public:
	typedef struct tagBossDesc : CMonster::DESC
	{
		
	}DESC;
protected:
	CBoss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss(const CBoss& Prototype);
	virtual ~CBoss() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END