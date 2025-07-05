#pragma once

#include "Client_Defines.h"
#include "EffectContainerPart.h"

BEGIN(Client)

class CEffect_Potion final : public CEffectContainerPart
{
public:
	enum PART { PART_LINE, PART_CONFETTI, PART_HEALTH_SPELL, PART_END };

public:
	typedef struct tagParticlePotionDesc : public CEffectContainerPart::DESC
	{
		const _bool* pParentIsUsePotion{ nullptr };
	}DESC;

private:
	CEffect_Potion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Potion(const CEffect_Potion& Prototype);
	virtual ~CEffect_Potion() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	const _bool* m_pParentIsUsePotion = { nullptr };
	_bool m_isUse = { true };

private:
	void Effect_Reset(_uint iPart);
	
private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;

public:
	static CEffect_Potion* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END