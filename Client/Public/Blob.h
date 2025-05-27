#pragma once

#include "Monster.h"

BEGIN(Client)

class CBlob final : public CMonster
{
public:
	typedef struct tagBlobDesc : public CMonster::DESC
	{

	}DESC;

public:
	enum PART { PART_BODY, PART_EFFECT, PART_END };

	enum ANIM_TYPE { IDLE, ATTACK, JUMP, ANIM_END };
	enum class STATES { IDLE, ATTACK, JUMP, STATES_END };

private:
	CBlob(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBlob(const CBlob& Prototype);
	virtual ~CBlob() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual LIFE Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public: /* 상태패턴 관련 함수들 */
	void Change_States(STATES eStates);

public: /* 상태로 넘겨줄 함수들 */
	/* 애니메이션 관련 */
	_bool Play_Animation(PART ePart, _float fTimeDelta);
	void  Change_Animation(PART ePart, _uint iNextIndex, _bool isLoop = true, _float fBlendDuration = 0.f, _bool isBlend = true);

private: /* 상태 패턴 변수들 */
	STATES m_eCurState{ STATES::STATES_END };
	STATES m_ePreState{ STATES::STATES_END };
	class CBlobState* m_pCurState = { nullptr };
	class CBlobState* m_pStates[ENUM_CLASS(STATES::STATES_END)] = { nullptr };

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;
	virtual HRESULT Ready_States() override;

public:
	static CBlob* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END
