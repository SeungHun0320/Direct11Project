#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Client)

class CUI3DContainerPart abstract : public CPartObject
{
public:
	typedef struct tagUI3DContainerPartDesc : public CPartObject::DESC
	{
		LEVEL eLevelID{};
	}DESC;

protected:
	CUI3DContainerPart(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI3DContainerPart(const CUI3DContainerPart& Prototype);
	~CUI3DContainerPart() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	_uint				 m_iNumPartObjects = {};
	vector<CPartObject*> m_PartObjects;

protected:
	LEVEL m_eLevelID = { LEVEL::LEVEL_END };

protected:
	HRESULT Add_PartObject(_uint iPartID, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg);

protected:
	virtual HRESULT Ready_Components(void* pArg);

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free()override;
};

END