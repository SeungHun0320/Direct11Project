#pragma once

namespace Engine
{
	enum class PROTOTYPE { TYPE_GAMEOBJECT, TYPE_COMPONENT };

	enum class RENDERGROUP { RG_PRIORITY, RG_NONBLEND, RG_BLEND, RG_UI, RG_END };

	enum class STATE { RIGHT, UP, LOOK, POSITION, END };
	
	enum class DIMK { LBUTTON, RBUTTON, WHEEL, DIMK_END };

	enum class DIMM { X, Y, WHEEL, DIMM_END };
}