#include "stdafx.h"
#include "ScreenDib.h"

////////////////////////////////////////////////////////////////
// 생성자, 파괴자
////////////////////////////////////////////////////////////////
CScreenDib::CScreenDib(int iWidth, int iHeight, int iColorBit)
{
	CreateDibBuffer(iWidth, iHeight, iColorBit);
}

CScreenDib::~CScreenDib()
{
	ReleaseDibBuffer();
}

////////////////////////////////////////////////////////////////
// DibBuffer Create.
// Parameters: (int)Width, (int)Height, (int)ColorBit
// Return : 없음
////////////////////////////////////////////////////////////////
void CScreenDib::CreateDibBuffer(int iWidth, int iHeight, int iColorBit)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_iColorBit = iColorBit;
	m_iPitch = (iWidth * (iColorBit/8) + 3) & ~3;

	m_stDibInfo.bmiHeader.biSize = sizeof(m_stDibInfo);
	m_stDibInfo.bmiHeader.biWidth = iWidth;
	m_stDibInfo.bmiHeader.biHeight = -iHeight;
	m_stDibInfo.bmiHeader.biPlanes = 1;
	m_stDibInfo.bmiHeader.biBitCount = iColorBit;
	m_stDibInfo.bmiHeader.biCompression = BI_RGB(0);
	m_stDibInfo.bmiHeader.biSizeImage = 0;
	m_stDibInfo.bmiHeader.biXPelsPerMeter = 0;
	m_stDibInfo.bmiHeader.biYPelsPerMeter = 0;
	m_stDibInfo.bmiHeader.biClrUsed = 0;
	m_stDibInfo.bmiHeader.biClrImportant = 0;

	m_bypBuffer = (BYTE*)malloc(m_iPitch * m_iHeight);
	/*
	입력받은 인자의 정보로 멤버변수 정보 셋팅,
	BITMAPINFO(BITMAPINFOHEADER)를 셋팅한다.

	이미지 사이즈를 계산하여 버퍼용 이미지 동적 할당.
	*/
}

////////////////////////////////////////////////////////////////
// DibBuffer Release.
// Parameters : 없음.
// return : 없음.
////////////////////////////////////////////////////////////////
void CScreenDib::ReleaseDibBuffer(void)
{
	free(m_bypBuffer);
	//메모리 해제~
}

////////////////////////////////////////////////////////////////
// DC에 찍는다
// Parameters : (HWND)윈도우 핸들, (int)(int)위치.
// Return : 없음.
////////////////////////////////////////////////////////////////
void CScreenDib::DrawBuffer(HWND hWnd, int iX, int iY)
{
	// 입력받은 hWnd 핸들의 DC를 얻어서
	// DC의 X, Y 위치에 스크린 버퍼 DIB를 출력한다.
	HDC hdc;
	hdc = GetDC(hWnd);
	SetDIBitsToDevice(hdc, 0, 0, m_iWidth, m_iHeight, 0, 0, 0, m_iHeight, m_bypBuffer, &m_stDibInfo, DIB_RGB_COLORS);
	ReleaseDC(hWnd, hdc);
}

////////////////////////////////////////////////////////////////
// 스크린 정보 얻기.
// Parameters : 없음
////////////////////////////////////////////////////////////////
BYTE * CScreenDib::GetDibBuffer(void)
{
	return m_bypBuffer;
}

int CScreenDib::GetWidth(void) { return m_iWidth; }
int CScreenDib::GetHeight(void) { return m_iHeight; }
int CScreenDib::GetPitch(void) { return m_iPitch; }
