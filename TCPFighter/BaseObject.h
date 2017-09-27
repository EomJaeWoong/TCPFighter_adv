#ifndef __BASEOBJECT__H__
#define __BASEOBJECT__H__

class CBaseObject
{
public :
	/////////////////////////////////////////////////////////////////////////
	// 생성자, 파괴자
	/////////////////////////////////////////////////////////////////////////
	CBaseObject(int iObjectID, int iObjectType);
	~CBaseObject();

	void ActionInput(DWORD dwAction);
	void NextFrame();
	BOOL isEndFrame();

	/////////////////////////////////////////////////////////////////////////
	// Getter
	/////////////////////////////////////////////////////////////////////////
	int GetCurX();
	int GetCurY();
	int GetObjectID();
	int GetObjectType();
	int GetOldX();
	int GetOldY();
	int GetSprite();

	/////////////////////////////////////////////////////////////////////////
	// Setter
	/////////////////////////////////////////////////////////////////////////
	void SetCurPosition(int curX, int curY);
	void SetObjectID(int objectID);
	void SetObjectType(e_OBJECT_TYPE ObjectType);
	void SetOldPosition(int oldX, int oldY);
	void SetPosition(int x, int y);
	void SetSprite(int iSpriteStart, int iSpriteMax, int iFrameDelay);

	/////////////////////////////////////////////////////////////////////////
	// Action, Draw 가상함수
	/////////////////////////////////////////////////////////////////////////
	virtual DWORD Action(DWORD dwParam) = 0;
	virtual void Draw(CSpriteDib *pSprite, BYTE* dypDest, int iDestWidth, int iDestHeight, int iDestPitch) = 0;

protected :
	BOOL m_bEndFrame;
	DWORD m_dwActionInput;
	int m_iCurX;
	int m_iCurY;
	int m_iDelayCount;
	int m_iFrameDelay;
	int m_iObjectID;
	int m_iObjectType;
	int m_iOldX;
	int m_iOldY;
	int m_iSpriteEnd;
	int m_iSpriteNow;
	int m_iSpriteStart;
};

#endif