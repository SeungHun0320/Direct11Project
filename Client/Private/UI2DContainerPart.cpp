#include "UI2DContainerPart.h"
#include "GameInstance.h"

CUI2DContainerPart::CUI2DContainerPart(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CUI2DContainerPart::CUI2DContainerPart(const CUI2DContainerPart& Prototype)
	: CPartObject(Prototype)
{
}

HRESULT CUI2DContainerPart::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI2DContainerPart::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_eLevelID = pDesc->eLevelID;
	m_iNumPartObjects = pDesc->iNumPartObjects;

	m_PartObjects.resize(m_iNumPartObjects);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CUI2DContainerPart::Priority_Update(_float fTimeDelta)
{
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject)
			pPartObject->Priority_Update(fTimeDelta);
	}
}

LIFE CUI2DContainerPart::Update(_float fTimeDelta)
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

void CUI2DContainerPart::Late_Update(_float fTimeDelta)
{
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject)
			pPartObject->Late_Update(fTimeDelta);
	}
}

HRESULT CUI2DContainerPart::Render()
{
	return S_OK;
}

HRESULT CUI2DContainerPart::Add_PartObject(_uint iPartID, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	CPartObject* pPartObject = dynamic_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pPartObject)
		return E_FAIL;

	m_PartObjects[iPartID] = pPartObject;

	return S_OK;
}

HRESULT CUI2DContainerPart::Ready_Components(void* pArg)
{
	return S_OK;
}

void CUI2DContainerPart::Free()
{
	__super::Free();
}
