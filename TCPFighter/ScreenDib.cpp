#include "stdafx.h"
#include "ScreenDib.h"

////////////////////////////////////////////////////////////////
// ������, �ı���
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
// Return : ����
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
	�Է¹��� ������ ������ ������� ���� ����,
	BITMAPINFO(BITMAPINFOHEADER)�� �����Ѵ�.

	�̹��� ����� ����Ͽ� ���ۿ� �̹��� ���� �Ҵ�.
	*/
}

////////////////////////////////////////////////////////////////
// DibBuffer Release.
// Parameters : ����.
// return : ����.
////////////////////////////////////////////////////////////////
void CScreenDib::ReleaseDibBuffer(void)
{
	free(m_bypBuffer);
	//�޸� ����~
}

////////////////////////////////////////////////////////////////
// DC�� ��´�
// Parameters : (HWND)������ �ڵ�, (int)(int)��ġ.
// Return : ����.
////////////////////////////////////////////////////////////////
void CScreenDib::DrawBuffer(HWND hWnd, int iX, int iY)
{
	// �Է¹��� hWnd �ڵ��� DC�� ��
	// DC�� X, Y ��ġ�� ��ũ�� ���� DIB�� ����Ѵ�.
	HDC hdc;
	hdc = GetDC(hWnd);
	SetDIBitsToDevice(hdc, 0, 0, m_iWidth, m_iHeight, 0, 0, 0, m_iHeight, m_bypBuffer, &m_stDibInfo, DIB_RGB_COLORS);
	ReleaseDC(hWnd, hdc);
}

////////////////////////////////////////////////////////////////
// ��ũ�� ���� ���.
// Parameters : ����
////////////////////////////////////////////////////////////////
BYTE * CScreenDib::GetDibBuffer(void)
{
	return m_bypBuffer;
}

int CScreenDib::GetWidth(void) { return m_iWidth; }
int CScreenDib::GetHeight(void) { return m_iHeight; }
int CScreenDib::GetPitch(void) { return m_iPitch; }
