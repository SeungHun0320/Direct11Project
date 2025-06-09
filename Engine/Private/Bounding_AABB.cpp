#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"

CBounding_AABB::CBounding_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBounding {pDevice, pContext}
{
}

HRESULT CBounding_AABB::Initialize(const CBounding::DESC* pDesc)
{
	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	const DESC* pAABBDesc = static_cast<const DESC*>(pDesc);

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

_bool CBounding_AABB::Intersect(CBounding* pTarget)
{
	_bool		isColl = { false };

	switch (pTarget->Get_Type())
	{
	case COLLIDER::AABB:
		isColl = m_pDesc->Intersects(*static_cast<CBounding_AABB*>(pTarget)->Get_Desc());
		break;
	case COLLIDER::OBB:
		isColl = m_pDesc->Intersects(*static_cast<CBounding_OBB*>(pTarget)->Get_Desc());
		break;
	case COLLIDER::SPHERE:
		isColl = m_pDesc->Intersects(*static_cast<CBounding_Sphere*>(pTarget)->Get_Desc());
		break;

	}

	return isColl;
}

_float3 CBounding_AABB::Compute_SlidingVector(CBounding* pTarget)
{
	_float3 vSlidingVector = {};

	const BoundingBox* pTargetAABB = static_cast<CBounding_AABB*>(pTarget)->Get_Desc();

	_float3 vDelta = {
		m_pDesc->Center.x - pTargetAABB->Center.x,
		m_pDesc->Center.y - pTargetAABB->Center.y,
		m_pDesc->Center.z - pTargetAABB->Center.z
	};

	_float3 vOverlap = {
		(m_pDesc->Extents.x + pTargetAABB->Extents.x) - fabsf(vDelta.x),
		(m_pDesc->Extents.y + pTargetAABB->Extents.y) - fabsf(vDelta.y),
		(m_pDesc->Extents.z + pTargetAABB->Extents.z) - fabsf(vDelta.z)
	};

	// 가장 겹친 축이 작은 방향으로만 이동
	if (vOverlap.x < vOverlap.y && vOverlap.x < vOverlap.z)
		vSlidingVector.x = (vDelta.x < 0.f ? -vOverlap.x : vOverlap.x);
	else if (vOverlap.y < vOverlap.z)
		vSlidingVector.y = (vDelta.y < 0.f ? -vOverlap.y : vOverlap.y);
	else
		vSlidingVector.z = (vDelta.z < 0.f ? -vOverlap.z : vOverlap.z);

	if (abs(vOverlap.x) < 0.001f && abs(vOverlap.y) < 0.001f && abs(vOverlap.z) < 0.001f)
		return _float3(0.f, 0.f, 0.f);

	return vSlidingVector;
}

#ifdef _DEBUG

HRESULT CBounding_AABB::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
	DX::Draw(pBatch, *m_pDesc, vColor);

	return S_OK;
}

#endif

_bool CBounding_AABB::Intersect_ToAABB(CBounding* pTarget)
{
	/* 본인의 Min과 Max를 */
	_float3		vSourMin = Compute_Min();
	_float3		vSourMax = Compute_Max();

	/* 타깃의 Min과 Max와 */
	_float3		vDestMin = static_cast<CBounding_AABB*>(pTarget)->Compute_Min();
	_float3		vDestMax = static_cast<CBounding_AABB*>(pTarget)->Compute_Max();

	/* 너비상으로 겹쳤는지 체크 */
	/* Min중의 Max, Max중의 Min */
	if (max(vSourMin.x, vDestMin.x) > min(vSourMax.x, vDestMax.x))
		return false;
	/* 세 축 다 확인 */
	if (max(vSourMin.y, vDestMin.y) > min(vSourMax.y, vDestMax.y))
		return false;

	if (max(vSourMin.z, vDestMin.z) > min(vSourMax.z, vDestMax.z))
		return false;

	return true;
}

_float3 CBounding_AABB::Compute_Min()
{
	/* 큐브의 앞 왼쪽 아래 */
	return _float3(m_pDesc->Center.x - m_pDesc->Extents.x,
		m_pDesc->Center.y - m_pDesc->Extents.y,
		m_pDesc->Center.z - m_pDesc->Extents.z);
}

_float3 CBounding_AABB::Compute_Max()
{
	/* 큐브의 뒤 오른쪽 위*/
	return _float3(m_pDesc->Center.x + m_pDesc->Extents.x,
		m_pDesc->Center.y + m_pDesc->Extents.y,
		m_pDesc->Center.z + m_pDesc->Extents.z);
}

CBounding_AABB* CBounding_AABB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CBounding::DESC* pDesc)
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
