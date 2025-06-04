#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

class CBody_SpiderTank final : public CPartObject
{
public:
	typedef struct tagBodySpiderTankDesc : public CPartObject::DESC
	{
		LEVEL eLevelID;
	}DESC;

	enum COL_TYPE { HEAD, WEAK, COL_END };
private:
	CBody_SpiderTank(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_SpiderTank(const CBody_SpiderTank& Prototype);
	virtual ~CBody_SpiderTank() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual _bool Is_CurrentAnim(_uint iNextIndex) override;
	virtual _bool Play_Animation(_float fTimeDelta) override;
	virtual void  Change_Animation(_uint iNextIndex, _bool isLoop = true, _float fBlendDuration = 0.f, _bool isBlend = true) override;
	virtual void  Set_TrackPosition(_float fTrackPosition) override;
	virtual void  Set_TickPerSecond(_float fTickPerSecond) override;

private:
	CCollider* m_pColliderCom[COL_END] = {nullptr};
	CShader*   m_pShaderCom = { nullptr };
	CModel*    m_pModelCom = { nullptr };

private:
	const _float4x4* m_pColMatrix[COL_END] = {nullptr};

private:
	LEVEL m_eLevelID = { LEVEL::LEVEL_END };

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	static CBody_SpiderTank* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

