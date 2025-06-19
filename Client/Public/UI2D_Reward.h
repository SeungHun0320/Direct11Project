#pragma once

#include "UIContainerPart.h"

class CUI2D_Reward final : public CUIContainerPart
{
public:
	enum PART { PART_BACKING, PART_ITEMS, PART_END };

private:
	CUI2D_Reward(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI2D_Reward(const CUI2D_Reward& Prototype);
	virtual ~CUI2D_Reward() = default;
};

