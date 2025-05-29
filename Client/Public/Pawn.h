#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"

BEGIN(Engine)
END

BEGIN(Client)

class CPawn abstract : public CContainerObject
{
public:
	typedef struct tagPawnDesc : public CContainerObject::DESC
	{
		LEVEL eLevelID;
	}DESC;
protected:
	CPawn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPawn(const CPawn& Prototype);
	virtual ~CPawn() = default;

public:
	void Set_Level(LEVEL eLevelID) {
		m_eLevelID = eLevelID;
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	LEVEL m_eLevelID = { LEVEL::LEVEL_END };

protected:
	virtual HRESULT Ready_Components(void* pArg);
	virtual HRESULT Ready_PartObjects() { return S_OK; };


public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;

};

END