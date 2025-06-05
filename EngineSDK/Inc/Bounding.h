#pragma once

#include "Base.h"

#ifdef _DEBUG
#include "DebugDraw.h"
#endif

BEGIN(Engine)

class CBounding abstract : public CBase
{
public:
	typedef struct tagBoundingDesc
	{
		/* 콜라이더 타입 */
		COLLIDER    eType;
		/* 중심점 */
		_float3		vCenter;
		/* 콜라이더를 들고있는 객체 */
		class CGameObject* pOwner = { nullptr };
		/* 콜라이더 그룹 */
		_uint iColliderGroupID;
		/* 콜라이더 아이디 */
		_uint iColliderID;

	}DESC;
protected:
	CBounding(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding() = default;

public:
	COLLIDER Get_Type() const {
		return m_eType;
	}

public:
	virtual HRESULT Initialize(const CBounding::DESC* pDesc);
	virtual void Update(_fmatrix WorldMatrix) PURE;
	virtual _bool Intersect(CBounding* pTarget) PURE;

#ifdef _DEBUG
public:
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) { return S_OK; }
#endif

protected:
	ID3D11Device*		 m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

protected:
	COLLIDER			 m_eType = { COLLIDER::COL_END };

public:
	virtual void Free() override;
};

END