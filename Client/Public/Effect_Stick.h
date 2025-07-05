#pragma once

#include "Client_Defines.h"
#include "EffectContainerPart.h"

BEGIN(Client)

class CEffect_Stick final : public CEffectContainerPart
{
public:
	enum PART { PART_LINE, PART_CONFETTI, PART_HEALTH_SPELL, PART_END };

public:
	typedef struct tagParticlePotionDesc : public CEffectContainerPart::DESC
	{
		WEAPON_TYPE* pParentWeaponType{ nullptr };
		const _float4x4* pSocketMatrix{ nullptr };
	}DESC;

private:
	CEffect_Stick(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Stick(const CEffect_Stick& Prototype);
	virtual ~CEffect_Stick() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	WEAPON_TYPE* m_pParentWeaponType = { nullptr };
	const _float4x4* m_pSocketMatrix = { nullptr };

private:
	void Effect_Reset(_uint iPart);

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;

public:
	static CEffect_Stick* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END