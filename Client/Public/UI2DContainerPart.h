#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Client)

class CUI2DContainerPart abstract : public CPartObject
{
public:
	typedef struct tagUI2DContainerPartDesc : public CPartObject::DESC
	{
		LEVEL	eLevelID{};
		_uint	iNumPartObjects{};
	}DESC;

protected:
	CUI2DContainerPart(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI2DContainerPart(const CUI2DContainerPart& Prototype);
	~CUI2DContainerPart() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Set_UIVisible(_uint iPart, _bool isVisible) {};
	virtual void Set_TextureIndex(_uint iPart, _uint iTextureIdx) {};

protected:
	_uint				 m_iNumPartObjects = {};
	vector<CPartObject*> m_PartObjects;

protected: 	/* 어떤 레벨에서 쓸지 오브젝트 별로 변수 필요함 */
	LEVEL m_eLevelID = { LEVEL::LEVEL_END };

protected:
	HRESULT Add_PartObject(_uint iPartID, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg);

protected:
	virtual HRESULT Ready_Components(void* pArg);
	virtual HRESULT Ready_PartObjects() { return S_OK; };

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free()override;
};

END