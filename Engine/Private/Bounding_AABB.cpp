#include "Bounding_AABB.h"

CBounding_AABB::CBounding_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBounding {pDevice, pContext}
{
}

HRESULT CBounding_AABB::Initialize(const CBounding::BOUNDING_DESC* pDesc)
{
	const AABB_DESC* pAABBDesc = static_cast<const AABB_DESC*>(pDesc);

	m_pOriginalDesc = new BoundingBox(pAABBDesc->vCenter, pAABBDesc->vExtents);
	m_pDesc = new BoundingBox(*m_pOriginalDesc);

	return S_OK;
}

void CBounding_AABB::Update(_fmatrix WorldMatrix)
{
	/* AABB의 회전값을 빼주기 위해서 (AABB가 회전한다는 것 자체가 말이 안 됨 )*/
	_matrix			TransformMatrix = WorldMatrix;

	/* 각행에 해당되는 값에 스케일값을 곱해서 회전값만 삭 빼줌 */
	TransformMatrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVector3Length(TransformMatrix.r[0]);
	TransformMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVector3Length(TransformMatrix.r[1]);
	TransformMatrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVector3Length(TransformMatrix.r[2]);

	/* 무려 콜라이더 안에 트랜스폼이라는 함수가 있다 !! */
	m_pOriginalDesc->Transform(*m_pDesc, TransformMatrix);
}

#ifdef _DEBUG

HRESULT CBounding_AABB::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
	DX::Draw(pBatch, *m_pDesc, vColor);

	return S_OK;
}

#endif

CBounding_AABB* CBounding_AABB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CBounding::BOUNDING_DESC* pDesc)
{
	CBounding_AABB* pInstance = new CBounding_AABB(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pDesc)))
	{
		MSG_BOX("Failed to Created : CBounding_AABB");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CBounding_AABB::Free()
{
	__super::Free();

	Safe_Delete(m_pOriginalDesc);
	Safe_Delete(m_pDesc);
}
