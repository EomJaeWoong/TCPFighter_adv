#ifndef __CMAP__H__
#define __CMAP__H__

#define dfMAP_WIDTH		6400
#define dfMAP_HEIGHT	6400

#define TILE_X			dfMAP_WIDTH / 64
#define TILE_Y			dfMAP_HEIGHT / 64

class CMap
{
public :
	void Draw(CSpriteDib *pSprite, BYTE* bypDest, int iDestWidth, int iDestHeight, int iDestPitch);
	void SetDrawPos(int iDrawPosX, int iDrawPosY);

	int GetDrawPosX(){ return m_iDrawPosX;  }
	int GetDrawPosY(){ return m_iDrawPosY;	}

	CMap();
	CMap(int iMapWidth, int iMapHeight);
	virtual ~CMap();

private :
	BYTE byTileMap[TILE_Y][TILE_X];

	//////////////////////////////////////////////////////////////////////////
	// Scroll X, Y
	// ȭ�鿡 ���� �׷����� ������ġ
	//////////////////////////////////////////////////////////////////////////
	int m_iDrawPosX;
	int m_iDrawPosY;

	//////////////////////////////////////////////////////////////////////////
	// ���� �� ũ��
	//////////////////////////////////////////////////////////////////////////
	int m_iMapWidth;
	int m_iMapHeight;
};

#endif