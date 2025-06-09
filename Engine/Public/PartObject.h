#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CPartObject abstract : public CGameObject
{
public:
	typedef struct tagPartObjectDesc : CGameObject::DESC
	{
		const _float4x4*    pParentMatrix {nullptr};
		CGameObject*		pOwner{ nullptr };
	}DESC;
protected:
	CPartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPartObject(const CPartObject& Prototype);
	virtual ~CPartObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual LIFE Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public: /* 애니메이션 */
	virtual _bool Is_CurrentAnim(_uint iNextIndex) { return true; };
	virtual _bool Play_Animation(_float fTimeDelta) { return true; };
	virtual void Set_TickPerSecond(_float fTickPerSecond) {};
	virtual void Change_Animation(_uint iNextIndex, _bool isLoop, _float fBlendDuration, _bool isBlend) {};
	virtual void Set_MeshVisible(_uint iIndex, _bool IsVisible) {};
	virtual void Set_TrackPosition(_float fTrackPosition) {};

	/* 뼈매트릭스 갖고오기 */
	virtual const _float4x4* Get_BoneMatrix(const _string& strBoneName) const { return nullptr; };

public: /* 콜라이더 */
	virtual void Set_Active(_bool isActive = true) {};
	virtual class CCollider* Get_Collider(_uint iColliderIndex = 0) const { return nullptr; };

protected:
	const _float4x4* m_pParentMatrix = { nullptr };
	_float4x4		 m_CombinedWorldMatrix{};

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END