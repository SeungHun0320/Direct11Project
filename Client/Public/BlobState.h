#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CBlobState abstract : public CBase
{
public:
	CBlobState(class CBlob* pOwner)
		: m_pOwner{ pOwner } { }
	virtual ~CBlobState() = default;

public:
	/* 상태 진입 시 호출 ( 이니셜라이즈 ) */
	virtual void Enter(_float fTimeDelta) PURE;

	/* 해당 상태일 때 프레임마다 호출 ( 업데이트 ) */
	virtual void Execute(_float fTimeDelta) PURE;

	/* 상태 퇴장시에 ( 종료시에 한 번 호출 ) */
	virtual void Exit() PURE;

protected:
	class CBlob* m_pOwner = { nullptr };
	_float   m_fTimeAcc = {};
	_float   m_fDuration = {};

public:
	virtual void Free() override {
		__super::Free();
	}
};

class CBlobState_Idle final : public CBlobState
{
public:
	CBlobState_Idle(class CBlob* pOwner);
	virtual ~CBlobState_Idle() = default;

public: // CBlobState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

private:
	_float m_fCheckTime = {};

public:
	virtual void Free() override;
};

class CBlobState_Attack final : public CBlobState
{
public:
	CBlobState_Attack(class CBlob* pOwner);
	virtual ~CBlobState_Attack() = default;

public: // CBlobState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

private:
	_uint m_iAttackCount = {};

public:
	virtual void Free() override;
};

class CBlobState_Jump final : public CBlobState
{
public:
	CBlobState_Jump(class CBlob* pOwner);
	virtual ~CBlobState_Jump() = default;

public: // CBlobState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

class CBlobState_Hit final : public CBlobState
{
public:
	CBlobState_Hit(class CBlob* pOwner);
	virtual ~CBlobState_Hit() = default;

public: // CBlobState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

END