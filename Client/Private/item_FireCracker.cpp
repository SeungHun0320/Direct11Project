#include "Item_FireCracker.h"
#include "GameInstance.h"

#include "Body_Item.h"

CItem_FireCracker::CItem_FireCracker(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CItem{ pDevice, pContext }
{
}

CItem_FireCracker::CItem_FireCracker(const CItem_FireCracker& Prototype)
    : CItem(Prototype)
{
}

HRESULT CItem_FireCracker::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CItem_FireCracker::Initialize(void* pArg)
{
    DESC* pDesc = static_cast<DESC*>(pArg);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_iPrice = 100;

    return S_OK;
}

void CItem_FireCracker::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CItem_FireCracker::Update(_float fTimeDelta)
{
    return  __super::Update(fTimeDelta);
}

void CItem_FireCracker::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CItem_FireCracker::Render()
{
    return S_OK;
}

void CItem_FireCracker::On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner)
{
    __super::On_Collision(MyColliderID, OtherColliderID, pOwner);
}

HRESULT CItem_FireCracker::Ready_Components(void* pArg)
{
    if (FAILED(__super::Ready_Components(pArg)))
        return E_FAIL;

    return S_OK;
}

HRESULT CItem_FireCracker::Ready_PartObjects()
{   
    /* 공통적인 파트 오브젝트 생성 */
    if (FAILED(__super::Ready_PartObjects()))
        return E_FAIL;

    // 각자 바디 생성
    CBody_Item::DESC	BodyDesc{};

    BodyDesc.eLevelID = m_eLevelID;
    BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
    BodyDesc.strPrototypeModelTag = TEXT("Prototype_Component_Model_FireCrackers");
    BodyDesc.vExtents = _float3(1.f, 1.f, 1.f);
    BodyDesc.vCenter = _float3(0.f, 0.f, 0.f);
    BodyDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::ITEM);
    BodyDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::FIRE_CRACKER);
    BodyDesc.pOwner = this;

    if (FAILED(__super::Add_PartObject(PART_BODY, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_Body_Item"), &BodyDesc)))
        return E_FAIL;

    return S_OK;
}

CItem_FireCracker* CItem_FireCracker::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CItem_FireCracker* pInstance = new CItem_FireCracker(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CItem_FireCracker");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CItem_FireCracker::Clone(void* pArg)
{
    CItem_FireCracker* pInstance = new CItem_FireCracker(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CItem_FireCracker");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CItem_FireCracker::Free()
{
    __super::Free();
}
