#include "Monster_Bullet_AOE.h"

CMonster_Bullet_AOE::CMonster_Bullet_AOE(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CMonster_Bullet{pDevice, pContext}
{
}

CMonster_Bullet_AOE::CMonster_Bullet_AOE(const CMonster_Bullet_AOE& Prototype)
    :CMonster_Bullet(Prototype)
{
}

HRESULT CMonster_Bullet_AOE::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMonster_Bullet_AOE::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CMonster_Bullet_AOE::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CMonster_Bullet_AOE::Update(_float fTimeDelta)
{
    return  __super::Update(fTimeDelta);
}

void CMonster_Bullet_AOE::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CMonster_Bullet_AOE::Render()
{
    return __super::Render();
}

HRESULT CMonster_Bullet_AOE::Ready_Components(void* pArg)
{
    if (FAILED(__super::Ready_Components(pArg)))
        return E_FAIL;

    return S_OK;
}

void CMonster_Bullet_AOE::Free()
{
    __super::Free();
}
