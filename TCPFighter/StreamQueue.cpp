#include "stdafx.h"
#include "StreamQueue.h"

/////////////////////////////////////////////////////////////////////////
// ������(���� ũ�� ����), �ı���.
//
//
/////////////////////////////////////////////////////////////////////////
CAyaStreamSQ::CAyaStreamSQ()
{
	Initial(1000);
}

CAyaStreamSQ::CAyaStreamSQ(int iBufferSize)
{
	Initial(iBufferSize);
}

CAyaStreamSQ::~CAyaStreamSQ(void)
{
	delete []m_chpBuffer;
}

/////////////////////////////////////////////////////////////////////////
// ���� ���� & ť�� �ʱ�ȭ.
//
// Parameters: (int)���ۿ뷮.
// Return: ����.
/////////////////////////////////////////////////////////////////////////
void CAyaStreamSQ::Initial(int iBufferSize)
{
	m_iBufferSize = iBufferSize;
	m_chpBuffer = new char[iBufferSize];

	m_iReadPos = 0;
	m_iWritePos = 0;
}

/////////////////////////////////////////////////////////////////////////
// ���� ��ü�� �뷮 ����.
//
// Parameters: ����.
// Return: (int)���ۿ뷮.
/////////////////////////////////////////////////////////////////////////
int	CAyaStreamSQ::GetBufferSize(void)
{
	return m_iBufferSize;
}

/////////////////////////////////////////////////////////////////////////
// ���� ������� �뷮 ���.
//
// Parameters: ����.
// Return: (int)������� �뷮.
/////////////////////////////////////////////////////////////////////////
int	CAyaStreamSQ::GetUseSize(void)
{
	if (m_iWritePos >= m_iReadPos)		
		return m_iWritePos - m_iReadPos;
	else								
		return GetBufferSize() - m_iReadPos + m_iWritePos;
}

/////////////////////////////////////////////////////////////////////////
// ���� ���ۿ� ���� �뷮 ���.
//
// Parameters: ����.
// Return: (int)�����뷮.
/////////////////////////////////////////////////////////////////////////
int	CAyaStreamSQ::GetFreeSize(void)
{
	if (m_iReadPos != m_iWritePos){
		if (m_iReadPos >= m_iWritePos)
			return m_iReadPos - m_iWritePos - 1;
		else
			return GetBufferSize() - m_iWritePos + m_iReadPos - 1;
	}

	else
		return GetBufferSize() - 1;
}

/////////////////////////////////////////////////////////////////////////
// ���� �����ͷ� �ܺο��� �ѹ濡 �а�, �� �� �ִ� ����.
// (������ ���� ����)
//
// Parameters: ����.
// Return: (int)��밡�� �뷮.
/////////////////////////////////////////////////////////////////////////
int	CAyaStreamSQ::GetNotBrokenGetSize(void)
{
	if (m_iWritePos < m_iReadPos)
		return GetBufferSize() - m_iReadPos;
	else
		return GetUseSize();
}

int	CAyaStreamSQ::GetNotBrokenPutSize(void)
{
	if (m_iWritePos < m_iReadPos)
		return GetBufferSize() - m_iWritePos;
	else
		return GetFreeSize();
}

/////////////////////////////////////////////////////////////////////////
// WritePos �� ����Ÿ ����.
//
// Parameters: (char *)����Ÿ ������. (int)ũ��. 
// Return: (int)���� ũ��.
/////////////////////////////////////////////////////////////////////////
int	CAyaStreamSQ::Put(char *chpData, int iSize)
{
	if (GetFreeSize() < iSize)
		iSize = GetFreeSize();
	if (GetNotBrokenPutSize() < iSize && m_iWritePos < m_iReadPos)
		iSize = GetNotBrokenPutSize();

	for (int iCnt = 0; iCnt < iSize; iCnt++)
	{
		m_chpBuffer[m_iWritePos++] = chpData[iCnt];
		if (m_iWritePos >= m_iBufferSize)	m_iWritePos = 0;
	}
	
	return iSize;
}

