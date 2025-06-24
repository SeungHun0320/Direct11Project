#include "Part_Particle.h"
#include "GameInstance.h"

CPart_Particle::CPart_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject{pDevice, pContext}
{
}

CPart_Particle::CPart_Particle(const CPart_Particle& Prototype)
	:CPartObject(Prototype)
{
}

HRESULT CPart_Particle::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CPart_Particle::Initialize(void* pArg)
{
	return E_NOTIMPL;
}

void CPart_Particle::Priority_Update(_float fTimeDelta)
{
}

LIFE CPart_Particle::Update(_float fTimeDelta)
{
	return LIFE();
}

void CPart_Particle::Late_Update(_float fTimeDelta)
{
}

HRESULT CPart_Particle::Render()
{
	return E_NOTIMPL;
}

HRESULT CPart_Particle::Ready_Components(void* pArg)
{
	return E_NOTIMPL;
}

HRESULT CPart_Particle::Bind_ShaderResources()
{
	return E_NOTIMPL;
}

CPart_Particle* CPart_Particle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CPart_Particle::Clone(void* pArg)
{
	return nullptr;
}

void CPart_Particle::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}
