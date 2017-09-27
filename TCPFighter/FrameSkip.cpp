#include "stdafx.h"
#include "FrameSkip.h"

CFrameSkip::CFrameSkip(int iMaxFPS)
	:m_iMaxFPS(1000 / iMaxFPS)
{
	m_dwSystemTick = 0;
	m_iTick = 0;
}

CFrameSkip::~CFrameSkip()
{

}

BOOL CFrameSkip::FrameSkip()
{
	if (m_dwSystemTick == 0)
		m_dwSystemTick = timeGetTime();

	m_iOneFrameTick = timeGetTime() - m_dwSystemTick;

	if (m_iTick > m_iMaxFPS)
	{
		m_iTick -= m_iMaxFPS;
		return FALSE;
	}

	if (m_iOneFrameTick < m_iMaxFPS)	Sleep((m_iMaxFPS - m_iOneFrameTick));
	else
	{
		m_iTick = m_iOneFrameTick - m_iMaxFPS;
		Sleep(m_iMaxFPS);
	}

	m_dwSystemTick = timeGetTime();
	
	return TRUE;
}