#ifndef __EFFECTOBJECT__H__
#define __EFFECTOBJECT__H__

class CEffectObject : public CBaseObject
{
public :
	CEffectObject(int iObjectID, int iObjectType, int iX, int iY,
		int iFrameDelay, int iSpriteStart, int iSpriteEnd);
	~CEffectObject();

	DWORD Action(DWORD dwParam);
	void Draw(CSpriteDib *pSprite, BYTE* bypDest, int iDestWidth, int iDestHeight, int iDestPitch);

private :
	BOOL m_bEffectStart;
	DWORD m_dwAttackID;
};

#endif