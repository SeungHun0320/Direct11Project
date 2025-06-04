#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

BEGIN(Client)

class CBody_Wizard abstract : public CPartObject
{
public:
	typedef struct tagBodySwordDesc : public CPartObject::DESC
	{
		LEVEL eLevelID;
	}DESC;

protected:
	CBody_Wizard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Wizard(const CBody_Wizard& Prototype);
	virtual ~CBody_Wizard() = default;

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

protected:
	CCollider* m_pColliderCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

protected:
	const _float4x4* m_pSocketMatrix = { nullptr };

protected:
	LEVEL m_eLevelID = { LEVEL::LEVEL_END };

protected:
	virtual HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END