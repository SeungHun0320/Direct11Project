#include "Bush.h"
#include "GameInstance.h"

#include "Body_Bush.h"

CBush::CBush(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEnvironment_Object{pDevice, pContext}
{
}

CBush::CBush(const CBush& Prototype)
	: CEnvironment_Object(Prototype)
{
}

HRESULT CBush::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBush::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBush::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CBush::Update(_float fTimeDelta)
{
	if (m_bDead)
		return LIFE::DEAD;

	return __super::Update(fTimeDelta);
}

void CBush::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBush::Render()
{
	return __super::Render();
}

void CBush::On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner)
{
	COLLIDER_ID eColliderID = static_cast<COLLIDER_ID>(OtherColliderID);

	switch (eColliderID)
	{
	case COLLIDER_ID::SWORD:
		/* 나중에 바로 죽이지 말고 좀 머 어? 감사합니다. */
		m_bDead = true;
		break;
	}
}

HRESULT CBush::Ready_Components(void* pArg)
{
	return S_OK;
}

HRESULT CBush::Ready_PartObjects()
{
	CBody_Bush::DESC BodyDesc{};

	BodyDesc.eLevelID = m_eLevelID;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	BodyDesc.pOwner = this;

	if (FAILED(__super::Add_PartObject(PART_BODY, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_Body_Bush"), &BodyDesc)))
		return E_FAIL;

	return S_OK;
}

CBush* CBush::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBush* pInstance = new CBush(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBush");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBush::Clone(void* pArg)
{
	CBush* pInstance = new CBush(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBush");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBush::Free()
{
	__super::Free();
}
