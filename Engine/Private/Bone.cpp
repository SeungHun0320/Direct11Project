#include "Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(const BONE* pDesc)
{
	m_strName = pDesc->strName;

	memcpy(&m_TransformationMatrix, &pDesc->TransformationMatrix, sizeof(_float4x4));

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	m_iParentBoneIndex = pDesc->iParentBoneIndex;

	return S_OK;
}

void CBone::Update_TransformationMatrix(_fmatrix TransformationMartix)
{
}

void CBone::Update_CombinedTransformationMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix)
{
	/* ���� �θ��ε����� -1�̶��? �θ� ���� ���ٴ� �� */
	/* �׷��� �׳� ������ Ʈ���������̼���ĸ� �����ָ� �� */
	/* �ʱ���¸� �����ִ� ��ĵ� �� ���� �θ𿡸� �����شٸ�,
	�ڽĵ��� �ʱ���¸� �����ִ� ����� ������ ������ �θ� ����� ���� �� */

	if (-1 == m_iParentBoneIndex)
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * PreTransformMatrix);

	else
		XMStoreFloat4x4(&m_CombinedTransformationMatrix,
			XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));
}

CBone* CBone::Create(const BONE* pDesc)
{
	CBone* pInstance = new CBone();

	if (FAILED(pInstance->Initialize(pDesc)))
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
