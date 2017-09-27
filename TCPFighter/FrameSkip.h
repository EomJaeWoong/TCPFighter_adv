#ifndef __FRAMESKIP__H__
#define __FRAMESKIP__H__

class CFrameSkip
{
public :
	CFrameSkip(int iMaxFPS);
	~CFrameSkip();
	BOOL FrameSkip();

private :
	int m_dwSystemTick;
	int m_iMaxFPS;
	int m_iOneFrameTick;
	int m_iTick;
};

#endif