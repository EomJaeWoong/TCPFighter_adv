#include "stdafx.h"
#include "SpriteDib.h"


//////////////////////////////////////////////////////////////////////////////
// ������, �ı���
//////////////////////////////////////////////////////////////////////////////
CSpriteDib::CSpriteDib(int iMaxSprite, DWORD dwColorKey)
	:m_iMaxSprite(iMaxSprite), m_dwColorKey(dwColorKey)
{
	m_stpSprite = new st_SPRITE[m_iMaxSprite];
}

CSpriteDib::~CSpriteDib()
{
	for (int iCount = 0; iCount > m_iMaxSprite; iCount++)
	{
		ReleaseSprite(iCount);
	}
}

//////////////////////////////////////////////////////////////////////////////
// Sprite ����, �ı�
//////////////////////////////////////////////////////////////////////////////
BOOL CSpriteDib::LoadDibSprite(int iSpriteIndex, WCHAR *szFileName, int iCenterPointX, int iCenterPointY)
{
	HANDLE			hFile;
	DWORD			dwRead;
	int				iPitch;
	int				iImageSize;
	BITMAPFILEHEADER	stFileHeader;
	BITMAPINFOHEADER	stInfoHeader;
	BYTE				*bypTempImage;
	DWORD *pDest, *pSrc;
	BYTE *pDestOrigin, *pSrcOrigin;

	hFile = CreateFile(szFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)	return FALSE;

	ReadFile(hFile, &stFileHeader, sizeof(BITMAPFILEHEADER), &dwRead, NULL);

	if (stFileHeader.bfType == 0x4D42){	
		ReadFile(hFile, &stInfoHeader, sizeof(BITMAPINFOHEADER), &dwRead, NULL);

		iPitch = (stInfoHeader.biWidth * (stInfoHeader.biBitCount / 8) + 3) & ~3;
		
		m_stpSprite[iSpriteIndex].iHeight = stInfoHeader.biHeight;
		m_stpSprite[iSpriteIndex].iWidth = stInfoHeader.biWidth;
		m_stpSprite[iSpriteIndex].iPitch = iPitch;
		m_stpSprite[iSpriteIndex].iCenterPointX = iCenterPointX;
		m_stpSprite[iSpriteIndex].iCenterPointY = iCenterPointY;

		iImageSize = m_stpSprite[iSpriteIndex].iHeight * iPitch;

		m_stpSprite[iSpriteIndex].byplmage = new BYTE[iImageSize];

		bypTempImage = new BYTE[iImageSize];

		ReadFile(hFile, bypTempImage, iImageSize, &dwRead, NULL);
		
		pDest = (DWORD*)m_stpSprite[iSpriteIndex].byplmage;
		pSrc = (DWORD*)(bypTempImage + (iImageSize - iPitch));
		pDestOrigin = (BYTE*)pDest;
		pSrcOrigin = (BYTE*)pSrc;

		for (int iSpriteHeight = 0; iSpriteHeight < stInfoHeader.biHeight; iSpriteHeight++)
		{
			pDest = (DWORD*)pDestOrigin;
			pSrc = (DWORD*)pSrcOrigin;

			for (int iSpriteWidth = 0; iSpriteWidth < stInfoHeader.biWidth; iSpriteWidth++)
			{
				*pDest = *pSrc;
				pDest++;
				pSrc++;
			}
			pDestOrigin += iPitch;
			pSrcOrigin -= iPitch;
		}
		
		delete[] bypTempImage;

		CloseHandle(hFile);
		return TRUE;
	}
	else{
		return FALSE;
	}
}

void CSpriteDib::ReleaseSprite(int iSpriteIndex)
{
	if (m_iMaxSprite <= iSpriteIndex)	return;

	if (NULL != m_stpSprite[iSpriteIndex].byplmage)
	{
		delete[] m_stpSprite[iSpriteIndex].byplmage;
		memset(&m_stpSprite[iSpriteIndex], 0, sizeof(st_SPRITE));
	}
}

