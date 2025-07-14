#pragma once

#include "Client_Defines.h"
#include "BaseActor.h"

BEGIN(Client)

class CMonster abstract : public CBaseActor
{
public:
	typedef struct tagMonsterDesc : CBaseActor::DESC
	{

	}DESC;
protected:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster(const CMonster& Prototype);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public: /* 락온에 잡혔을 때 유아이 렌더용 */
	virtual void Set_IsLockOnTarget(_bool IsLockOnTarget = true) {
		m_IsLockOnTarget = IsLockOnTarget;
	}

public: /* 타깃 관련 */
	virtual _bool Find_Player();

	const _bool Get_IsFind() const {
		return m_IsFind;
	}
	void Set_IsFind(_bool IsFind) {
		m_IsFind = IsFind;
	}

	const _float Get_DistanceToPlayer() const {
		return m_fDistanceToPlayer;
	}

	const _float Get_ChaseStopDistance() const {
		return m_fChaseStopDistance;
	}

	const _float Get_DetectDistance() const {
		return m_fDetectDistance;
	}

	const _float Get_PreferredDistance() const {
		return m_fPreferredDistance;
	}

	void Set_PreferrendDistance(_float fPreferredDistance) {
		m_fPreferredDistance = fPreferredDistance;
	}

	const _vector Get_TargetPosition() const;

public: /* 충돌관련 */
	virtual CCollider* Get_Collider(_uint iColliderIndex = 0);

protected: /* 맵툴에선 굳이 넣어줄 필요가 없어서 그냥 이니셜라이즈에서 때려박는게 나은거 같기도 */
	CTransform*  m_pTargetTransform = { nullptr };

protected: /* 상태 관련 변수들 */
	_float m_fDistanceToPlayer = {};
	_float m_fDetectDistance = {};
	_float m_fChaseStopDistance = {};
	_float m_fPreferredDistance = {};
	_bool  m_IsFind = {};
	_bool  m_IsLockOnTarget = {};

protected:
	virtual void On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner) override;
	HRESULT Create_HitEffect();
	HRESULT Craete_SmokeEffect();

protected:
	virtual HRESULT Ready_Components(void* pArg);
	virtual HRESULT Ready_PartObjects() { return S_OK; };
	virtual HRESULT Ready_States() { return S_OK; };

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;

};

END