#pragma once

#include "Monster.h"

BEGIN(Client)

class CBlob final : public CMonster
{
public:
	typedef struct tagBlobDesc : public CMonster::DESC
	{

	}DESC;

private:
	CBlob(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBlob(const CBlob& Prototype);
	virtual ~CBlob() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CBlob* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END
