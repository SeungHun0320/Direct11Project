#include "Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
}

CComponent* CLayer::Get_Component(const _wstring& strComponentTag, _uint iIndex)
{
	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	
	return (*iter)->Get_Component(strComponentTag);
}

CGameObject* CLayer::Find_Object(_uint iIndex)
{
	if (iIndex >= m_GameObjects.size())
		return nullptr;

	auto Iter = m_GameObjects.begin();

	for (_uint i = 0; i < iIndex; ++i)
		Iter++;

	return *Iter;
}

HRESULT CLayer::Add_GameObject(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

void CLayer::Priority_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Priority_Update(fTimeDelta);

	}
		
}

void CLayer::Update(_float fTimeDelta)
{
	LIFE eLife = {};
	for (auto Iter = m_GameObjects.begin();
		Iter != m_GameObjects.end();)
	{
		if (nullptr != *Iter)
		{
			eLife = (*Iter)->Update(fTimeDelta);
			if (LIFE::DEAD == eLife)
			{
				Safe_Release(*Iter);
				Iter = m_GameObjects.erase(Iter);
			}
			else
				Iter++;
		}
	}
}

void CLayer::Late_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Late_Update(fTimeDelta);

	}
}

void CLayer::Clear()
{
	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();
}

CLayer* CLayer::Create()
{
	return new CLayer();
}

void CLayer::Free()
{
	__super::Free();

	Clear();
}
