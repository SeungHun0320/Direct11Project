#include "EffectContainerPart.h"
#include "GameInstance.h"

CEffectContainerPart::CEffectContainerPart(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CEffectContainerPart::CEffectContainerPart(const CEffectContainerPart& Prototype)
	: CPartObject(Prototype)
{
}

HRESULT CEffectContainerPart::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffectContainerPart::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_pLevelID = pDesc->pParentLevelID;
	m_iNumPartObjects = pDesc->iNumPartObjects;

	m_PartObjects.resize(m_iNumPartObjects);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CEffectContainerPart::Priority_Update(_float fTimeDelta)
{
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject)
			pPartObject->Priority_Update(fTimeDelta);
	}
}

LIFE CEffectContainerPart::Update(_float fTimeDelta)
{
	LIFE eLife = {};
	for (auto Iter = m_PartObjects.begin();
		Iter != m_PartObjects.end();)
	{
		if (nullptr != *Iter)
		{
			eLife = (*Iter)->Update(fTimeDelta);
			if (LIFE::DEAD == eLife)
			{
				Safe_Release(*Iter);
				Iter = m_PartObjects.erase(Iter);
			}
			else
				Iter++;
		}
		else
			Iter++;
	}

	return LIFE::NONE;
}

void CEffectContainerPart::Late_Update(_float fTimeDelta)
{
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject)
			pPartObject->Late_Update(fTimeDelta);
	}
}

HRESULT CEffectContainerPart::Render()
{
	return S_OK;
}

HRESULT CEffectContainerPart::Add_PartObject(_uint iPartID, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	CPartObject* pPartObject = dynamic_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pPartObject)
		return E_FAIL;

	m_PartObjects[iPartID] = pPartObject;

	return S_OK;
}

HRESULT CEffectContainerPart::Ready_Components(void* pArg)
{
	return S_OK;
}

void CEffectContainerPart::Free()
{
	__super::Free();

	for (auto& pPartObject : m_PartObjects)
		Safe_Release(pPartObject);

	m_PartObjects.clear();
}
