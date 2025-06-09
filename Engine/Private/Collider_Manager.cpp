#include "Collider_Manager.h"
#include "Collider.h"

#include "GameObject.h"

CCollider_Manager::CCollider_Manager()
{
}

void CCollider_Manager::Clear()
{
    for (_uint i = 0; i < m_iNumGroups; ++i)
    {
        for (auto& Collider : m_pColliders[i])
            Safe_Release(Collider);
        m_pColliders[i].clear();
    }
}

void CCollider_Manager::Delete_Collider(const CGameObject* pOwner)
{
    for (_uint i = 0; i < m_iNumGroups; ++i)
    {
        for (auto Iter = m_pColliders[i].begin();
            Iter != m_pColliders[i].end();)
        {
            if ((*Iter)->Get_Owner() == pOwner)
            {
                Safe_Release(*Iter);
                Iter = m_pColliders[i].erase(Iter);
                continue;
            }
            Iter++;
        }
    }
}

void CCollider_Manager::Reset_SlidingVectors()
{
    for (_uint i = 0; i < m_iNumGroups; ++i)
    {
        for (auto& pCollider : m_pColliders[i])
        {
            if (nullptr != pCollider && pCollider->Get_IsActive())
                pCollider->Clear_SlidingVector();
        }
    }
}

#ifdef _DEBUG
void CCollider_Manager::Reset_Colliders()
{
    for (_uint i = 0; i < m_iNumGroups; i++)
    {
        for (auto& Collider : m_pColliders[i])
            Collider->Reset_Collsion();
    }
}
#endif // _DEBUG

HRESULT CCollider_Manager::Initialize(_uint iNumGroups)
{
    m_iNumGroups = iNumGroups;

    m_pColliders = new list<CCollider*>[iNumGroups];

    return S_OK;
}

HRESULT CCollider_Manager::Add_Collider(CCollider* pCollider, _uint iColliderGroupID)
{
    if (iColliderGroupID >= m_iNumGroups ||
        nullptr == pCollider)
        return E_FAIL;

    m_pColliders[iColliderGroupID].push_back(pCollider);
    Safe_AddRef(pCollider);

    return S_OK;
}

void CCollider_Manager::Intersect(_uint iColliderGroupID1, _uint iColliderGroupID2)
{
    if (iColliderGroupID1 >= m_iNumGroups ||
        iColliderGroupID2 >= m_iNumGroups)
        return;

    for (auto& pCollider1 : m_pColliders[iColliderGroupID1])
    {
        if (nullptr == pCollider1 || !pCollider1->Get_IsActive())
            continue;

        for (auto& pCollider2 : m_pColliders[iColliderGroupID2])
        {
            if (nullptr == pCollider2 || !pCollider2->Get_IsActive())
                continue;

            if (pCollider1 == pCollider2)
                continue;
           

            if (pCollider1->Intersect(pCollider2))
            {
                CGameObject* pOwner1 = pCollider1->Get_Owner();
                CGameObject* pOwner2 = pCollider2->Get_Owner();

                if (nullptr == pOwner1 && nullptr == pOwner2)
                    return;

                pOwner1->On_Collision(pCollider1->Get_ID(), pCollider2->Get_ID(), pOwner2);
                pOwner2->On_Collision(pCollider2->Get_ID(), pCollider1->Get_ID(), pOwner1);
            }
        }
    }
}

CCollider_Manager* CCollider_Manager::Create(_uint iNumGroups)
{
    CCollider_Manager* pInstance = new CCollider_Manager();

    if (FAILED(pInstance->Initialize(iNumGroups)))
    {
        MSG_BOX("Failed to Created : CCollider_Manager");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCollider_Manager::Free()
{
    __super::Free();
    Clear();
    Safe_Delete_Array(m_pColliders);
}
