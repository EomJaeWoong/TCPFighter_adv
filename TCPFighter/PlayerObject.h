#ifndef __PLAYEROBJECT__H__
#define __PLAYEROBJECT__H__

class CPlayerObject : public CBaseObject
{
public :
	CPlayerObject(BOOL PlayerCharacter, int iObjectID, int iObjectType, char chHP, int iDirection);
	~CPlayerObject();

	DWORD Action(DWORD dwParam);
	void ActionProc();
	void Draw(CSpriteDib *pSprite, BYTE* dypDest, int iDestWidth, int iDestHeight, int iDestPitch);
	void InputActionProc();
	BOOL isPlayer();

	int GetDirection();

	void SetActionAttack1();
	void SetActionAttack2();
	void SetActionAttack3();
	void SetActionMove(DWORD actionMove);
	void SetActionStand();
	void SetDirection(int dir);
	void SetHP(char hp);

private :
	BOOL m_bPlayerCharacter;
	char m_chHP;
	DWORD m_dwActionCur;
	DWORD m_dwActionOld;
	int m_iDirCur;
	int m_iDirOld;
};

#endif