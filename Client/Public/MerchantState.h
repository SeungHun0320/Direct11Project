#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CMerchantState abstract : public CBase
{
public:
	CMerchantState(class CMerchant* pOwner)
		: m_pOwner{ pOwner } {
	}
	virtual ~CMerchantState() = default;

public:
	/* 상태 진입 시 호출 ( 이니셜라이즈 ) */
	virtual void Enter(_float fTimeDelta) PURE;

	/* 해당 상태일 때 프레임마다 호출 ( 업데이트 ) */
	virtual void Execute(_float fTimeDelta) PURE;

	/* 상태 퇴장시에 ( 종료시에 한 번 호출 ) */
	virtual void Exit() PURE;

protected:
	class CMerchant* m_pOwner = { nullptr };
	_float   m_fTimeAcc = {};
	_float   m_fDuration = {};

public:
	virtual void Free() override {
		__super::Free();
	}
};

class CMerchantState_Hidden final : public CMerchantState
{
public:
	CMerchantState_Hidden(class CMerchant* pOwner);
	virtual ~CMerchantState_Hidden() = default;

public: // CMerchantState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

private:
	_float m_fCheckTime = {};

public:
	virtual void Free() override;
};

class CMerchantState_WakeUp final : public CMerchantState
{
public:
	CMerchantState_WakeUp(class CMerchant* pOwner);
	virtual ~CMerchantState_WakeUp() = default;

public: // CMerchantState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

class CMerchantState_Idle final : public CMerchantState
{
public:
	CMerchantState_Idle(class CMerchant* pOwner);
	virtual ~CMerchantState_Idle() = default;

public: // CMerchantState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

class CMerchantState_ThankYou final : public CMerchantState
{
public:
	CMerchantState_ThankYou(class CMerchant* pOwner);
	virtual ~CMerchantState_ThankYou() = default;

public: // CMerchantState을(를) 통해 상속됨
	void Enter(_float fTimeDelta) override;
	void Execute(_float fTimeDelta) override;
	void Exit() override;

public:
	virtual void Free() override;
};

END