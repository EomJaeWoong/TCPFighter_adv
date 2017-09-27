// TCPFighter.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "TCPFighter.h"

// 전역 변수:
HINSTANCE hInst;	// 현재 인스턴스입니다.
HWND g_hWnd;

/*----------------------------------------------------------------------------*/
// 자신(플레이어)
/*----------------------------------------------------------------------------*/
CBaseObject *g_pPlayerObject;

/*----------------------------------------------------------------------------*/
// 오브젝트들(플레이어, 이펙트)
/*----------------------------------------------------------------------------*/
Object g_Object;

/*----------------------------------------------------------------------------*/
// 스크린과 스프라이트
/*----------------------------------------------------------------------------*/
CScreenDib g_cScreenDib(dfSCREEN_WIDTH, dfSCREEN_HEIGHT, 32);
CSpriteDib *g_pSpriteDib;

/*----------------------------------------------------------------------------*/
// 프레임
/*----------------------------------------------------------------------------*/
CFrameSkip g_FrameSkip(50);

/*----------------------------------------------------------------------------*/
// 타일 맵
/*----------------------------------------------------------------------------*/
CMap g_cTileMap;

/*----------------------------------------------------------------------------*/
// Client Socket
/*----------------------------------------------------------------------------*/
SOCKET client_sock;
BOOL g_bActiveApp;

/*----------------------------------------------------------------------------*/
// Client Send,Recv Buffer
/*----------------------------------------------------------------------------*/
CAyaStreamSQ SendQ(10000);
CAyaStreamSQ RecvQ(10000);

DWORD g_dwFrameCount = 0;
DWORD g_dwStartTick = 0 ;
DWORD g_dwEndTick = 0;

