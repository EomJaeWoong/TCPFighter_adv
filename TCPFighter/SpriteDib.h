#ifndef __SPRITEDIB__H__
#define __SPRITEDIB__H__

class CSpriteDib
{
public:
	//////////////////////////////////////////////////////////////////////////////
	// Sprite 구조체
	//////////////////////////////////////////////////////////////////////////////
	typedef struct st_SPRITE
	{
		BYTE		*byplmage;
		int		iWidth;
		int		iHeight;
		int		iPitch;
		int		iCenterPointX;	// 중점 x
		int		iCenterPointY;	// 중점 y
	};

	//////////////////////////////////////////////////////////////////////////////
	// 생성자, 파괴자
	//////////////////////////////////////////////////////////////////////////////
	CSpriteDib(int iMaxSprite, DWORD dwColorKey);
	virtual ~CSpriteDib();

	//////////////////////////////////////////////////////////////////////////////
	// Sprite 생성, 파괴
	//////////////////////////////////////////////////////////////////////////////
	BOOL LoadDibSprite(int iSpriteIndex, WCHAR *szFileName, int iCenterPointX, int iCenterPointY);
	void ReleaseSprite(int iSpriteIndex);

	//////////////////////////////////////////////////////////////////////////////
	// Sprite 그리기(방법별로)
	// 1. 그대로 출력
	// 2. 반투명
	// 3. 붉게 출력
	// 4. 이미지 자체 출력
	//////////////////////////////////////////////////////////////////////////////
	void DrawSprite(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest,
		int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen = 100);
	void DrawSprite50(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest,
		int iDestWidth, int iDestHeight, int iDestPitch);
	void DrawSpriteRed(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest,
		int iDestWidth, int iDestHeight, int iDestPitch);
	void DrawImage(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest,
		int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen = 100);

protected :
	int			m_iMaxSprite;
	st_SPRITE	*m_stpSprite;
	DWORD		m_dwColorKey;
};

#endif