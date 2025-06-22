#include "Item_Coin.h"
#include "GameInstance.h"

#include "Body_Item.h"

CItem_Coin::CItem_Coin(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CItem{pDevice, pContext}
{
}

CItem_Coin::CItem_Coin(const CItem_Coin& Prototype)
	:CItem(Prototype)
{
}

HRESULT CItem_Coin::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItem_Coin::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTargetTransform = static_cast<CTransform*>(GET_PLAYER->Get_Component(TEXT("Com_Transform")));
	if (nullptr == m_pTargetTransform)
		return E_FAIL;

	Safe_AddRef(m_pTargetTransform);

	return S_OK;
}

void CItem_Coin::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CItem_Coin::Update(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (2.f <= m_fTimeAcc)
		m_pTransformCom->Go_Target(m_pTargetTransform->Get_State(STATE::POSITION), fTimeDelta, 0.f);


	return __super::Update(fTimeDelta);
}

void CItem_Coin::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CItem_Coin::Render()
{
	return S_OK;
}

void CItem_Coin::On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner)
{
	COLLIDER_ID eColliderID = static_cast<COLLIDER_ID>(OtherColliderID);

	if (CI_PLAYER(eColliderID))
		m_bDead = true;
}

HRESULT CItem_Coin::Ready_Components(void* pArg)
{
	if (__super::Ready_Components(pArg))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CItem_Coin::Ready_PartObjects()
{
	CBody_Item::DESC	BodyDesc{};

	BodyDesc.eLevelID = m_eLevelID;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
	BodyDesc.strPrototypeModelTag = TEXT("Prototype_Component_Model_Coin");
	BodyDesc.vExtents =  _float3(0.2f, 0.2f, 0.2f);
	BodyDesc.vCenter =	_float3(0.f, 0.f, 0.f);
	BodyDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::ITEM);
	BodyDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::COIN);
	BodyDesc.pOwner = this;

	if (FAILED(__super::Add_PartObject(PART_BODY, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_Body_Item"), &BodyDesc)))
		return E_FAIL;

	return S_OK;
}

CItem_Coin* CItem_Coin::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CItem_Coin* pInstance = new CItem_Coin(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CItem_Coin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CItem_Coin::Clone(void* pArg)
{
	CItem_Coin* pInstance = new CItem_Coin(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CItem_Coin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem_Coin::Free()
{
	__super::Free();

	Safe_Release(m_pTargetTransform);
}
