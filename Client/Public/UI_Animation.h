#pragma once

#include "UI.h"

BEGIN(Client)

class CUI_Animation : public CUI
{
public:
	typedef struct tagUIAnimationDesc : CUI::DESC
	{
		const float* pRatio{ nullptr };

	}DESC;

protected:
	CUI_Animation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Animation(const CUI_Animation& Prototype);
	virtual ~CUI_Animation() = default;

public:
	virtual HRESULT Initialize_Prototype(UI_TYPE eType) override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	const _float* m_pRatio = {};


protected:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CUI_Animation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_TYPE eType);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free()override;
};

END
