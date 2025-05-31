#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

#include "Wizard_Candleabra.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CBody_WizardCandleabra final : public CPartObject
{
public:
	typedef struct tagBodyCandleabraDesc : public CPartObject::DESC
	{
		LEVEL eLevelID;
	}DESC;
private:
	CBody_WizardCandleabra(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_WizardCandleabra(const CBody_WizardCandleabra& Prototype);
	virtual ~CBody_WizardCandleabra() = default;

public:
	const _float4x4* Get_SocketMatrix(const _string& strBoneName);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual _bool Play_Animation(_float fTimeDelta) override;
	virtual void Change_Animation(_uint iNextIndex, _bool isLoop = true, _float fBlendDuration = 0.f, _bool isBlend = true) override;
	virtual void  Set_TrackPosition(_float fTrackPosition) override;

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

	CWizard_Candleabra::STATES* m_pParentState = { nullptr };

private:
	LEVEL m_eLevelID = { LEVEL::LEVEL_END };
	_int m_iSoonseo = {};

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	static CBody_WizardCandleabra* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END