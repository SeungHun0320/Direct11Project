#include "Pawn.h"
#include "GameInstance.h"

#include "PartObject.h"

CPawn::CPawn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CContainerObject{pDevice, pContext}
{
}

CPawn::CPawn(const CPawn& Prototype)
	: CContainerObject(Prototype)
{
}

HRESULT CPawn::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPawn::Initialize(void* pArg)
{
	DESC* pDesc = static_cast<DESC*>(pArg);

	m_eLevelID = pDesc->eLevelID;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}

void CPawn::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CPawn::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CPawn::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CPawn::Render()
{
	return S_OK;
}

HRESULT CPawn::Ready_Components(void* pArg)
{
	return S_OK;
}

void CPawn::Free()
{
	__super::Free();
}
