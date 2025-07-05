#pragma once

#include "Client_Defines.h"
#include "EffectContainerPart.h"

BEGIN(Client)

class CEffect_Dagger final : public CEffectContainerPart
{
public:
	enum PART { PART_ICE, PART_SWIRL, PART_STARDUST, PART_END };

public:
	typedef struct tagParticlePotionDesc : public CEffectContainerPart::DESC
	{
		WEAPON_TYPE* pParentWeaponType{ nullptr };
		const _bool* pParentisDaggerAttack{ nullptr };
	}DESC;

private:
	CEffect_Dagger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Dagger(const CEffect_Dagger& Prototype);
	virtual ~CEffect_Dagger() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	WEAPON_TYPE*			m_pParentWeaponType = { nullptr };
	const _bool*			m_pParentisDaggerAttack = { nullptr };

private:
	_bool					m_isUpdate = { false };

private:
	void Reset_Effect(_uint iPart);
	void Reset_Animation(_uint iPart);
	void Set_MeshVisible(_uint iPart, _bool IsVisible = true);

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;

public:
	static CEffect_Dagger* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END