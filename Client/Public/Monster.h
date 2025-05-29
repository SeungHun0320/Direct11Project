#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"


BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CMonster abstract : public CContainerObject
{
public:
	typedef struct tagMonsterDesc : CContainerObject::DESC
	{
		LEVEL eLevelID{};
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

public:
	virtual _bool Find_Player();

public:
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

	const _vector Get_TargetPosition() const;


protected: /* 맵툴에선 굳이 넣어줄 필요가 없어서 그냥 이니셜라이즈에서 때려박는게 나은거 같기도 */
	class CGameObject* m_pTarget = { nullptr };
	CTransform*  m_pTargetTransform = { nullptr };

protected:
	LEVEL m_eLevelID = {LEVEL::LEVEL_END};

protected: /* 상태 관련 변수들 */
	_float m_fDistanceToPlayer = {};
	_float m_fDetectDistance = {};
	_float m_fChaseStopDistance = {};
	_bool  m_IsFind = {};

protected:
	virtual HRESULT Ready_Components(void* pArg);
	virtual HRESULT Ready_PartObjects() { return S_OK; };
	virtual HRESULT Ready_States() { return S_OK; };

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;

};

END