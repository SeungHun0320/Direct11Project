#pragma once

#include "Bounding.h"

BEGIN(Engine)

class CBounding_OBB final : public CBounding
{
public:
	typedef struct tagBoundingOrientedBoxDesc : public CBounding::BOUNDING_DESC
	{
		/* 반지름 */
		_float3		vExtents;
		/* OBB는 회전값이 들어감 */
		_float3		vRotation;
	}OBB_DESC;
private:
	CBounding_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_OBB() = default;

public:
	HRESULT Initialize(const CBounding::BOUNDING_DESC* pDesc);
	virtual void Update(_fmatrix WorldMatrix) override;

#ifdef _DEBUG
public:
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) override;
#endif

private:
	/* 채워지는 초기정보는 로컬상에 상태대로 셋팅한다. */
	BoundingOrientedBox* m_pOriginalDesc = { nullptr };
	/* 실제 충돌을 할 월드 좌표의 콜라이더 */
	BoundingOrientedBox* m_pDesc = { nullptr };

public:
	static CBounding_OBB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CBounding::BOUNDING_DESC* pDesc);
	virtual void Free()override;
};

END