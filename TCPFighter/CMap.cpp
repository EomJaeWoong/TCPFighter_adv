#include "stdafx.h"
#include "CMap.h"

CMap::CMap()
{
	m_iMapWidth = dfMAP_WIDTH;
	m_iMapHeight = dfMAP_HEIGHT;

	m_iDrawPosX = 0;
	m_iDrawPosY = 0;

	memset(byTileMap, 0, sizeof(byTileMap));
}

CMap::CMap(int iMapWidth = 6400, int iMapHeight = 6400)
{
	m_iMapWidth = iMapWidth;
	m_iMapHeight = iMapHeight;

	memset(byTileMap, 0, sizeof(byTileMap));
}

CMap::~CMap()
{

}

/*------------------------------------------------------------------------------------------------*/
// 타일 그리기
/*------------------------------------------------------------------------------------------------*/
void CMap::Draw(CSpriteDib *pSprite, BYTE* bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	//----------------------------------------------------------------------------------------------
	// 그리기
	//----------------------------------------------------------------------------------------------
	for (int iCntY = 0; iCntY < 9; iCntY++)
	{
		for (int iCntX = 0; iCntX < 11; iCntX++)
		{
			pSprite->DrawImage(eMAP, (iCntX * 64) - (m_iDrawPosX % 64), iCntY * 64 - (m_iDrawPosY % 64),
				bypDest, iDestWidth, iDestHeight, iDestPitch);
		}
	}
}

/*------------------------------------------------------------------------------------------------*/
// ScrollX, Y 설정
/*------------------------------------------------------------------------------------------------*/
void CMap::SetDrawPos(int iDrawPosX, int iDrawPosY)
{
	m_iDrawPosX = iDrawPosX - dfSCREEN_WIDTH / 2;
	m_iDrawPosY = iDrawPosY - dfSCREEN_HEIGHT / 2 - 20;

	if (iDrawPosX < dfSCREEN_WIDTH / 2)					m_iDrawPosX = 0;
	if (iDrawPosY < dfSCREEN_HEIGHT / 2 + 20)			m_iDrawPosY = 0;
	if (iDrawPosX + dfSCREEN_WIDTH / 2 > dfMAP_WIDTH)
		m_iDrawPosX = dfMAP_WIDTH - dfSCREEN_WIDTH;
	if (iDrawPosY + dfSCREEN_HEIGHT / 2 - 20 > dfMAP_HEIGHT)
		m_iDrawPosY = dfMAP_HEIGHT - dfSCREEN_HEIGHT;
}