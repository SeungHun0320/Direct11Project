#pragma once

#include "Bounding.h"

BEGIN(Engine)

class CBounding_OBB final : public CBounding
{
public:
	typedef struct tagBoundingOrientedBoxDesc : public CBounding::DESC
	{
		/* 반지름 */
		_float3		vExtents;
		/* OBB는 회전값이 들어감 */
		_float3		vRotation;
	}DESC;

	typedef struct tagOBBInfo
	{
		/* 중심점 */
		_float3		vCenter;
		/* 중심에서의 거리 */
		_float3		vCenterDir[3];
		/* 큐브와 평행한 축 */
		_float3		vAxisDir[3];
	}OBB_INFO;

private:
	CBounding_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_OBB() = default;

public:
	const BoundingOrientedBox* Get_Desc() const {
		return m_pDesc;
	}

public:
	virtual HRESULT Initialize(const CBounding::DESC* pDesc) override;;
	virtual void Update(_fmatrix WorldMatrix) override;
	virtual _bool Intersect(CBounding* pTarget) override;
	virtual _float3 Compute_SlidingVector(CBounding* pTarget) override;


#ifdef _DEBUG
public:
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) override;
#endif

private:
	/* 채워지는 초기정보는 로컬상에 상태대로 셋팅한다. */
	BoundingOrientedBox* m_pOriginalDesc = { nullptr };
	/* 실제 충돌을 할 월드 좌표의 콜라이더 */
	BoundingOrientedBox* m_pDesc = { nullptr };

private:
	_bool Intersect_ToOBB(CBounding* pTarget);
	OBB_INFO Compute_OBB();

public:
	static CBounding_OBB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CBounding::DESC* pDesc);
	virtual void Free()override;
};

END