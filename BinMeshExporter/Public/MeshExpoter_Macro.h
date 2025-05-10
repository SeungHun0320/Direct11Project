#pragma once

#define BEGIN(NAMESPACE) namespace NAMESPACE { 
#define END }

#define MSG_BOX(message)								\
::MessageBox(nullptr, TEXT(message), L"error", MB_OK)