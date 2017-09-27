/*------------------------------------------------------------------
ScreenDib.h
------------------------------------------------------------------*/

#ifndef __SCREEN__DIB__
#define __SCREEN__DIB__

/*------------------------------------------------------------------
Dib를 사용한 GDI용 스크린 버퍼.
윈도우의 HDC에 DIB를 사용하여 그림을 찍어준다.
------------------------------------------------------------------*/

class CScreenDib
{
public:
	////////////////////////////////////////////////////////////////
	// 생성자, 파괴자
	////////////////////////////////////////////////////////////////
	CScreenDib(int iWidth, int iHeight, int iColorBit);
	virtual ~CScreenDib();

protected:
	void CreateDibBuffer(int iWidth, int iHeight, int iColorBit);
	void ReleaseDibBuffer(void);

public:
	void DrawBuffer(HWND hWnd, int iX = 0, int iY = 0);

	BYTE *GetDibBuffer(void);
	int GetWidth(void);
	int GetHeight(void);
	int GetPitch(void);

protected:
	BITMAPINFO	m_stDibInfo;
	BYTE			*m_bypBuffer;

	int			m_iWidth;
	int			m_iHeight;
	int			m_iPitch;
	int			m_iColorBit;
	int			m_iBufferSize;
};

#endif