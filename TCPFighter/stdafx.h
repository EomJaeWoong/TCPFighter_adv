// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#define _WINSOCK_DEPRECATED_NO_WARNINGS
// Windows ��� ����:
#include <windows.h>
#include <windowsx.h>

// C ��Ÿ�� ��� �����Դϴ�.
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

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
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