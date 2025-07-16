#pragma once

#include "UIContainerPart.h"

BEGIN(Client)

class CUI2D_Purchase final : public CUIContainerPart
{
public:
	enum PART {
		PART_UIBOX, PART_NAME, PART_MONEY, PART_BUY_BUTTON, PART_CANCEL_BUTTON, PART_END
	};

public:
	typedef struct tagUI2DPlayerHPBarDesc : public CUIContainerPart::DESC
	{
		const _int* pParentPrice{ nullptr };
		const _bool* pParentisDeal{ nullptr };
	}DESC;

private:
	CUI2D_Purchase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI2D_Purchase(const CUI2D_Purchase& Prototype);
	virtual ~CUI2D_Purchase() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	const _int Get_ButtonIndex(PART ePart);

private:
	const _int*  m_pParentPrice = { nullptr };
	const _bool* m_pParentisDeal = { nullptr };

private:
	void ChooseButton(PART ePart);
	void Subscribe_Events();

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;

public:
	static CUI2D_Purchase* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END