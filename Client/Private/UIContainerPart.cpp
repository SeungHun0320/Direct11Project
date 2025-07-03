#include "UIContainerPart.h"
#include "GameInstance.h"

#include "UI.h"
CUIContainerPart::CUIContainerPart(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CUIContainerPart::CUIContainerPart(const CUIContainerPart& Prototype)
	: CPartObject(Prototype)
{
}

HRESULT CUIContainerPart::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIContainerPart::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_pLevelID = pDesc->pParentLevelID;
	m_iNumPartObjects = pDesc->iNumPartObjects;

	m_PartObjects.resize(m_iNumPartObjects);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}

void CUIContainerPart::Priority_Update(_float fTimeDelta)
{
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject)
			pPartObject->Priority_Update(fTimeDelta);
	}
}

LIFE CUIContainerPart::Update(_float fTimeDelta)
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

void CUIContainerPart::Late_Update(_float fTimeDelta)
{
	for (auto& pPartObject : m_PartObjects)
	{
		if (nullptr != pPartObject)
			pPartObject->Late_Update(fTimeDelta);
	}
}

HRESULT CUIContainerPart::Render()
{
	return S_OK;
}

void CUIContainerPart::Set_UIVisible(_uint iPart, _bool isVisible)
{
	static_cast<CUI*>(m_PartObjects[iPart])->Set_Visible(isVisible);
}

void CUIContainerPart::Set_TextureIndex(_uint iPart, _uint iTextureIdx)
{
	static_cast<CUI*>(m_PartObjects[iPart])->Set_TextureIndex(iTextureIdx);
}

HRESULT CUIContainerPart::Add_PartObject(_uint iPartID, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	CPartObject* pPartObject = dynamic_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pPartObject)
		return E_FAIL;

	m_PartObjects[iPartID] = pPartObject;

	return S_OK;
}

HRESULT CUIContainerPart::Ready_Components(void* pArg)
{
	return S_OK;
}

void CUIContainerPart::Free()
{
	__super::Free();

	for (auto& pPartObject : m_PartObjects)
		Safe_Release(pPartObject);

	m_PartObjects.clear();
}