//////////////////////////////////////////////////////////////////////////////
// Sprite �׸���(�������)
// 1. �״�� ���
// 2. ������
// 3. �Ӱ� ���
// 4. �̹��� ��ü ���
//////////////////////////////////////////////////////////////////////////////
void CSpriteDib::DrawSprite(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest,
	int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen)
{
	//////////////////////////////////////////////////////////////////////////////
	// �̹��� ������
	//////////////////////////////////////////////////////////////////////////////
	DWORD *pSrc = (DWORD*)m_stpSprite[iSpriteIndex].byplmage;
	DWORD *pDest = (DWORD*)bypDest + (iDrawX + (iDrawY * iDestWidth)) - 
		(m_stpSprite[iSpriteIndex].iCenterPointX + (m_stpSprite[iSpriteIndex].iCenterPointY * iDestWidth));
	BYTE *pDestOrigin = (BYTE*)pDest;
	BYTE *pSrcOrigin = (BYTE*)pSrc;
	
	//////////////////////////////////////////////////////////////////////////////
	// Ŭ���� ����
	//////////////////////////////////////////////////////////////////////////////
	
	int iDrawPosX = iDrawX - m_stpSprite[iSpriteIndex].iCenterPointX;
	int iDrawPosY = iDrawY - m_stpSprite[iSpriteIndex].iCenterPointY;
	int iSpriteWidth = m_stpSprite[iSpriteIndex].iWidth;
	int iSpriteHeight = m_stpSprite[iSpriteIndex].iHeight;

	if (iSpriteIndex >= m_iMaxSprite || m_stpSprite->byplmage == NULL)	return;

	//////////////////////////////////////////////////////////////////////////////
	// Ŭ���� ó��
	//////////////////////////////////////////////////////////////////////////////

	if (iDrawPosX + m_stpSprite[iSpriteIndex].iWidth > iDestWidth)		//������
	{
		iSpriteWidth = iDestWidth - iDrawPosX;
	}

	if (iDrawPosY + m_stpSprite[iSpriteIndex].iHeight > iDestHeight)	//�Ʒ�
	{
		iSpriteHeight = iDestHeight - iDrawPosY;
	}

	if (iDrawPosY < 0)													//��
	{
		iDrawPosY = abs(iDrawPosY);
		iSpriteHeight -= iDrawPosY;
		pSrcOrigin += (iDrawPosY * m_stpSprite[iSpriteIndex].iWidth) * 4;
		pDestOrigin += (iDrawPosY * iDestWidth) * 4;
	}

	if (iDrawPosX < 0)													//����
	{
		iDrawPosX = abs(iDrawPosX);
		iSpriteWidth -= iDrawPosX;
		pSrcOrigin += iDrawPosX * 4;
		pDestOrigin += iDrawPosX * 4;
	}

	//////////////////////////////////////////////////////////////////////////////
	// ���̿� ���� ��� ó��
	//////////////////////////////////////////////////////////////////////////////
	iSpriteWidth = (int)(iSpriteWidth * iDrawLen / 100);

	//////////////////////////////////////////////////////////////////////////////
	// �׸� �׸���
	//////////////////////////////////////////////////////////////////////////////
	for (int iCntHeight = 0; iCntHeight < iSpriteHeight; iCntHeight++)
	{
		pDest = (DWORD*)pDestOrigin;
		pSrc = (DWORD*)pSrcOrigin;
		for (int iCntWidth = 0; iCntWidth < iSpriteWidth; iCntWidth++)
		{
			if ((*pSrc & 0x00ffffff) != 0x00ffffff)
				*pDest = *pSrc;

			pDest++;
			pSrc++;
		}

		pDestOrigin += iDestPitch;
		pSrcOrigin += m_stpSprite[iSpriteIndex].iPitch;
	}
}

