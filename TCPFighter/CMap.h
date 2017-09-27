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
	// 화면에 맵이 그려지는 시작위치
	//////////////////////////////////////////////////////////////////////////
	int m_iDrawPosX;
	int m_iDrawPosY;

	//////////////////////////////////////////////////////////////////////////
	// 실제 맵 크기
	//////////////////////////////////////////////////////////////////////////
	int m_iMapWidth;
	int m_iMapHeight;
};

#endif