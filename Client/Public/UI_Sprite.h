#pragma once

#include "UI.h"

BEGIN(Client)

class CUI_Sprite : public CUI
{
public:
	typedef struct tagUISpriteDesc : CUI::DESC
	{
		_float fMaxFrame{};
		_int   iColumns{};
		_float fSpeed{};

	}DESC;

protected:
	CUI_Sprite(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Sprite(const CUI_Sprite& Prototype);
	virtual ~CUI_Sprite() = default;

public:
	virtual HRESULT Initialize_Prototype(UI_TYPE eType) override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	_float  m_fCurFrame = {};
	_float  m_fMaxFrame = {};
	/* 얼마단위로 행 끊을건지 */
	_int   m_iColumns = {};

	_float m_fSpeed = {};

protected:
	void  Move_Frame(_float fTimeDelta);

protected:
	virtual HRESULT Ready_Components(void* pArg) override;

public:
	static CUI_Sprite* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_TYPE eType);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free()override;
};

END
