#ifndef __SPRITEDIB__H__
#define __SPRITEDIB__H__

class CSpriteDib
{
public:
	//////////////////////////////////////////////////////////////////////////////
	// Sprite ����ü
	//////////////////////////////////////////////////////////////////////////////
	typedef struct st_SPRITE
	{
		BYTE		*byplmage;
		int		iWidth;
		int		iHeight;
		int		iPitch;
		int		iCenterPointX;	// ���� x
		int		iCenterPointY;	// ���� y
	};

	//////////////////////////////////////////////////////////////////////////////
	// ������, �ı���
	//////////////////////////////////////////////////////////////////////////////
	CSpriteDib(int iMaxSprite, DWORD dwColorKey);
	virtual ~CSpriteDib();

	//////////////////////////////////////////////////////////////////////////////
	// Sprite ����, �ı�
	//////////////////////////////////////////////////////////////////////////////
	BOOL LoadDibSprite(int iSpriteIndex, WCHAR *szFileName, int iCenterPointX, int iCenterPointY);
	void ReleaseSprite(int iSpriteIndex);

	//////////////////////////////////////////////////////////////////////////////
	// Sprite �׸���(�������)
	// 1. �״�� ���
	// 2. ������
	// 3. �Ӱ� ���
	// 4. �̹��� ��ü ���
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