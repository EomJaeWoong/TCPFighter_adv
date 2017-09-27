#include <windows.h>
#include "stdafx.h"
#include "NPacket.h"

//////////////////////////////////////////////////////////////////////////
// ������, �ı���.
//
// Parameters:
//				---------------------------------------------------------
//				(int) ���� ������.
//				---------------------------------------------------------
//				(const CNPacket &) ��Ŷ�� ���縦 ���� ��Ŷ ���۷���.
// Return:
//////////////////////////////////////////////////////////////////////////
CNPacket::CNPacket()
{
	Initial();
}

CNPacket::CNPacket(int iBufferSize)
{
	Initial(iBufferSize);
}

CNPacket::CNPacket(const CNPacket &clSrcPacket)
{
	
}

CNPacket::~CNPacket()
{
	Release();
}

//////////////////////////////////////////////////////////////////////////
// ��Ŷ �ʱ�ȭ.
//
// �޸� �Ҵ��� ���⼭ �ϹǷ�, �Ժη� ȣ���ϸ� �ȵȴ�. 
//
// Parameters: (int)BufferSize.
// Return: ����.
//////////////////////////////////////////////////////////////////////////
void	CNPacket::Initial(int iBufferSize)
{
	m_chpBuffer = new BYTE[iBufferSize];
	m_iBufferSize = iBufferSize;

	m_chpReadPos = m_chpBuffer;
	m_chpWritePos = m_chpBuffer;

	m_iDataSize = 0;
}

//////////////////////////////////////////////////////////////////////////
// ��Ŷ  �ı�.
//
// Parameters: ����.
// Return: ����.
//////////////////////////////////////////////////////////////////////////
void	CNPacket::Release(void)
{
	delete[] m_chpBuffer;
}



//////////////////////////////////////////////////////////////////////////
// ��Ŷ û��.
//
// Parameters: ����.
// Return: ����.
//////////////////////////////////////////////////////////////////////////
void	CNPacket::Clear(void)
{
	memset(m_chpBuffer, NULL, m_iBufferSize);
	m_chpWritePos = GetBufferPtr();
	m_chpReadPos = GetBufferPtr();
}

//////////////////////////////////////////////////////////////////////////
// ���� Pos �̵�. (�����̵��� �ȵ�)
// GetBufferPtr �Լ��� �̿��Ͽ� �ܺο��� ������ ���� ������ ������ ��� ���. 
//
// Parameters: (int) �̵� ������.
// Return: (int) �̵��� ������.
//////////////////////////////////////////////////////////////////////////
int		CNPacket::MoveWritePos(int iSize)
{
	m_chpWritePos += iSize;

	return iSize;
}

int		CNPacket::MoveReadPos(int iSize)
{
	m_chpReadPos += iSize;

	return iSize;
}






/* ============================================================================= */
// ������ ���۷�����.
/* ============================================================================= */
/*
CNPacket	&operator = (CNPacket &clSrcPacket)
{

}
*/
void CNPacket::operator << (char chValue){	Put(chValue);	}
void CNPacket::operator << (unsigned char byValue){	Put(byValue);}
void CNPacket::operator << (WCHAR wchValue){	Put(wchValue);}
void CNPacket::operator << (short shValue){	Put(shValue);}
void CNPacket::operator << (int iValue){	Put(iValue);}
void CNPacket::operator << (unsigned int iValue){	Put(iValue);}
void CNPacket::operator << (float fValue){	Put(fValue);}
void CNPacket::operator << (__int64 i64Value){	Put(i64Value);}
void CNPacket::operator << (unsigned __int64 i64Value){	Put(i64Value);}
void CNPacket::operator << (WCHAR *szString){	Put(szString);}

void CNPacket::operator >> (char &chValue){ Get(chValue); }
void CNPacket::operator >> (unsigned char &byValue){ Get(byValue); }
void CNPacket::operator >> (WCHAR &wchValue){ Get(wchValue); }
void CNPacket::operator >> (short &shValue){ Get(shValue); }
void CNPacket::operator >> (int &iValue){ Get(iValue); }
void CNPacket::operator >> (unsigned int &iValue){ Get(iValue); }
void CNPacket::operator >> (float &fValue){ Get(fValue); }
void CNPacket::operator >> (__int64 &i64Value){ Get(i64Value); }
void CNPacket::operator >> (unsigned __int64 &i64Value){ Get(i64Value); }
void CNPacket::operator >> (WCHAR *szString){ Get(szString, wcslen(szString)); }

//////////////////////////////////////////////////////////////////////////
// �ֱ�.	�� ���� Ÿ�Ը��� ��� ����.
//////////////////////////////////////////////////////////////////////////
int CNPacket::Put(char chValue)				// C# - sbyte	// signed 1Byte
{
	return PutData((BYTE *)&chValue, eSBYTE);
}

