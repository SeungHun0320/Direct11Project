#pragma once

#include "Base.h"

BEGIN(Engine)

class CCollider;

class CCollider_Manager final : public CBase
{
private:
	CCollider_Manager();
	virtual ~CCollider_Manager() = default;

public:
	HRESULT Initialize(_uint iNumGroups);
	/* 콜라이더 생성하자마자 넣어버리자 */
	HRESULT Add_Collider(class CCollider* pCollider, _uint iColliderGroupID);
	/* 호출된 그룹별로 충돌검사 */
	void    Intersect(_uint iColliderGroupID1, _uint iColliderGroupID2);
	/* 등록된 콜라이더 삭제 레벨 넘어갈 때 호출하면 됨 */
	void Clear();
	/* 오우너가 죽었을때 매니저에 등록된 콜라이더 삭제 */
	void Delete_Collider(const class CGameObject* pOwner);
	/* 슬라이딩 벡터 초기화 */
	void Reset_SlidingVectors();

#ifdef _DEBUG
public:	/* 고작 충돌이 됐는지 안됐는지 그리는거 때문에 이런거 하는게 좀 에바인거 같음 */
	void Reset_Colliders();
#endif // _DEBUG

private:
	_uint			  m_iNumGroups = {};
	list<CCollider*>* m_pColliders = { nullptr };

public:
	static CCollider_Manager* Create(_uint iNumGroups);
	virtual void Free() override;
};

END