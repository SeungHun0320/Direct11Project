#include "Bounding_OBB.h"

CBounding_OBB::CBounding_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBounding {pDevice, pContext}
{
}

HRESULT CBounding_OBB::Initialize(const CBounding::BOUNDING_DESC* pDesc)
{
	const OBB_DESC* pOBBDesc = static_cast<const OBB_DESC*>(pDesc);

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

#ifdef _DEBUG

HRESULT CBounding_OBB::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
	DX::Draw(pBatch, *m_pDesc, vColor);

	return S_OK;
}

#endif

CBounding_OBB* CBounding_OBB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CBounding::BOUNDING_DESC* pDesc)
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
