#include "Item_Potion.h"
#include "GameInstance.h"

#include "Body_Item.h"

CItem_Potion::CItem_Potion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CItem{ pDevice, pContext }
{
}

CItem_Potion::CItem_Potion(const CItem_Potion& Prototype)
    : CItem(Prototype)
{
}

HRESULT CItem_Potion::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CItem_Potion::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_iPrice = 100;

    return S_OK;
}

void CItem_Potion::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CItem_Potion::Update(_float fTimeDelta)
{
    return  __super::Update(fTimeDelta);
}

void CItem_Potion::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CItem_Potion::Render()
{
    return S_OK;
}

void CItem_Potion::On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner)
{
    __super::On_Collision(MyColliderID, OtherColliderID, pOwner);
}

HRESULT CItem_Potion::Ready_Components(void* pArg)
{
    if (FAILED(__super::Ready_Components(pArg)))
        return E_FAIL;

    return S_OK;
}

HRESULT CItem_Potion::Ready_PartObjects()
{
    // 각자 바디 생성
    CBody_Item::DESC	BodyDesc{};

    BodyDesc.eLevelID = m_eLevelID;
    BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
    BodyDesc.strPrototypeModelTag = TEXT("Prototype_Component_Model_Potion");
    BodyDesc.vExtents = _float3(1.f, 1.f, 1.f);
    BodyDesc.vCenter = _float3(0.f, 0.f, 0.f);
    BodyDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::ITEM);
    BodyDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::POTION);
    BodyDesc.pOwner = this;

    if (FAILED(__super::Add_PartObject(PART_BODY, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_Body_Item"), &BodyDesc)))
        return E_FAIL;

    return S_OK;
}

CItem_Potion* CItem_Potion::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CItem_Potion* pInstance = new CItem_Potion(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CItem_Potion");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CItem_Potion::Clone(void* pArg)
{
    CItem_Potion* pInstance = new CItem_Potion(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CItem_Potion");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CItem_Potion::Free()
{
    __super::Free();
}
