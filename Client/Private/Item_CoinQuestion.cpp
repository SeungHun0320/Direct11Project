#include "Item_CoinQuestion.h"
#include "GameInstance.h"

#include "Body_Item.h"

CItem_CoinQuestion::CItem_CoinQuestion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CItem{ pDevice, pContext }
{
}

CItem_CoinQuestion::CItem_CoinQuestion(const CItem_CoinQuestion& Prototype)
    : CItem(Prototype)
{
}

HRESULT CItem_CoinQuestion::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CItem_CoinQuestion::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_iPrice = 100;

    return S_OK;
}

void CItem_CoinQuestion::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CItem_CoinQuestion::Update(_float fTimeDelta)
{
    return  __super::Update(fTimeDelta);
}

void CItem_CoinQuestion::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CItem_CoinQuestion::Render()
{
    return S_OK;
}

void CItem_CoinQuestion::On_Collision(_uint MyColliderID, _uint OtherColliderID, CGameObject* pOwner)
{
    __super::On_Collision(MyColliderID, OtherColliderID, pOwner);
}

HRESULT CItem_CoinQuestion::Ready_Components(void* pArg)
{
    if (FAILED(__super::Ready_Components(pArg)))
        return E_FAIL;

    return S_OK;
}

HRESULT CItem_CoinQuestion::Ready_PartObjects()
{
    /* 공통적인 파트 오브젝트 생성 */
    if (FAILED(__super::Ready_PartObjects()))
        return E_FAIL;

    // 각자 바디 생성
    CBody_Item::DESC	BodyDesc{};

    BodyDesc.eLevelID = m_eLevelID;
    BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Float4x4();
    BodyDesc.strPrototypeModelTag = TEXT("Prototype_Component_Model_Trinket_Coin");
    BodyDesc.vExtents = _float3(1.f, 1.f, 1.f);
    BodyDesc.vCenter = _float3(0.f, 0.f, 0.f); 
    BodyDesc.iColliderGroupID = ENUM_CLASS(COLLIDER_GROUP::ITEM);
    BodyDesc.iColliderID = ENUM_CLASS(COLLIDER_ID::COIN_QUESTION);
    BodyDesc.pOwner = this;

    if (FAILED(__super::Add_PartObject(PART_BODY, ENUM_CLASS(m_eLevelID), TEXT("Prototype_GameObject_Body_Item"), &BodyDesc)))
        return E_FAIL;

    return S_OK;
}

CItem_CoinQuestion* CItem_CoinQuestion::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CItem_CoinQuestion* pInstance = new CItem_CoinQuestion(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CItem_CoinQuestion");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CItem_CoinQuestion::Clone(void* pArg)
{
    CItem_CoinQuestion* pInstance = new CItem_CoinQuestion(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CItem_CoinQuestion");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CItem_CoinQuestion::Free()
{
    __super::Free();
}
