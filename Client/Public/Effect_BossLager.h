#pragma once

#include "EffectContainerPart.h"

class CEffect_BossLager final : public CEffectContainerPart
{
public:
	enum PART { PART_LAGER, PART_END };

public:
	typedef struct tagEffectBossLagerDesc : public CEffectContainerPart::DESC
	{
		const _bool* pParnetisUseLager{ nullptr };
	}DESC;

private:
	CEffect_BossLager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_BossLager(const CEffect_BossLager& Prototype);
	virtual ~CEffect_BossLager() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	const _bool* m_pParnetisUseLager = { nullptr };

private:
	void Set_MeshVisible(_uint iPart, _bool isVisible = true);
	void TurnZ(_uint iPart, _float fTimeDelta);
	void Scaling(_uint iPart, const _float3& vScale);


private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;

public:
	static CEffect_BossLager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

