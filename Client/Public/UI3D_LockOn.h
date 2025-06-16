#pragma once

#include "UIContainerPart.h"

BEGIN(Client)

class CUI3D_LockOn final : public CUIContainerPart
{
public:
	enum PART { PART_RING, PART_HEX_START, PART_HEX = 3, PART_END };

public:
	typedef struct tagUI3DLockOnDesc : public CUIContainerPart::DESC
	{

	}DESC;

private:
	CUI3D_LockOn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI3D_LockOn(const CUI3D_LockOn& Prototype);
	virtual ~CUI3D_LockOn() = default;

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
	_float m_fHexAngleAcc{};

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT Ready_PartObjects() override;

public:
	static CUI3D_LockOn* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

