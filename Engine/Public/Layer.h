#pragma once

#include "Base.h"

/* 객체들을 모아놓는다. */

BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	class CComponent* Get_Component(const _wstring& strComponentTag, _uint iIndex);
	class CComponent* Get_Component(_uint iPartID, const _wstring& strComponentTag, _uint iIndex);
	class CGameObject* Find_Object(_uint iIndex);
	class CGameObject* Find_ObjectByName(const _wstring& strObjectName);
	list<class CGameObject*>* Find_ObjectList();
public:
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

public:
	void Clear();

private:
	list<class CGameObject*>	m_GameObjects;
	class CGameInstance*		m_pGameInstance = { nullptr };

public:
	static CLayer* Create();
	virtual void Free();
};

END