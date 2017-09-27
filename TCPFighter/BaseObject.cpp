#include "stdafx.h"
#include "BaseObject.h"

/////////////////////////////////////////////////////////////////////////
// 생성자, 파괴자
/////////////////////////////////////////////////////////////////////////
CBaseObject::CBaseObject(int iObjectID, int iObjectType)
	: m_iObjectID(iObjectID), m_iObjectType(iObjectType)
{

}

CBaseObject::~CBaseObject()
{

}

void CBaseObject::ActionInput(DWORD dwAction)
{
	m_dwActionInput = dwAction;
}

void CBaseObject::NextFrame()
{
	if (0 > m_iSpriteStart)
		return;

	m_iDelayCount++;

	if (m_iDelayCount >= m_iFrameDelay)
	{
		m_iDelayCount = 0;

		m_iSpriteNow++;

		////////////////////////////////////
		// 애니메이션 처음으로 돌리기
		////////////////////////////////////
		if (m_iSpriteNow > m_iSpriteEnd)
		{
			m_iSpriteNow = m_iSpriteStart;
			m_bEndFrame = TRUE;
		}
	}
}

BOOL CBaseObject::isEndFrame()
{
	return m_bEndFrame;
}

/////////////////////////////////////////////////////////////////////////
// Getter
/////////////////////////////////////////////////////////////////////////
int CBaseObject::GetCurX()			{ return m_iCurX; }
int CBaseObject::GetCurY()			{ return m_iCurY; }
int CBaseObject::GetObjectID()		{ return m_iObjectID; }
int CBaseObject::GetObjectType()	{ return m_iObjectType; }
int CBaseObject::GetOldX()			{ return m_iOldX; }
int CBaseObject::GetOldY()			{ return m_iOldY; }
int CBaseObject::GetSprite()		{ return m_iSpriteNow; }

/////////////////////////////////////////////////////////////////////////
// Setter
/////////////////////////////////////////////////////////////////////////
void CBaseObject::SetCurPosition(int curX, int curY)
{
	m_iCurX = curX;
	m_iCurY = curY;
}

void CBaseObject::SetObjectID(int objectID)
{
	m_iObjectID = objectID;
}

void CBaseObject::SetObjectType(e_OBJECT_TYPE ObjectType)
{
	m_iObjectType = ObjectType;
}

void CBaseObject::SetOldPosition(int oldX, int oldY)
{
	m_iOldX = oldX;
	m_iOldY = oldY;
}

void CBaseObject::SetPosition(int x, int y)
{
	SetOldPosition(m_iCurX, m_iCurY);
	SetCurPosition(x, y);
}

void CBaseObject::SetSprite(int iSpriteStart, int iSpriteEnd, int iFrameDelay)
{
	m_iSpriteStart = iSpriteStart;
	m_iSpriteEnd = iSpriteEnd;
	m_iFrameDelay = iFrameDelay;

	m_iSpriteNow = iSpriteStart;
	m_iDelayCount = 0;
	m_bEndFrame = FALSE;
}