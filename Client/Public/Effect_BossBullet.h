#pragma once

#include "EffectContainerPart.h"


BEGIN(Client)

class CEffect_BossBullet final : public CEffectContainerPart
{
public:
	enum PART { PART_MUZZLE, PART_PARTICLE, PART_END };

public:
	typedef struct tagEffectBossBulletDesc : public CEffectContainerPart::DESC
	{
		const _bool* pParentisShot{ nullptr };
	}DESC;

private:
	CEffect_BossBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_BossBullet(const CEffect_BossBullet& Prototype);
	virtual ~CEffect_BossBullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	void Set_MeshVisible(_uint iPart, _bool IsVisible = true);
	void TurnZ(_uint iPart, _float fTimeDelta);

private:
	const _bool* m_pParentisShot = { nullptr };

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;

public:
	static CEffect_BossBullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END