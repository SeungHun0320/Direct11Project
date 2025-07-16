#pragma once

#include "Monster.h"

BEGIN(Client)

class CMerchant final : public CMonster
{
public:
	enum PART { PART_BODY, PART_END };
	enum ANIM_TYPE { HIDDEN, WAKE_UP, IDLE, THANK_YOU, BLINK, ANIM_END };
	enum class STATES { HIDDEN, WAKE_UP, IDLE, THANK_YOU, STATES_END };

public:
	typedef struct tagMerchantDesc : public CMonster::DESC
	{

	}DESC;

protected:
	CMerchant(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMerchant(const CMerchant& Prototype);
	virtual ~CMerchant() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: /* 상태패턴 관련 함수들 */
	void Change_States(STATES eStates);
	_vector Get_State(STATE eState);

public:
	const _bool Get_isSell() const { return m_isSell; }
	void Set_isSell(_bool isSell) { m_isSell = isSell; }

public: /* 상태로 넘겨줄 함수들 */
	/* 애니메이션 관련 */
	_bool Play_Animation(PART ePart, _float fTimeDelta);
	void  Change_Animation(PART ePart, _uint iNextIndex, _bool isLoop = true, _float fBlendDuration = 0.f, _bool isBlend = true);

public:
	/* 이동 관련 */
	void Go_Left(_float fTimeDelta, _float fSpeed);
	void Go_Right(_float fTimeDelta, _float fSpeed);
	void Go_Up(_float fTimeDelta, _float fSpeed);

	_bool is_TargetOnRight();

private:
	STATES m_eCurState{ STATES::STATES_END };
	STATES m_ePreState{ STATES::STATES_END };
	class CMerchantState* m_pCurState = { nullptr };
	class CMerchantState* m_pStates[ENUM_CLASS(STATES::STATES_END)] = { nullptr };

private:
	_bool m_isSell = { false };

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;
	virtual HRESULT Ready_States() override;

public:
	static  CMerchant* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END