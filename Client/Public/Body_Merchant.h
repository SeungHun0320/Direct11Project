#pragma once
#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CBody_Merchant final : public CPartObject
{
public:
	typedef struct tagBodyMerchantDesc : public CPartObject::DESC
	{
		const _bool* pParentisFind{ nullptr };
		const LEVEL* pParentLevelID{ nullptr };
	}DESC;

private:
	CBody_Merchant(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Merchant(const CBody_Merchant& Prototype);
	virtual ~CBody_Merchant() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Shadow() override;

public:
	virtual _bool Play_Animation(_float fTimeDelta) override;
	virtual void Change_Animation(_uint iNextIndex, _bool isLoop = true, _float fBlendDuration = 0.f, _bool isBlend = true) override;

private:
	CShader* m_pShaderCom = { nullptr };
	CModel*  m_pModelCom = { nullptr };

private:
	const LEVEL* m_pParentLevelID = { nullptr };
	const _bool* m_pParentisFind = { nullptr };

private:
	_float4 m_vEmissiveColor = {};

protected:
	virtual HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();

public:
	static CBody_Merchant* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END