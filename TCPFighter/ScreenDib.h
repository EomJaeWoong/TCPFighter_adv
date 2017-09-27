/*------------------------------------------------------------------
ScreenDib.h
------------------------------------------------------------------*/

#ifndef __SCREEN__DIB__
#define __SCREEN__DIB__

/*------------------------------------------------------------------
Dib�� ����� GDI�� ��ũ�� ����.
�������� HDC�� DIB�� ����Ͽ� �׸��� ����ش�.
------------------------------------------------------------------*/

class CScreenDib
{
public:
	////////////////////////////////////////////////////////////////
	// ������, �ı���
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