int CNPacket::Put(unsigned char byValue)		// C# - byte	// unsigned 1Byte
{
	return PutData((BYTE *)&byValue, eBYTE);
}

int CNPacket::Put(WCHAR wchValue)			// C# - char	// unicode char 2Byte
{
	return PutData((BYTE *)&wchValue, eWCHAR);
}

int CNPacket::Put(short shValue)				// C# - short	// signed 2Byte
{
	return PutData((BYTE *)&shValue, eSHORT);
}

int CNPacket::Put(unsigned short wValue)		// C# - ushort	// unsigned 2Byte
{
	return PutData((BYTE *)&wValue, eUSHORT);
}

int CNPacket::Put(int iValue)				// C# - int		// signed 4Byte
{
	return PutData((BYTE *)&iValue, eINT);
}

int CNPacket::Put(unsigned int iValue)		// C# - uint	// unsigned 4Byte
{
	return PutData((BYTE *)&iValue, eUINT);
}

int CNPacket::Put(float fValue)				// C# - float	// float 4Byte
{
	return PutData((BYTE *)&fValue, eFLOAT);
}

int CNPacket::Put(__int64 i64Value)			// C# - long	// signed 8Byte
{
	return PutData((BYTE *)&i64Value, eINT64);
}

int CNPacket::Put(unsigned __int64 i64Value)	// C# - ulong	// unsigned 8Byte
{
	return PutData((BYTE *)&i64Value, eUINT64);
}

/*
int	CNPacket::Put(Proud::String &String)		// ���ڿ� �ֱ�
{

}
*/

int	CNPacket::Put(WCHAR *szString)
{
	return PutData((BYTE*)szString, (wcslen(szString) + 1) * 2);
}

int CNPacket::Put(char *Lump, int iSize)
{
	return PutData((BYTE*)Lump, iSize);
}
//////////////////////////////////////////////////////////////////////////
// ����.	�� ���� Ÿ�Ը��� ��� ����.
//////////////////////////////////////////////////////////////////////////

int CNPacket::Get(unsigned char &byValue) const
{
	return GetData((BYTE *)&byValue, eSBYTE);
}

int CNPacket::Get(char &chValue) const
{
	return GetData((BYTE *)&chValue, eBYTE);
}

int CNPacket::Get(WCHAR &wchValue) const
{
	return GetData((BYTE *)&wchValue, eWCHAR);
}

int CNPacket::Get(short &shValue) const
{
	return GetData((BYTE *)&shValue, eSHORT);
}

int CNPacket::Get(unsigned short &wValue) const
{
	return GetData((BYTE *)&wValue, eUSHORT);
}

int CNPacket::Get(int &iValue) const
{
	return GetData((BYTE *)&iValue, eINT);
}

int CNPacket::Get(unsigned int &iValue) const
{
	return GetData((BYTE *)&iValue, eUINT);
}

int CNPacket::Get(float &fValue) const
{
	return GetData((BYTE *)&fValue, eFLOAT);
}

int CNPacket::Get(__int64 &i64Value) const			// C# - long
{
	return GetData((BYTE *)&i64Value, eINT64);
}

int CNPacket::Get(unsigned __int64 &i64Value) const	// C# - ulong
{
	return GetData((BYTE *)&i64Value, eUINT64);
}

/*
int	CNPacket::Get(Proud::String &String) const		// ���ڿ� ����
{

}
*/

int	CNPacket::Get(WCHAR *szString, int iBuffer) const
{
	return GetData((BYTE *)szString, iBuffer);
}

int CNPacket::Get(char *Lump, int iSize)
{
	return GetData((BYTE*)Lump, iSize);
}
//////////////////////////////////////////////////////////////////////////
// ����Ÿ ���.
//
// Parameters: (unsigned char *)Dest ������. (int)Size.
// Return: (int)������ ������.
//////////////////////////////////////////////////////////////////////////
int		CNPacket::GetData(unsigned char *bypDest, int iSize) const
{
	int iCnt;

	for (iCnt = 0; iCnt < iSize; iCnt++)
	{
		bypDest[iCnt] = *m_chpReadPos;
		m_chpReadPos++;
		m_iDataSize--;
	}

	return iCnt;
}

//////////////////////////////////////////////////////////////////////////
// ����Ÿ ����.
//
// Parameters: (unsigned char *)Src ������. (int)SrcSize.
// Return: (int)������ ������.
//////////////////////////////////////////////////////////////////////////
int		CNPacket::PutData(unsigned char *bypSrc, int iSrcSize)
{
	int iCnt;

	for (iCnt = 0; iCnt < iSrcSize; iCnt++)
	{
		*m_chpWritePos = bypSrc[iCnt];
		m_chpWritePos++;
		m_iDataSize++;
	}

	return iCnt;
}