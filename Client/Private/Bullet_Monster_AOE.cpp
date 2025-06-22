#include "Bullet_Monster_AOE.h"

CBullet_Monster_AOE::CBullet_Monster_AOE(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CBullet_Monster{pDevice, pContext}
{
}

CBullet_Monster_AOE::CBullet_Monster_AOE(const CBullet_Monster_AOE& Prototype)
    :CBullet_Monster(Prototype)
{
}

HRESULT CBullet_Monster_AOE::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBullet_Monster_AOE::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CBullet_Monster_AOE::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

LIFE CBullet_Monster_AOE::Update(_float fTimeDelta)
{
    return  __super::Update(fTimeDelta);
}

void CBullet_Monster_AOE::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CBullet_Monster_AOE::Render()
{
    return __super::Render();
}

HRESULT CBullet_Monster_AOE::Ready_Components(void* pArg)
{
    if (FAILED(__super::Ready_Components(pArg)))
        return E_FAIL;

    return S_OK;
}

void CBullet_Monster_AOE::Free()
{
    __super::Free();
}
