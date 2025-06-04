#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype(COLLIDER eType);
	virtual HRESULT Initialize(void* pArg) override;

	void Update(_fmatrix WorldMatrix);

#ifdef _DEBUG
	HRESULT Render();
#endif

private:
	COLLIDER		 m_eType = { COLLIDER::COL_END };
	class CBounding* m_pBounding = { nullptr };

	_bool			 m_isColl = { false };

#ifdef _DEBUG
private:	/* 그리기용으로 만든 친구들 */
	/* 기본적인 그리기를 도와주는 친구 */
	PrimitiveBatch<VertexPositionColor>* m_pBatch  = { nullptr };
	/* 정점 정보를 갖고 셰이더를 적용해주는 친구 */
	BasicEffect*						 m_pEffect = { nullptr };
	/* 셰이더로 넘겨주기 위해서 필요한 친구 */
	ID3D11InputLayout*				     m_pInputLayout = { nullptr };
#endif

public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, COLLIDER eType);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END