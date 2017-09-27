// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#define _WINSOCK_DEPRECATED_NO_WARNINGS
// Windows 헤더 파일:
#include <windows.h>
#include <windowsx.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <list>
#include <mmsystem.h>

#pragma comment (lib, "winmm.lib")
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include "eSprite.h"
#include "ObjectType.h"
#include "ScreenDib.h"
#include "SpriteDib.h"
#include "PacketDefine.h"
#include "BaseObject.h"
#include "PlayerObject.h"
#include "EffectObject.h"
#include "StreamQueue.h"
#include "NPacket.h"
#include "Network.h"
#include "FrameSkip.h"
#include "CMap.h"

typedef list<CBaseObject *> Object;

extern CAyaStreamSQ SendQ;
extern CAyaStreamSQ RecvQ;

extern Object g_Object;
extern CBaseObject *g_pPlayerObject;

extern CMap g_cTileMap;