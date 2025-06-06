#include "Wizard.h"

CWizard::CWizard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster{pDevice, pContext}
{
}

CWizard::CWizard(const CWizard& Prototype)
	:CMonster(Prototype)
{
}

HRESULT CWizard::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWizard::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CWizard::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

LIFE CWizard::Update(_float fTimeDelta)
{
	return __super::Update(fTimeDelta);
}

void CWizard::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CWizard::Render()
{
	return S_OK;
}

_vector CWizard::Get_State(STATE eState)
{
	return m_pTransformCom->Get_State(eState);
}

void CWizard::Go_Target(_fvector vTarget, _float fTimeDelta, _float fSpeed, _float fMinDistance)
{
	m_pTransformCom->Set_SpeedPerSec(fSpeed);
	m_pTransformCom->LookAtLerpEx(vTarget, fTimeDelta, 10.f);
	m_pTransformCom->Go_Target(vTarget, fTimeDelta, fMinDistance);
}

void CWizard::Move(_fvector vDir, _float fTimeDelta, _float fSpeed)
{
	m_pTransformCom->Set_SpeedPerSec(fSpeed);
	m_pTransformCom->LookDirLerp(vDir, fTimeDelta, fSpeed * 1.5f);
	m_pTransformCom->Go_Dir(vDir, fTimeDelta);
}

void CWizard::Hit(_fvector vDir, _float fTimeDelta, _float fSpeed)
{
	m_pTransformCom->Set_SpeedPerSec(fSpeed);
	m_pTransformCom->Go_Dir(vDir, fTimeDelta);
}

void CWizard::Turn(_fvector vAxis, _float fTimeDelta)
{
	m_pTransformCom->Turn(vAxis, fTimeDelta);
}

void CWizard::LookAt(_fvector vDir, _float fTimeDelta, _float fSpeed)
{
	m_pTransformCom->LookAtLerpEx(vDir, fTimeDelta, fSpeed);
}

HRESULT CWizard::Ready_Components(void* pArg)
{
	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CWizard::Free()
{
	__super::Free();
}
