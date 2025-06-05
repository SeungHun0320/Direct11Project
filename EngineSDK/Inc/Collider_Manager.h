#pragma once

#include "Base.h"

BEGIN(Engine)

class CCollider_Manager final : public CBase
{
private:
	CCollider_Manager();
	virtual ~CCollider_Manager() = default;
	
public: /* 레벨 넘어갈 때 호출하면*/
	void Clear();

public:
	HRESULT Initialize(_uint iNumGroups);
	/* 콜라이더 생성하자마자 넣어버리자 */
	HRESULT Add_Collider(class CCollider* pCollider, _uint iColliderGroupID);

	void    Intersect(_uint iColliderGroupID1, _uint iColliderGroupID2);

private:
	_uint					m_iNumGroups = {};
	list<class CCollider*>* m_pColliders = { nullptr };

public:
	static CCollider_Manager* Create(_uint iNumGroups);
	virtual void Free() override;
};

END