void CSpriteDib::DrawSprite50(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest,
	int iDestWidth, int iDestHeight, int iDestPitch)
{
	//////////////////////////////////////////////////////////////////////////////
	// �̹��� ������
	//////////////////////////////////////////////////////////////////////////////
	DWORD *pSrc = (DWORD*)m_stpSprite[iSpriteIndex].byplmage;
	DWORD *pDest = (DWORD*)bypDest + (iDrawX + (iDrawY * iDestWidth)) -
		(m_stpSprite[iSpriteIndex].iCenterPointX + (m_stpSprite[iSpriteIndex].iCenterPointY * iDestWidth));
	BYTE *pDestOrigin = (BYTE*)pDest;
	BYTE *pSrcOrigin = (BYTE*)pSrc;

	//////////////////////////////////////////////////////////////////////////////
	// Ŭ���� ����
	//////////////////////////////////////////////////////////////////////////////

	int iDrawPosX = iDrawX - m_stpSprite[iSpriteIndex].iCenterPointX;
	int iDrawPosY = iDrawY - m_stpSprite[iSpriteIndex].iCenterPointY;
	int iSpriteWidth = m_stpSprite[iSpriteIndex].iWidth;
	int iSpriteHeight = m_stpSprite[iSpriteIndex].iHeight;

	if (iSpriteIndex >= m_iMaxSprite || m_stpSprite->byplmage == NULL)	return;

	//////////////////////////////////////////////////////////////////////////////
	// Ŭ���� ó��
	//////////////////////////////////////////////////////////////////////////////

	if (iDrawPosX + m_stpSprite[iSpriteIndex].iWidth > iDestWidth)		//������
	{
		iSpriteWidth = iDestWidth - iDrawPosX;
	}

	if (iDrawPosY + m_stpSprite[iSpriteIndex].iHeight > iDestHeight)	//�Ʒ�
	{
		iSpriteHeight = iDestHeight - iDrawPosY;
	}

	if (iDrawPosY < 0)													//��
	{
		iDrawPosY = abs(iDrawPosY);
		iSpriteHeight -= iDrawPosY;
		pSrcOrigin += (iDrawPosY * m_stpSprite[iSpriteIndex].iWidth) * 4;
		pDestOrigin += (iDrawPosY * iDestWidth) * 4;
	}

	if (iDrawPosX < 0)													//����
	{
		iDrawPosX = abs(iDrawPosX);
		iSpriteWidth -= iDrawPosX;
		pSrcOrigin += iDrawPosX * 4;
		pDestOrigin += iDrawPosX * 4;
	}

	//////////////////////////////////////////////////////////////////////////////
	// �׸� �׸���
	//////////////////////////////////////////////////////////////////////////////
	for (int iCntHeight = 0; iCntHeight < iSpriteHeight; iCntHeight++)
	{
		pDest = (DWORD*)pDestOrigin;
		pSrc = (DWORD*)pSrcOrigin;
		for (int iCntWidth = 0; iCntWidth < iSpriteWidth; iCntWidth++)
		{
			if ((*pSrc & 0x00ffffff) != 0x00ffffff)
			{
				*((BYTE*)pDest + 0) = *((BYTE*)pSrc + 0) / 2 + *((BYTE*)pDest + 0) / 2;
				*((BYTE*)pDest + 1) = *((BYTE*)pSrc + 1) / 2 + *((BYTE*)pDest + 1) / 2;
				*((BYTE*)pDest + 2) = *((BYTE*)pSrc + 2) / 2 + *((BYTE*)pDest + 2) / 2;
				*((BYTE*)pDest + 3) = *((BYTE*)pSrc + 3) / 2 + *((BYTE*)pDest + 3) / 2;
				//*pDest = *pSrc;
			}

			pDest++;
			pSrc++;
		}

		pDestOrigin += iDestPitch;
		pSrcOrigin += m_stpSprite[iSpriteIndex].iPitch;
	}
}

void CSpriteDib::DrawSpriteRed(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest,
	int iDestWidth, int iDestHeight, int iDestPitch)
{
	//////////////////////////////////////////////////////////////////////////////
	// �̹��� ������
	//////////////////////////////////////////////////////////////////////////////
	DWORD *pSrc = (DWORD*)m_stpSprite[iSpriteIndex].byplmage;
	DWORD *pDest = (DWORD*)bypDest + (iDrawX + (iDrawY * iDestWidth)) -
		(m_stpSprite[iSpriteIndex].iCenterPointX + (m_stpSprite[iSpriteIndex].iCenterPointY * iDestWidth));
	BYTE *pDestOrigin = (BYTE*)pDest;
	BYTE *pSrcOrigin = (BYTE*)pSrc;

	//////////////////////////////////////////////////////////////////////////////
	// Ŭ���� ����
	//////////////////////////////////////////////////////////////////////////////

	int iDrawPosX = iDrawX - m_stpSprite[iSpriteIndex].iCenterPointX;
	int iDrawPosY = iDrawY - m_stpSprite[iSpriteIndex].iCenterPointY;
	int iSpriteWidth = m_stpSprite[iSpriteIndex].iWidth;
	int iSpriteHeight = m_stpSprite[iSpriteIndex].iHeight;

	if (iSpriteIndex >= m_iMaxSprite || m_stpSprite->byplmage == NULL)	return;

	//////////////////////////////////////////////////////////////////////////////
	// Ŭ���� ó��
	//////////////////////////////////////////////////////////////////////////////

	if (iDrawPosX + m_stpSprite[iSpriteIndex].iWidth > iDestWidth)		//������
	{
		iSpriteWidth = iDestWidth - iDrawPosX;
	}

	if (iDrawPosY + m_stpSprite[iSpriteIndex].iHeight > iDestHeight)	//�Ʒ�
	{
		iSpriteHeight = iDestHeight - iDrawPosY;
	}

	if (iDrawPosY < 0)													//��
	{
		iDrawPosY = abs(iDrawPosY);
		iSpriteHeight -= iDrawPosY;
		pSrcOrigin += (iDrawPosY * m_stpSprite[iSpriteIndex].iWidth) * 4;
		pDestOrigin += (iDrawPosY * iDestWidth) * 4;
	}

	if (iDrawPosX < 0)													//����
	{
		iDrawPosX = abs(iDrawPosX);
		iSpriteWidth -= iDrawPosX;
		pSrcOrigin += iDrawPosX * 4;
		pDestOrigin += iDrawPosX * 4;
	}

	//////////////////////////////////////////////////////////////////////////////
	// �׸� �׸���
	//////////////////////////////////////////////////////////////////////////////
	for (int iCntHeight = 0; iCntHeight < iSpriteHeight; iCntHeight++)
	{
		pDest = (DWORD*)pDestOrigin;
		pSrc = (DWORD*)pSrcOrigin;
		for (int iCntWidth = 0; iCntWidth < iSpriteWidth; iCntWidth++)
		{
			if ((*pSrc & 0x00ffffff) != 0x00ffffff)
			{
				*((BYTE*)pDest + 0) = *((BYTE*)pSrc + 0) / 2;
				*((BYTE*)pDest + 1) = *((BYTE*)pSrc + 1) / 2;
				*((BYTE*)pDest + 2) = *((BYTE*)pSrc + 2);
				*((BYTE*)pDest + 3) = *((BYTE*)pSrc + 3);
				//*pDest = *pSrc;
			}

			pDest++;
			pSrc++;
		}

		pDestOrigin += iDestPitch;
		pSrcOrigin += m_stpSprite[iSpriteIndex].iPitch;
	}
}

