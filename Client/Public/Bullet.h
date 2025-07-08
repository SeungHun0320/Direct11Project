#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollider;
class CModel;
class CShader;
class CNavigation;
END

BEGIN(Client)

class CBullet abstract : public CGameObject
{
public:
	typedef struct tagBulletDesc : public CGameObject::DESC
	{
		LEVEL eLevelID = { LEVEL::LEVEL_END };
		_wstring strPrototypeModelTag;
		_float3 vDir{};
	}DESC;

protected:
	CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBullet(const CBullet& Prototype);
	virtual ~CBullet() = default;

public:
	_float Get_AttackValue() const { return m_fAttack; }
	_float Get_StaggerValue() const { return m_fStaggerValue; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	LEVEL m_eLevelID = { LEVEL::LEVEL_END };

protected:
	_float3 m_vDir = {};

protected: /* °ø°Ý·Â */
	_float m_fAttack = {};
	_float m_fStaggerValue = {};

protected:
	_float m_fTimeAcc = {};
	_float m_fDeadTime = {};

protected:
	CCollider* m_pColliderCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

protected:
	virtual HRESULT Craete_SmokeEffect();

protected:
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner) override;

protected:
	virtual HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;

};

END