#include "Bounding_OBB.h"
#include "Bounding_AABB.h"
#include "Bounding_Sphere.h"

CBounding_OBB::CBounding_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBounding {pDevice, pContext}
{
}

HRESULT CBounding_OBB::Initialize(const CBounding::DESC* pDesc)
{
	if (FAILED(__super::Initialize(pDesc)))

		return E_FAIL;
	const DESC* pOBBDesc = static_cast<const DESC*>(pDesc);

	/* 플롯3로 각 축에 대한 각도를 받아와서, 쿼터니언으로 변환후에 */
	/* 쿼터니언을 충돌체의 Desc로 넣어줌*/
	_float4		vQuaternion = {};

	XMStoreFloat4(&vQuaternion, XMQuaternionRotationRollPitchYaw(pOBBDesc->vRotation.x, pOBBDesc->vRotation.y, pOBBDesc->vRotation.z));

	m_pOriginalDesc = new BoundingOrientedBox(pOBBDesc->vCenter, pOBBDesc->vExtents, vQuaternion);
	m_pDesc = new BoundingOrientedBox(*m_pOriginalDesc);

	return S_OK;
}

void CBounding_OBB::Update(_fmatrix WorldMatrix)
{
	m_pOriginalDesc->Transform(*m_pDesc, WorldMatrix);
}

_bool CBounding_OBB::Intersect(CBounding* pTarget)
{
	_bool		isColl = { false };

	switch (pTarget->Get_Type())
	{
	case COLLIDER::AABB:
		isColl = m_pDesc->Intersects(*static_cast<CBounding_AABB*>(pTarget)->Get_Desc());
		break;
	case COLLIDER::OBB:
		isColl = m_pDesc->Intersects(*static_cast<CBounding_OBB*>(pTarget)->Get_Desc());
		//isColl = Intersect_ToOBB(pTarget);
		break;
	case COLLIDER::SPHERE:
		isColl = m_pDesc->Intersects(*static_cast<CBounding_Sphere*>(pTarget)->Get_Desc());
		break;

	}

	return isColl;
}

_float3 CBounding_OBB::Compute_SlidingVector(CBounding* pTarget)
{
	return _float3();
}

#ifdef _DEBUG

HRESULT CBounding_OBB::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
	DX::Draw(pBatch, *m_pDesc, vColor);

	return S_OK;
}

#endif

_bool CBounding_OBB::Intersect_ToOBB(CBounding* pTarget)
{
	/* 나의 충돌용 정보와 상대방의 충돌용 정보를 계산해서 받아옴 */
	OBB_INFO	OBBDesc[2] = {
		Compute_OBB(),
		static_cast<CBounding_OBB*>(pTarget)->Compute_OBB()
	};

	/* 충돌 거리 */
	_float		fDistance[3] = {};

	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			/* 상대의 중심에서 내 중심을 뺀 값에 평행한 축을 내적해서 중심과 평행한 거리를 계산함 ( 음수랑 비교할게 아니니 절댓값 ) */
			fDistance[0] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenter) - XMLoadFloat3(&OBBDesc[0].vCenter),
				XMLoadFloat3(&OBBDesc[i].vAxisDir[j]))));

			/* 본인의 중심점에서의 거리와 각각의 평행한 축을 내적해서 거리를 구한뒤 싹 다 더해줌 */
			fDistance[1] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterDir[0]), XMLoadFloat3(&OBBDesc[i].vAxisDir[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterDir[1]), XMLoadFloat3(&OBBDesc[i].vAxisDir[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterDir[2]), XMLoadFloat3(&OBBDesc[i].vAxisDir[j]))));

			/* 상대의 중심점에서의 거리와 각각의 평행한 축을 내적해서 거리를 구한뒤 싹 다 더해줌 */
			fDistance[2] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterDir[0]), XMLoadFloat3(&OBBDesc[i].vAxisDir[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterDir[1]), XMLoadFloat3(&OBBDesc[i].vAxisDir[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterDir[2]), XMLoadFloat3(&OBBDesc[i].vAxisDir[j]))));

			/* 중심과 평행한거리가 각 거리끼리 더한값보다 작다면? 충돌 안함 */
			if (fDistance[0] > fDistance[1] + fDistance[2])
				return false;
		}
	}

	return true;
}

CBounding_OBB::OBB_INFO CBounding_OBB::Compute_OBB()
{
	/* 보통 큐브 앞 왼쪽 아래로 계산함 */
	OBB_INFO		OBBDesc{};

	_float3			vPoints[8];

	/* 콜라이더에 점의 위치를 반환하는 함수가 있음 */
	m_pDesc->GetCorners(vPoints);

	/* 중심점 받아오기 */
	OBBDesc.vCenter = m_pDesc->Center;

	/* 각 점에서 뺀 값에 0.5를 곱해서 중심점에서 떨어진 방향을 구함 */
	XMStoreFloat3(&OBBDesc.vCenterDir[0], (XMLoadFloat3(&vPoints[5]) - XMLoadFloat3(&vPoints[4])) * 0.5f);
	XMStoreFloat3(&OBBDesc.vCenterDir[1], (XMLoadFloat3(&vPoints[7]) - XMLoadFloat3(&vPoints[4])) * 0.5f);
	XMStoreFloat3(&OBBDesc.vCenterDir[2], (XMLoadFloat3(&vPoints[0]) - XMLoadFloat3(&vPoints[4])) * 0.5f);

	/* 정규화 해서 축의 방향을 구함 */
	for (size_t i = 0; i < 3; i++)
		XMStoreFloat3(&OBBDesc.vAxisDir[i], XMVector3Normalize(XMLoadFloat3(&OBBDesc.vCenterDir[i])));

	return OBBDesc;
}

CBounding_OBB* CBounding_OBB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CBounding::DESC* pDesc)
{
	CBounding_OBB* pInstance = new CBounding_OBB(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pDesc)))
	{
		MSG_BOX("Failed to Created : CBounding_OBB");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CBounding_OBB::Free()
{
	__super::Free();

	Safe_Delete(m_pOriginalDesc);
	Safe_Delete(m_pDesc);
}
