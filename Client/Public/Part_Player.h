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

class CPart_Player abstract : public CPartObject
{
public:
	typedef struct tagPartPlayerDesc : public CPartObject::DESC
	{
		LEVEL eLevelID;
		CPlayer::STATES* pParentState = { nullptr };
	}DESC;

protected:
	CPart_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPart_Player(const CPart_Player& Prototype);
	virtual ~CPart_Player() = default;

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
	virtual void Change_Animation(_uint iNextIndex, _bool isLoop, _float fBlendDuration, _bool isBlend) override;
	virtual void Set_MeshVisible(_uint iIndex, _bool IsVisible) override;
	virtual void Set_TrackPosition(_float fTrackPosition) override;
	virtual _bool Is_CurrentAnim(_uint iNextIndex) override;

public:
	virtual void Set_Active(_bool isActive);
	virtual CCollider* Get_Collider(_uint iColliderIndex = 0) const override;


protected:
	CCollider* m_pColliderCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

	CPlayer::STATES* m_pParentState = { nullptr };

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