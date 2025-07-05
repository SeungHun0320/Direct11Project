#pragma once

#include "Client_Defines.h"
#include "EffectContainerPart.h"

BEGIN(Client)

class CEffect_Hit final : public CEffectContainerPart
{
public:
	enum PART { PART_SPLASH, PART_LINE, PART_END };

public:
	typedef struct tagEffectHitDesc : public CEffectContainerPart::DESC
	{

	}DESC;

private:
	CEffect_Hit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Hit(const CEffect_Hit& Prototype);
	virtual ~CEffect_Hit() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	void Effect_Reset(_uint iPart);

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;

public:
	static CEffect_Hit* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END