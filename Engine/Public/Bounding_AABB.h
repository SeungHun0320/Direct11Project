#pragma once

#include "Bounding.h"

BEGIN(Engine)

class CBounding_AABB final : public CBounding
{
public:
	typedef struct tagBoundingBoxDesc : public CBounding::DESC
	{
		/* 반지름 */
		_float3		vExtents;
	}DESC;
private:
	CBounding_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_AABB() = default;

public:
	const BoundingBox* Get_Desc() const {
		return m_pDesc;
	}

public:
	virtual HRESULT Initialize(const CBounding::DESC* pDesc) override;
	virtual void Update(_fmatrix WorldMatrix) override;
	virtual _bool Intersect(CBounding* pTarget) override;
	virtual _float3 Compute_SlidingVector(CBounding* pTarget) override;


#ifdef _DEBUG
public:
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) override;
#endif

private:
	/* 채워지는 초기정보는 로컬상에 상태대로 셋팅한다. */
	BoundingBox* m_pOriginalDesc = { nullptr };
	/* 실제 충돌을 할 월드 좌표의 콜라이더 */
	BoundingBox* m_pDesc = { nullptr };

private:
	_bool Intersect_ToAABB(CBounding* pTarget);
	_float3 Compute_Min();
	_float3 Compute_Max();

public:
	static CBounding_AABB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CBounding::DESC* pDesc);
	virtual void Free()override;
};

END