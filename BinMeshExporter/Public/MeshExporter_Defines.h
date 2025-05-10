#pragma once
#include "../Default/framework.h"
#include <process.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "MeshExporter_Macro.h"
#include "MeshExporter_Enum.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "ImGuizmo.h"
#include "ImGuiFileDialog.h"

const unsigned int			g_iWinSizeX = 1280;
const unsigned int			g_iWinSizeY = 720;


extern HWND g_hWnd;
extern HINSTANCE g_hInst;

using namespace MeshExporter;

#ifdef _DEBUG            // 메모리 누수 체크 매크로

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif