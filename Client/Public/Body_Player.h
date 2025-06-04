#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

#include "Player.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

BEGIN(Client)

class CBody_Player final : public CPartObject
{
public:
	typedef struct tagBodyPlayerDesc : public CPartObject::DESC
	{
		LEVEL eLevelID;
		CPlayer::STATES* pParentState = { nullptr };

	}DESC;
private:
	CBody_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Player(const CBody_Player& Prototype);
	virtual ~CBody_Player() = default;

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
	virtual _bool Play_Animation(_float fTimeDelta) override ;
	virtual void Change_Animation(_uint iNextIndex, _bool isLoop, _float fBlendDuration, _bool isBlend) override;
	virtual void Set_MeshVisible(_uint iIndex, _bool IsVisible) override;
	virtual void Set_TrackPosition(_float fTrackPosition) override;
	virtual _bool Is_CurrentAnim(_uint iNextIndex) override;

private:
	CCollider*  m_pColliderCom = { nullptr };
	CShader*	m_pShaderCom = { nullptr };
	CModel*		m_pModelCom = { nullptr };

	CPlayer::STATES* m_pParentState = { nullptr };

private:
	LEVEL m_eLevelID = { LEVEL::LEVEL_END };

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	static CBody_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END