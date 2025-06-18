#pragma once

#include "UIContainerPart.h"

BEGIN(Client)

class CUI3D_Interaction final : public CUIContainerPart
{
public:
	enum PART { PART_SPACE, PART_END };

public:
	typedef struct tagUI3DInteractionDesc : public CUIContainerPart::DESC
	{
		_bool* pParentIsCollisioned{ nullptr };
	}DESC;

private:
	CUI3D_Interaction(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI3D_Interaction(const CUI3D_Interaction& Prototype);
	virtual ~CUI3D_Interaction() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Set_UIVisible(_uint iPart, _bool isVisible) override;
	virtual void Set_TextureIndex(_uint iPart, _uint iTextureIdx)override;

private:
	_bool* m_pParentIsCollisioned = { nullptr };

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;

public:
	static CUI3D_Interaction* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END