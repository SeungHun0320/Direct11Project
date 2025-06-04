#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

#include "Blob.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

BEGIN(Client)

class CBody_Blob final : public CPartObject
{
public:
	typedef struct tagBodyBlobDesc : public CPartObject::DESC
	{
		LEVEL eLevelID;
	}DESC;
private:
	CBody_Blob(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Blob(const CBody_Blob& Prototype);
	virtual ~CBody_Blob() = default;

public:
	const _float4x4* Get_SocketMatrix(const _string& strBoneName);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual _bool Play_Animation(_float fTimeDelta) override;
	virtual void Change_Animation(_uint iNextIndex, _bool isLoop = true, _float fBlendDuration = 0.f, _bool isBlend = true) override;
	virtual void  Set_TrackPosition(_float fTrackPosition) override;

private:
	CCollider* m_pColliderCom = { nullptr };
	CShader*   m_pShaderCom = { nullptr };
	CModel*	   m_pModelCom = { nullptr };

	CBlob::STATES* m_pParentState = { nullptr };

private:
	LEVEL m_eLevelID = { LEVEL::LEVEL_END };
	_int m_iSoonseo = {};

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	static CBody_Blob* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END