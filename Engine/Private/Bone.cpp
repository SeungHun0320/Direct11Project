#include "Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(const BONE* pDesc, _int iParentBoneIndex)
{
	//m_strName = pDesc.strName;

	//memcpy(&m_TransformationMatrix, &pDesc->TransformationMatrix, sizeof(_float4x4));

	//XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

	//XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	//m_iParentBoneIndex = iParentBoneIndex;

	return S_OK;
}

void CBone::Update_TransformationMatrix(_fmatrix TransformationMartix)
{
}

void CBone::Update_CombinedTransformationMatrix(const vector<CBone*>& Bones)
{
	/* ���� �θ��ε����� -1�̶��? �θ� ���� ���ٴ� �� */
	/* �׷��� �׳� ������ Ʈ���������̼���ĸ� �����ָ� �� */
	if (-1 == m_iParentBoneIndex)
		m_CombinedTransformationMatrix = m_TransformationMatrix;

	else
		XMStoreFloat4x4(&m_CombinedTransformationMatrix,
			XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));
}

CBone* CBone::Create(const BONE* pDesc, _int iParentBoneIndex)
{
	CBone* pInstance = new CBone();

	if (FAILED(pInstance->Initialize(pDesc, iParentBoneIndex)))
	{
		MSG_BOX("Failed to Created : CBone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBone::Free()
{
	__super::Free();
}
