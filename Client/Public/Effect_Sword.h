#pragma once

#include "Client_Defines.h"
#include "EffectContainerPart.h"

BEGIN(Client)

class CEffect_Sword final : public CEffectContainerPart
{
public:
	enum PART { PART_SPLASH, PART_LINE, PART_TRAIL, PART_END };

public:
	typedef struct tagParticlePotionDesc : public CEffectContainerPart::DESC
	{
		WEAPON_TYPE* pParentWeaponType{ nullptr };
	}DESC;

private:
	CEffect_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Sword(const CEffect_Sword& Prototype);
	virtual ~CEffect_Sword() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	WEAPON_TYPE* m_pParentWeaponType = { nullptr };

public:
	void Effect_Reset(_uint iPart);

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;

public:
	static CEffect_Sword* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END