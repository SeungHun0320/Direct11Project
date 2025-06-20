#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CCollider;
class CShader;
class CModel;
END

BEGIN(Client)

class CBody_Item final: public CPartObject
{
public:
	typedef struct tagBodyItemDesc : public CPartObject::DESC
	{
		LEVEL    eLevelID{};
		/* 모델 */
		_wstring strPrototypeModelTag{};
		/* 콜라이더 */
		_uint	iColliderGroupID{};
		_uint	iColliderID{};
		_float3 vExtents{};
		_float3 vCenter{};

	}DESC;
private:
	CBody_Item(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Item(const CBody_Item& Prototype);
	virtual ~CBody_Item() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CCollider* m_pColliderCom = { nullptr };
	CShader*   m_pShaderCom = { nullptr };
	CModel*	   m_pModelCom = { nullptr };

private:
	LEVEL      m_eLevelID = { LEVEL::LEVEL_END };

private:
	virtual HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	static CBody_Item* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END