int retval; 

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void InitialGame();
void Update_Game(void);
void KeyProcess();
void Action();
void Draw();
void ConnectProc();
BOOL ReadProc();
void WriteProc();

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	timeBeginPeriod(1);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 여기에 코드를 입력합니다.
	MSG msg;
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TCPFIGHTER));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_TCPFIGHTER);
	wcex.lpszClassName = L"TCPFighter";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassEx(&wcex);

	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	g_hWnd = CreateWindow(L"TCPFighter", L"TCPFighter", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, hInstance, NULL);

	if (!g_hWnd)
	{
		return FALSE;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	////////////////////////////////////////////////////////////////////////////////
	// 게임, 네트워크 초기화
	////////////////////////////////////////////////////////////////////////////////
	InitialGame();
	InitialNetwork(&client_sock, &g_hWnd);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 윈도우 사이즈 맞추기
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	SetFocus(g_hWnd);

	RECT WindowRect;
	WindowRect.top = 0;
	WindowRect.left = 0;
	WindowRect.right = 640;
	WindowRect.bottom = 480;

	AdjustWindowRectEx(&WindowRect, GetWindowStyle(g_hWnd), GetMenu(g_hWnd) != NULL, GetWindowExStyle(g_hWnd));

	int iX = (GetSystemMetrics(SM_CXSCREEN) - 640) / 2;
	int iY = (GetSystemMetrics(SM_CYSCREEN) - 480) / 2;

	MoveWindow(g_hWnd, iX, iY, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top, TRUE);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 기본 메시지 루프입니다.
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		else
		{
			Update_Game();
		}
	}

	return (int) msg.wParam;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	// 프로그램 비활성화 체크
	case WM_ACTIVATEAPP :
		g_bActiveApp = (BOOL)wParam;
		break;
		
	//네트워크 메시지
	case WM_NETWORK :
		if (WSAGETASYNCERROR(lParam))
			MessageBox(g_hWnd, L"WM_NETWORK", NULL, NULL);

		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_CONNECT :
			break;

		case FD_CLOSE :
			break;

		case FD_READ :
			ReadProc();
			break;

		case FD_WRITE :
			WriteProc();
			break;
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 그리기 코드를 추가합니다.
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;

	timeBeginPeriod(1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 게임 초기화
// - 스프라이트 생성과 로드 
// - 내 캐릭터 생성
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void InitialGame()
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// 스프라이트 로드
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	g_pSpriteDib = new CSpriteDib(eSPRITE_MAX, 0x00ffffff);

	g_pSpriteDib->LoadDibSprite(eMAP, L"Data\\Tile_01.bmp", 0, 0);

	// 맵 추가
	g_pSpriteDib->LoadDibSprite(ePLAYER_STAND_L01, L"Data\\Stand_L_01.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_STAND_L02, L"Data\\Stand_L_02.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_STAND_L03, L"Data\\Stand_L_03.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_STAND_L04, L"Data\\Stand_L_02.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_STAND_L05, L"Data\\Stand_L_01.bmp", 71, 90);

	g_pSpriteDib->LoadDibSprite(ePLAYER_STAND_R01, L"Data\\Stand_R_01.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_STAND_R02, L"Data\\Stand_R_02.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_STAND_R03, L"Data\\Stand_R_03.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_STAND_R04, L"Data\\Stand_R_02.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_STAND_R05, L"Data\\Stand_R_01.bmp", 71, 90);

	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_L01, L"Data\\MOVE_L_01.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_L02, L"Data\\MOVE_L_02.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_L03, L"Data\\MOVE_L_03.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_L04, L"Data\\MOVE_L_04.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_L05, L"Data\\MOVE_L_05.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_L06, L"Data\\MOVE_L_06.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_L07, L"Data\\MOVE_L_07.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_L08, L"Data\\MOVE_L_08.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_L09, L"Data\\MOVE_L_09.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_L10, L"Data\\MOVE_L_10.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_L11, L"Data\\MOVE_L_11.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_L12, L"Data\\MOVE_L_12.bmp", 71, 90);

	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_R01, L"Data\\MOVE_R_01.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_R02, L"Data\\MOVE_R_02.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_R03, L"Data\\MOVE_R_03.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_R04, L"Data\\MOVE_R_04.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_R05, L"Data\\MOVE_R_05.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_R06, L"Data\\MOVE_R_06.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_R07, L"Data\\MOVE_R_07.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_R08, L"Data\\MOVE_R_08.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_R09, L"Data\\MOVE_R_09.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_R10, L"Data\\MOVE_R_10.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_R11, L"Data\\MOVE_R_11.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_MOVE_R12, L"Data\\MOVE_R_12.bmp", 71, 90);

	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK1_L01, L"Data\\Attack1_L_01.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK1_L02, L"Data\\Attack1_L_02.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK1_L03, L"Data\\Attack1_L_03.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK1_L04, L"Data\\Attack1_L_04.bmp", 71, 90);

	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK1_R01, L"Data\\Attack1_R_01.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK1_R02, L"Data\\Attack1_R_02.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK1_R03, L"Data\\Attack1_R_03.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK1_R04, L"Data\\Attack1_R_04.bmp", 71, 90);

	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK2_L01, L"Data\\Attack2_L_01.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK2_L02, L"Data\\Attack2_L_02.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK2_L03, L"Data\\Attack2_L_03.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK2_L04, L"Data\\Attack2_L_04.bmp", 71, 90);

	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK2_R01, L"Data\\Attack2_R_01.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK2_R02, L"Data\\Attack2_R_02.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK2_R03, L"Data\\Attack2_R_03.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK2_R04, L"Data\\Attack2_R_04.bmp", 71, 90);

	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK3_L01, L"Data\\Attack3_L_01.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK3_L02, L"Data\\Attack3_L_02.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK3_L03, L"Data\\Attack3_L_03.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK3_L04, L"Data\\Attack3_L_04.bmp", 71, 90);

	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK3_R01, L"Data\\Attack3_R_01.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK3_R02, L"Data\\Attack3_R_02.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK3_R03, L"Data\\Attack3_R_03.bmp", 71, 90);
	g_pSpriteDib->LoadDibSprite(ePLAYER_ATTACK3_R04, L"Data\\Attack3_R_04.bmp", 71, 90);

	g_pSpriteDib->LoadDibSprite(eEFFECT_SPARK_01, L"Data\\xSpark_1.bmp", 70, 70);
	g_pSpriteDib->LoadDibSprite(eEFFECT_SPARK_02, L"Data\\xSpark_2.bmp", 70, 70);
	g_pSpriteDib->LoadDibSprite(eEFFECT_SPARK_03, L"Data\\xSpark_3.bmp", 70, 70);
	g_pSpriteDib->LoadDibSprite(eEFFECT_SPARK_04, L"Data\\xSpark_4.bmp", 70, 70);
	
	g_pSpriteDib->LoadDibSprite(eGUAGE_HP, L"Data\\HPGuage.bmp", 0, 0);

	g_pSpriteDib->LoadDibSprite(eSHADOW, L"Data\\Shadow.bmp", 32, 4);
	///////////////////////////////////////////////////////////////////////////////////////////////////////
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 게임 루프
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void Update_Game(void)
{
	timeBeginPeriod(1);

	if (g_bActiveApp)
		KeyProcess();

	Action();

	if (g_FrameSkip.FrameSkip())
	{
		if (g_dwStartTick == 0)	g_dwStartTick = timeGetTime();
		if (timeGetTime() - g_dwStartTick >= 1000)
		{
			if (g_pPlayerObject != NULL)
			{
				HDC hdc = GetDC(g_hWnd);
				WCHAR point[30];
				wsprintf(point, L"%d, %d, %d", g_pPlayerObject->GetCurX(), g_pPlayerObject->GetCurY(),
					g_dwFrameCount);
				TextOut(hdc, 0, 480, point, wcslen(point));
				ReleaseDC(g_hWnd, hdc);
			}
			g_dwFrameCount = 0;
			g_dwStartTick = timeGetTime();
		}
		g_dwFrameCount++;
		Draw();
		g_cScreenDib.DrawBuffer(g_hWnd);
	}

	timeBeginPeriod(1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 키 처리
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void KeyProcess()
{
	DWORD dwAction = dfACTION_STAND;

	if (g_pPlayerObject == NULL)	return;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)		dwAction = dfACTION_MOVE_LL;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)	dwAction = dfACTION_MOVE_RR;
	if (GetAsyncKeyState(VK_UP) & 0x8000)		dwAction = dfACTION_MOVE_UU;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)		dwAction = dfACTION_MOVE_DD;

	if ((GetAsyncKeyState(VK_LEFT) & 0x8001) && (GetAsyncKeyState(VK_UP) & 0x8001))
		dwAction = dfACTION_MOVE_LU;
	if ((GetAsyncKeyState(VK_LEFT) & 0x8001) && (GetAsyncKeyState(VK_DOWN) & 0x8001))
		dwAction = dfACTION_MOVE_LD;
	if ((GetAsyncKeyState(VK_RIGHT) & 0x8001) && (GetAsyncKeyState(VK_UP) & 0x8001))
		dwAction = dfACTION_MOVE_RU;
	if ((GetAsyncKeyState(VK_RIGHT) & 0x8001) && (GetAsyncKeyState(VK_DOWN) & 0x8001))
		dwAction = dfACTION_MOVE_RD;

	if (GetAsyncKeyState(0x5A) & 0x8001 || GetAsyncKeyState(0x5A) & 1)
		dwAction = dfACTION_ATTACK1;
	if (GetAsyncKeyState(0x58) & 0x8001 || GetAsyncKeyState(0x58) & 1)
		dwAction = dfACTION_ATTACK2;
	if (GetAsyncKeyState(0x43) & 0x8001 || GetAsyncKeyState(0x43) & 1)
		dwAction = dfACTION_ATTACK3;

	g_pPlayerObject->ActionInput(dwAction);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Action
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void Action()
{
	Object::iterator oIter;
	for (oIter = g_Object.begin(); oIter != g_Object.end(); )
	{
		CBaseObject *pBaseObject = (*oIter);
		
		if (!pBaseObject->Action(pBaseObject->GetObjectID()))	oIter++;
		else
		{
			delete pBaseObject;
			g_Object.erase(oIter++);
		}

		WriteProc();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// TileMap 좌표 지정
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	if (g_pPlayerObject != NULL)
		g_cTileMap.SetDrawPos(g_pPlayerObject->GetCurX(), g_pPlayerObject->GetCurY());

	//y축 정렬 시키기
	//이펙트는 제일 뒤로 밀기
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Draw
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void Draw()
{
	BYTE *bypDest = g_cScreenDib.GetDibBuffer();
	int iDestWidth = g_cScreenDib.GetWidth();
	int iDestHeight = g_cScreenDib.GetHeight();
	int iDestPitch = g_cScreenDib.GetPitch();	

	g_cTileMap.Draw(g_pSpriteDib, bypDest, iDestWidth, iDestHeight, iDestPitch);

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Y축 정렬
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	g_Object.sort(GreaterY());


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Effect, Player 정렬
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	g_Object.sort(EffectOrPlayer());

	Object::iterator oIter;
	for (oIter = g_Object.begin(); oIter != g_Object.end(); ++oIter)
		(*oIter)->Draw(g_pSpriteDib, bypDest, iDestWidth, iDestHeight, iDestPitch);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Packet 읽기
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL ReadProc()
{
	st_NETWORK_PACKET_HEADER Header;
	CNPacket cPacket;

	while (1){
		retval = recv(client_sock, RecvQ.GetWriteBufferPtr(), RecvQ.GetNotBrokenPutSize(), 0);

		if (retval == 0)
			return FALSE;

		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
				return FALSE;
			else
				MessageBox(g_hWnd, L"ReadProc", NULL, NULL);
		}
		RecvQ.MoveWritePos(retval);

		while (RecvQ.GetUseSize() > 0)
		{
			cPacket.Clear();

			//--------------------------------------------------------------------------------------*/
			//RecvQ 용량이 header보다 작은지 검사
			/*--------------------------------------------------------------------------------------*/
			if (RecvQ.GetUseSize() < sizeof(Header))
				return FALSE;

			RecvQ.Peek((char *)&Header, sizeof(Header));

			/*--------------------------------------------------------------------------------------*/
			//header code 검사
			/*--------------------------------------------------------------------------------------*/
			if (Header.byCode != (BYTE)0x89)
				return FALSE;

			/*--------------------------------------------------------------------------------------*/
			//header + payload 용량이 RecvQ용량보다 큰지 검사
			/*--------------------------------------------------------------------------------------*/
			if (RecvQ.GetUseSize() < Header.bySize + sizeof(Header))
				return FALSE;

			RecvQ.RemoveData(sizeof(Header));

			/*--------------------------------------------------------------------------------------*/
			//payload를 cPacket에 뽑고 같은지 검사
			/*--------------------------------------------------------------------------------------*/
			if (Header.bySize !=
				RecvQ.Get((char *)cPacket.GetBufferPtr(), Header.bySize))
				return FALSE;

			RecvQ.RemoveData(1);

			//TODO : 패킷의 endcode 검사해야됨

			switch (Header.byType)
			{
				//////////////////////////////////////////////////////////////////////////////////////////
				// 내 캐릭터 생성
				//////////////////////////////////////////////////////////////////////////////////////////
			case dfPACKET_SC_CREATE_MY_CHARACTER:
				recvProc_CreateMyCharacter(&cPacket);
				break;

				//////////////////////////////////////////////////////////////////////////////////////////
				// 다른 캐릭터 생성
				//////////////////////////////////////////////////////////////////////////////////////////
			case dfPACKET_SC_CREATE_OTHER_CHARACTER:
				recvProc_CreateOtherCharacter(&cPacket);
				break;

				/////////////////////////////////////////////////////////////////////////////////////////
				// 캐릭터 제거
				/////////////////////////////////////////////////////////////////////////////////////////
			case dfPACKET_SC_DELETE_CHARACTER:
				recvProc_DeleteCharacter(&cPacket);
				break;

				/////////////////////////////////////////////////////////////////////////////////////////
				// 움직임 시작
				/////////////////////////////////////////////////////////////////////////////////////////
			case dfPACKET_SC_MOVE_START:
				recvProc_MoveStart(&cPacket);
				break;

				/////////////////////////////////////////////////////////////////////////////////////////
				// 움직임 멈춤
				/////////////////////////////////////////////////////////////////////////////////////////
			case dfPACKET_SC_MOVE_STOP:
				recvProc_MoveStop(&cPacket);
				break;

				/////////////////////////////////////////////////////////////////////////////////////////
				// 공격 1
				/////////////////////////////////////////////////////////////////////////////////////////
			case dfPACKET_SC_ATTACK1:
				recvProc_Attack1(&cPacket);
				break;

				/////////////////////////////////////////////////////////////////////////////////////////
				// 공격 2
				/////////////////////////////////////////////////////////////////////////////////////////
			case dfPACKET_SC_ATTACK2:
				recvProc_Attack2(&cPacket);
				break;

				/////////////////////////////////////////////////////////////////////////////////////////
				// 공격 3
				/////////////////////////////////////////////////////////////////////////////////////////
			case dfPACKET_SC_ATTACK3:
				recvProc_Attack3(&cPacket);
				break;

				/////////////////////////////////////////////////////////////////////////////////////////
				// 데미지
				/////////////////////////////////////////////////////////////////////////////////////////
			case dfPACKET_SC_DAMAGE:
				recvProc_Damage(&cPacket);
				break;

			case dfPACKET_SC_SYNC:
				recvProc_Sync(&cPacket);
				break;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Packet 보내기
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void WriteProc()
{
	int retval;

	while (1){
		retval = send(client_sock, SendQ.GetReadBufferPtr(), SendQ.GetNotBrokenGetSize(), 0);

		if (retval == 0)	return;

		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return;
			}//에러처리
		}

		if (retval > 0)
			SendQ.RemoveData(retval);
	}
}