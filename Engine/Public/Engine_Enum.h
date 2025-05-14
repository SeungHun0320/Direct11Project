#pragma once

namespace Engine
{
    enum class MODEL { NONANIM, ANIM };

	enum class PROTOTYPE { TYPE_GAMEOBJECT, TYPE_COMPONENT };

	enum class RENDERGROUP { RG_PRIORITY, RG_NONBLEND, RG_BLEND, RG_UI, RG_END };

	enum class STATE { RIGHT, UP, LOOK, POSITION, ST_END };

	enum class D3DTS { VIEW, PROJ, TS_END };
	
	enum class DIMK { LBUTTON, RBUTTON, WHEEL, DIMK_END };

	enum class DIMM { X, Y, WHEEL, DIMM_END };

    enum class LIFE { NONE, DEAD };

	enum class TEX_TYPE 
    {
        NONE = 0,
        DIFFUSE = 1,
        SPECULAR = 2,
        AMBIENT = 3,
        EMISSIVE = 4,
        HEIGHT = 5,
        NORMALS = 6,
        SHININESS = 7,
        OPACITY = 8,
        DISPLACEMENT = 9,
        LIGHTMAP = 10,
        REFLECTION = 11,
        BASE_COLOR = 12,
        NORMAL_CAMERA = 13,
        EMISSION_COLOR = 14,
        METALNESS = 15,
        DIFFUSE_ROUGHNESS = 16,
        AMBIENT_OCCLUSION = 17,
        UNKNOWN = 18,
        SHEEN = 19,
        CLEARCOAT = 20,
        TRANSMISSION = 21,
        MAYA_BASE = 22,
        MAYA_SPECULAR = 23,
        MAYA_SPECULAR_COLOR = 24,
        MAYA_SPECULAR_ROUGHNESS = 25,
        ANISOTROPY = 26,
        GLTF_METALLIC_ROUGHNESS = 27,

        MAX = GLTF_METALLIC_ROUGHNESS
    };
}