/////////////////////////////////////////////////////////////////////////
// ReadPos ���� ����Ÿ ������. ReadPos �̵�.
//
// Parameters: (char *)����Ÿ ������. (int)ũ��.
// Return: (int)������ ũ��.
/////////////////////////////////////////////////////////////////////////
int	CAyaStreamSQ::Get(char *chpDest, int iSize)
{
	if (GetUseSize() < iSize)
		iSize = GetUseSize();
	if (GetNotBrokenGetSize() < iSize && m_iWritePos < m_iReadPos)
		iSize = GetNotBrokenGetSize();

	for (int iCnt = 0; iCnt < iSize; iCnt++)
	{
		chpDest[iCnt] = m_chpBuffer[m_iReadPos++];
		if (m_iReadPos >= m_iBufferSize)	m_iReadPos = 0;
	}
	
	return iSize;
}

/////////////////////////////////////////////////////////////////////////
// ReadPos ���� ����Ÿ �о��. ReadPos ����.
//
// Parameters: (char *)����Ÿ ������. (int)ũ��.
// Return: (int)������ ũ��.
/////////////////////////////////////////////////////////////////////////
int	CAyaStreamSQ::Peek(char *chpDest, int iSize)
{
	if (GetUseSize() < iSize)
		iSize = GetUseSize();
	if (GetNotBrokenGetSize() < iSize && m_iWritePos < m_iReadPos)
		iSize = GetNotBrokenGetSize();

	for (int iCnt = 0; iCnt < iSize; iCnt++)
	{
		chpDest[iCnt] = m_chpBuffer[m_iReadPos + iCnt];
	}

	return iSize;
}





/////////////////////////////////////////////////////////////////////////
// ���ϴ� ���̸�ŭ �б���ġ ���� ����.
//
// Parameters: ����.
// Return: ����.
/////////////////////////////////////////////////////////////////////////
void CAyaStreamSQ::RemoveData(int iSize)
{
	if (GetUseSize() < iSize)
		return;
	else
	{
		if (m_iReadPos + iSize >= GetBufferSize())
			m_iReadPos = (m_iReadPos + iSize) % GetBufferSize();
		else
			m_iReadPos += iSize;
	}
}

/////////////////////////////////////////////////////////////////////////
// Write �� ��ġ�� �̵��ϴ� �Լ�.
//
// Parameters: ����.
// Return: (int)Write �̵� ������
/////////////////////////////////////////////////////////////////////////
int	CAyaStreamSQ::MoveWritePos(int iSize)
{
	if (GetFreeSize() < iSize)
		return 0;
	else
	{
		if (m_iWritePos + iSize >= GetBufferSize())
			m_iWritePos = (m_iWritePos + iSize) % GetBufferSize();
		else
			m_iWritePos += iSize;
	}

	return iSize;
}


/////////////////////////////////////////////////////////////////////////
// ������ ��� ����Ÿ ����.
//
// Parameters: ����.
// Return: ����.
/////////////////////////////////////////////////////////////////////////
void CAyaStreamSQ::ClearBuffer(void)
{
	for (int iCnt = 0; iCnt < GetBufferSize(); iCnt++)
	{
		m_chpBuffer[iCnt] = 0;
	}
}

/////////////////////////////////////////////////////////////////////////
// ������ ������ ����.
//
// Parameters: ����.
// Return: (char *) ���� ������.
/////////////////////////////////////////////////////////////////////////
char *CAyaStreamSQ::GetBufferPtr(void)
{
	return m_chpBuffer;
}

/////////////////////////////////////////////////////////////////////////
// ������ ReadPos ������ ����.
//
// Parameters: ����.
// Return: (char *) ���� ������.
/////////////////////////////////////////////////////////////////////////
char *CAyaStreamSQ::GetReadBufferPtr(void)
{
	return &m_chpBuffer[m_iReadPos];
}

/////////////////////////////////////////////////////////////////////////
// ������ WritePos ������ ����.
//
// Parameters: ����.
// Return: (char *) ���� ������.
/////////////////////////////////////////////////////////////////////////
char *CAyaStreamSQ::GetWriteBufferPtr(void)
{
	return &m_chpBuffer[m_iWritePos];
}