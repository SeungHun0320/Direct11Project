#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CSpiderTankState abstract : public CBase
{
public:
	CSpiderTankState(class CSpiderTank* pOwner)
		: m_pOwner{ pOwner } { }
	virtual ~CSpiderTankState() = default;

public:
	virtual void Enter(_float fTimeDelta) PURE;
	virtual void Execute(_float fTimeDelta) PURE;
	virtual void Exit() PURE;

protected:
	class CSpiderTank* m_pOwner = { nullptr };
	_float   m_fTimeAcc = {};
	_float   m_fDuration = {};

public:
	virtual void Free() override {
		__super::Free();
	}
};

/* ------------------------------- 아이들 ------------------------------- */
		  
class CSpiderTankState_Sleep final : public CSpiderTankState
{
public:
	CSpiderTankState_Sleep(class CSpiderTank* pOwner);
	virtual ~CSpiderTankState_Sleep() = default;

public:	// CSpiderTankState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;

};

class CSpiderTankState_WakeUp final : public CSpiderTankState
{
public:
	CSpiderTankState_WakeUp(class CSpiderTank* pOwner);
	virtual ~CSpiderTankState_WakeUp() = default;

public: // CSpiderTankState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;

};

class CSpiderTankState_Idle final : public CSpiderTankState
{
public:
	CSpiderTankState_Idle(class CSpiderTank* pOwner);
	virtual ~CSpiderTankState_Idle() = default;

public: // CSpiderTankState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	void Decide_Rotation();

public:
	virtual void Free() override;

};


/* ------------------------------- 무브 ------------------------------- */

class CSpiderTankState_Forward final : public CSpiderTankState
{
public:
	CSpiderTankState_Forward(class CSpiderTank* pOwner);
	virtual ~CSpiderTankState_Forward() = default;

public: // CSpiderTankState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

private:
	_float3 m_vMovePos{};

public:
	virtual void Free() override;

};

class CSpiderTankState_Backward final : public CSpiderTankState
{
public:
	CSpiderTankState_Backward(class CSpiderTank* pOwner);
	virtual ~CSpiderTankState_Backward() = default;

public: // CSpiderTankState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

private:
	_float3 m_vMoveDir{};

public:
	virtual void Free() override;

};

class CSpiderTankState_TurnRight final : public CSpiderTankState
{
public:
	CSpiderTankState_TurnRight(class CSpiderTank* pOwner);
	virtual ~CSpiderTankState_TurnRight() = default;

public: // CSpiderTankState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;

};

class CSpiderTankState_TurnLeft final : public CSpiderTankState
{
public:
	CSpiderTankState_TurnLeft(class CSpiderTank* pOwner);
	virtual ~CSpiderTankState_TurnLeft() = default;

public: // CSpiderTankState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;

};

class CSpiderTankState_Reverse final : public CSpiderTankState
{
public:
	CSpiderTankState_Reverse(class CSpiderTank* pOwner);
	virtual ~CSpiderTankState_Reverse() = default;

public: // CSpiderTankState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

private:
	_float3 m_vMoveDir{};

public:
	virtual void Free() override;

};

/* ------------------------------- 어택 ------------------------------- */

/* 레이저 빔 */
class CSpiderTankState_Lager final : public CSpiderTankState
{
public:
	CSpiderTankState_Lager(class CSpiderTank* pOwner);
	virtual ~CSpiderTankState_Lager() = default;

public: // CSpiderTankState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;

};

/* 쫄몹 소환(쫄면 먹고싶다) */
class CSpiderTankState_SpawnMob final : public CSpiderTankState
{
public:
	CSpiderTankState_SpawnMob(class CSpiderTank* pOwner);
	virtual ~CSpiderTankState_SpawnMob() = default;

public: // CSpiderTankState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;

};

/* 빠른 공격 ( 좌, 중간, 우 ) */
class CSpiderTankState_FastAttack final : public CSpiderTankState
{
public:
	CSpiderTankState_FastAttack(class CSpiderTank* pOwner);
	virtual ~CSpiderTankState_FastAttack() = default;

public: // CSpiderTankState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;

};

/* 휘두르기 ( 좌, 우 )*/
class CSpiderTankState_Swing final : public CSpiderTankState
{
public:
	CSpiderTankState_Swing(class CSpiderTank* pOwner);
	virtual ~CSpiderTankState_Swing() = default;

public: // CSpiderTankState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;

};

/* 짱 센 휘두르기 */
class CSpiderTankState_FullSwing final : public CSpiderTankState
{
public:
	CSpiderTankState_FullSwing(class CSpiderTank* pOwner);
	virtual ~CSpiderTankState_FullSwing() = default;

public: // CSpiderTankState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

private:
	_bool m_bBlendStarted = {false};
	_float3 m_vMoveDir{};

public:
	virtual void Free() override;

};

/* 총알 발사 준비 */
class CSpiderTankState_ReadyShot final : public CSpiderTankState
{
public:
	CSpiderTankState_ReadyShot(class CSpiderTank* pOwner);
	virtual ~CSpiderTankState_ReadyShot() = default;

public: // CSpiderTankState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;

};

/* 총알 발사 */
class CSpiderTankState_Shot final : public CSpiderTankState
{
public:
	CSpiderTankState_Shot(class CSpiderTank* pOwner);
	virtual ~CSpiderTankState_Shot() = default;

public: // CSpiderTankState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;

};

/* 총알 발사 끝 */
class CSpiderTankState_EndShot final : public CSpiderTankState
{
public:
	CSpiderTankState_EndShot(class CSpiderTank* pOwner);
	virtual ~CSpiderTankState_EndShot() = default;

public: // CSpiderTankState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;

};

/* 포탄 발사 준비 */
class CSpiderTankState_ReadyBomb final : public CSpiderTankState
{
public:
	CSpiderTankState_ReadyBomb(class CSpiderTank* pOwner);
	virtual ~CSpiderTankState_ReadyBomb() = default;

public: // CSpiderTankState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;

};

/* 포탄 발사 */
class CSpiderTankState_ShotBomb final : public CSpiderTankState
{
public:
	CSpiderTankState_ShotBomb(class CSpiderTank* pOwner);
	virtual ~CSpiderTankState_ShotBomb() = default;

public: // CSpiderTankState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;

};

/* 포탄 발사 끝 */
class CSpiderTankState_EndBomb final : public CSpiderTankState
{
public:
	CSpiderTankState_EndBomb(class CSpiderTank* pOwner);
	virtual ~CSpiderTankState_EndBomb() = default;

public: // CSpiderTankState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;

};

/* ------------------------------- 피격 ------------------------------- */

/* 피격 */
class CSpiderTankState_Pinch final : public CSpiderTankState
{
public:
	CSpiderTankState_Pinch(class CSpiderTank* pOwner);
	virtual ~CSpiderTankState_Pinch() = default;

public: // CSpiderTankState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;

};

/* 넉백 */
class CSpiderTankState_KnockBack final : public CSpiderTankState
{
public:
	CSpiderTankState_KnockBack(class CSpiderTank* pOwner);
	virtual ~CSpiderTankState_KnockBack() = default;

public: // CSpiderTankState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;

};

/* 사망 */
class CSpiderTankState_Dead final : public CSpiderTankState
{
public:
	CSpiderTankState_Dead(class CSpiderTank* pOwner);
	virtual ~CSpiderTankState_Dead() = default;

public: // CSpiderTankState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;

};


END