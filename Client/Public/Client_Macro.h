#pragma once

#define _VUp		_float3(0.f,1.f,0.f)

#define GET_PLAYER m_pGameInstance->Find_Object(ENUM_CLASS(LEVEL::STATIC), TEXT("Layer_Pawn"))

#define KEY_DOWN		m_pGameInstance->Key_Down
#define	KEY_PRESSING	m_pGameInstance->Key_Pressing
#define KEY_UP			m_pGameInstance->Key_Up

#define MOUSE_DOWN		m_pGameInstance->Mouse_Down
#define MOUSE_PRESSING	m_pGameInstance->Mouse_Pressing
#define MOUSE_UP		m_pGameInstance->Mouse_Up

#define DIR_FORWARD			XMVectorSet(0.f, 0.f, 1.f, 0.f)
#define DIR_FORWARDLEFT		XMVectorSet(-1.f, 0.f, 1.f, 0.f)
#define DIR_FORWARDRIGHT    XMVectorSet(1.f, 0.f, 1.f, 0.f)
#define DIR_BACKWARD		XMVectorSet(0.f, 0.f, -1.f, 0.f)
#define DIR_BACKWARDLEFT    XMVectorSet(-1.f, 0.f, -1.f, 0.f)
#define DIR_BACKWARDRIGHT   XMVectorSet(1.f, 0.f, -1.f, 0.f)
#define DIR_LEFT			XMVectorSet(-1.f, 0.f, 0.f, 0.f)
#define DIR_RIGHT			XMVectorSet(1.f, 0.f, 0.f, 0.f)

