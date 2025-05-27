#include "Body_Player.h"

CBody_Player::CBody_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
}

CBody_Player::CBody_Player(const CBody_Player& Prototype)
{
}

HRESULT CBody_Player::Initialize_Prototype()
{
    return E_NOTIMPL;
}

HRESULT CBody_Player::Initialize(void* pArg)
{
    return E_NOTIMPL;
}

void CBody_Player::Priority_Update(_float fTimeDelta)
{
}

LIFE CBody_Player::Update(_float fTimeDelta)
{
    return LIFE();
}

void CBody_Player::Late_Update(_float fTimeDelta)
{
}

HRESULT CBody_Player::Render()
{
    return E_NOTIMPL;
}

HRESULT CBody_Player::Ready_Components(void* pArg)
{
    return E_NOTIMPL;
}

HRESULT CBody_Player::Bind_ShaderResources()
{
    return E_NOTIMPL;
}

CBody_Player* CBody_Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    return nullptr;
}

CGameObject* CBody_Player::Clone(void* pArg)
{
    return nullptr;
}

void CBody_Player::Free()
{
}
