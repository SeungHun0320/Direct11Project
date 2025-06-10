#include "UI3DContainerPart.h"

#include "GameInstance.h"

CUI3DContainerPart::CUI3DContainerPart(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject{pDevice, pContext}
{
}

CUI3DContainerPart::CUI3DContainerPart(const CUI3DContainerPart& Prototype)
	:CPartObject(Prototype)
{
}

HRESULT CUI3DContainerPart::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CUI3DContainerPart::Initialize(void* pArg)
{
	return E_NOTIMPL;
}

void CUI3DContainerPart::Priority_Update(_float fTimeDelta)
{
}

LIFE CUI3DContainerPart::Update(_float fTimeDelta)
{
	return LIFE();
}

void CUI3DContainerPart::Late_Update(_float fTimeDelta)
{
}

HRESULT CUI3DContainerPart::Render()
{
	return E_NOTIMPL;
}

HRESULT CUI3DContainerPart::Add_PartObject(_uint iPartID, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	CPartObject* pPartObject = dynamic_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pPartObject)
		return E_FAIL;

	m_PartObjects[iPartID] = pPartObject;

	return S_OK;
}

HRESULT CUI3DContainerPart::Ready_Components(void* pArg)
{
	return S_OK;
}

void CUI3DContainerPart::Free()
{
	__super::Free();
}