void CSpriteDib::DrawImage(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest,
	int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen)
{
	//////////////////////////////////////////////////////////////////////////////
	// �̹��� ������
	//////////////////////////////////////////////////////////////////////////////
	DWORD *pSrc = (DWORD*)m_stpSprite[iSpriteIndex].byplmage;
	DWORD *pDest = (DWORD*)bypDest + (iDrawX + (iDrawY * iDestWidth)) -
		(m_stpSprite[iSpriteIndex].iCenterPointX + (m_stpSprite[iSpriteIndex].iCenterPointY * iDestWidth));
	BYTE *pDestOrigin = (BYTE*)pDest;
	BYTE *pSrcOrigin = (BYTE*)pSrc;

	//////////////////////////////////////////////////////////////////////////////
	// Ŭ���� ����
	//////////////////////////////////////////////////////////////////////////////

	int iDrawPosX = iDrawX - m_stpSprite[iSpriteIndex].iCenterPointX;
	int iDrawPosY = iDrawY - m_stpSprite[iSpriteIndex].iCenterPointY;
	int iSpriteWidth = m_stpSprite[iSpriteIndex].iWidth;
	int iSpriteHeight = m_stpSprite[iSpriteIndex].iHeight;

	if (iSpriteIndex >= m_iMaxSprite || m_stpSprite->byplmage == NULL)	return;

	//////////////////////////////////////////////////////////////////////////////
	// Ŭ���� ó��
	//////////////////////////////////////////////////////////////////////////////

	if (iDrawPosX + m_stpSprite[iSpriteIndex].iWidth > iDestWidth)		//������
	{
		iSpriteWidth = iDestWidth - iDrawPosX;
	}

	if (iDrawPosY + m_stpSprite[iSpriteIndex].iHeight > iDestHeight)	//�Ʒ�
	{
		iSpriteHeight = iDestHeight - iDrawPosY;
	}

	if (iDrawPosY < 0)													//��
	{
		iDrawPosY = abs(iDrawPosY);
		iSpriteHeight -= iDrawPosY;
		pSrcOrigin += (iDrawPosY * m_stpSprite[iSpriteIndex].iWidth) * 4;
		pDestOrigin += (iDrawPosY * iDestWidth) * 4;
	}

	if (iDrawPosX < 0)													//����
	{
		iDrawPosX = abs(iDrawPosX);
		iSpriteWidth -= iDrawPosX;
		pSrcOrigin += iDrawPosX * 4;
		pDestOrigin += iDrawPosX * 4;
	}

	//////////////////////////////////////////////////////////////////////////////
	// ���̿� ���� ��� ó��
	//////////////////////////////////////////////////////////////////////////////
	iSpriteWidth = (int)(iSpriteWidth * iDrawLen / 100);

	//////////////////////////////////////////////////////////////////////////////
	// �׸� �׸���
	//////////////////////////////////////////////////////////////////////////////
	for (int iCntHeight = 0; iCntHeight < iSpriteHeight; iCntHeight++)
	{
		pDest = (DWORD*)pDestOrigin;
		pSrc = (DWORD*)pSrcOrigin;
		for (int iCntWidth = 0; iCntWidth < iSpriteWidth; iCntWidth++)
		{
			*pDest = *pSrc;

			pDest++;
			pSrc++;
		}

		pDestOrigin += iDestPitch;
		pSrcOrigin += m_stpSprite[iSpriteIndex].iPitch;
	}
}
