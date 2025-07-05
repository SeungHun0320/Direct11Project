#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"

BEGIN(Engine)
class CCollider;
class CNavigation;
END

BEGIN(Client)

class CBaseActor abstract : public CContainerObject
{
public:
	typedef struct tagPawnDesc : public CContainerObject::DESC
	{
		LEVEL eLevelID;
	}DESC;
protected:
	CBaseActor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBaseActor(const CBaseActor& Prototype);
	virtual ~CBaseActor() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: /* 공격력, 그로기 수치 */
	_float Get_AttackValue() const {
		return m_fAttack;
	}

public: /* 피격 관련 */
	_bool Get_IsHit() const {
		return m_isHit;
	}
	void Reset_IsHit() {
		m_isHit = false;
	}

	_float Get_StaggerValue() const {
		return m_fStaggerValue;
	}

	_bool Get_IsStagger() const {
		return m_isStagger;
	}
	void Set_IsStagger(_bool isStagger = false) { m_isStagger = isStagger; }

public: /* 충돌 관련 */
	_bool Get_IsBlocked() const {
		return m_isBlocked;
	}
	
protected:
	CNavigation* m_pNavigationCom = { nullptr };

protected:
	LEVEL m_eLevelID = { LEVEL::LEVEL_END };

protected: /* 피격 관련 */
	_bool  m_isHit = { false };
	_bool  m_isInvincible = { false };
	_float m_fInvicibleTime = {};

protected: /* 체력 */
	_float m_fHp = {};
	_float m_fMaxHp = {};

protected: /* 공격력 */
	_float m_fAttack = {};
	_float m_fStaggerValue = {};

protected: /* 그로기 */
	_float m_fStaggerGage = {};
	_float m_fMaxStaggerGage = {};
	_bool  m_isStagger = { false };

protected: /* 충돌 관련 */
	_bool  m_isBlocked = { false };

protected:
	void Update_InvicibleTime(_float fTimeDelta);
	void Start_Invicible(_float fDuration);
	virtual void On_Hit(_float fDamage, _float fStaggerValue, _float fInvicibleDuration) {};
	virtual _float Compute_InvincibleTime(COLLIDER_ID eColliderID) { return 0.f; };

protected:
	virtual HRESULT Ready_Components(void* pArg);
	virtual HRESULT Ready_PartObjects() { return S_OK; };


public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;

};

END