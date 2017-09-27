/*---------------------------------------------------------------

	N Packet.

	��Ʈ��ũ ��Ŷ�� Ŭ����.
	�����ϰ� ��Ŷ�� ������� ����Ÿ�� In, Out �Ѵ�.

	�÷����� ���� ������ ����� �ٸ��Ƿ�, ��Ʈ��ũ ��Ŷ ���۽ÿ� ������ �߻��� �� �ִ�.
	�׸��Ͽ� �÷����� �������� ���� ���������� ������ ����.

	--------------------------------------------------------

		NPacket::_ValueSizeCheck()  
		
		�Լ��� ���� 1ȸ ȣ���Ͽ� �� ������ ������ Ȯ���� �ʼ��� �ؾ��Ѵ�.
		FALSE ���Ͻ� �����ڿ��� �����ض�.


		�Ʒ� ������� �ٸ� ��� ������ �ÿ����� �߻��Ѵ�.

		C					C#				TYPE
		char				sbyte			signed 1Byte
		unsigned char		byte			unsigned 1Byte
		WCHAR				char			unicode char 2Byte

		short				short			signed 2Byte
		unsigned short		ushort			unsigned 2Byte

		int					int				signed 4Byte
		unsigned int		uint			unsigned 4Byte
		float				float			float 4Byte

		__int64				long			signed 8Byte
		unsigned __int64	ulong			unsigned 8Byte
		
	--------------------------------------------------------


	- ����.

	CNPacket cPacket;

	�ֱ�.
	clPacket << 40030;	or	clPacket << iValue;		(int �ֱ�)
	clPacket << 3;		or	clPacket << byValue;	(BYTE �ֱ�)
	clPacket << 1.4;	or	clPacket << fValue;		(float �ֱ�)

	����.
	clPacket >> iValue;		(int ����)
	clPacket >> byValue;	(BYTE ����)
	clPacket >> fValue;		(float ����)

	!.	���ԵǴ� ����Ÿ FIFO ������ �����ȴ�.
		ť�� �ƴϹǷ�, �ֱ�(<<).����(>>) �� ȥ���ؼ� ����ϸ� �ȵȴ�.
		
----------------------------------------------------------------*/
#ifndef  __N_PACKET__
#define  __N_PACKET__

class CNPacket
{
public:
	
	/*---------------------------------------------------------------
	AyaPacket Enum.

	----------------------------------------------------------------*/
	enum enN_PACKET
	{
		eBUFFER_DEFAULT			= 10000,	// ��Ŷ�� �⺻ ���� ������.
		eSTRING_MAX				= 1024,		// ���ڿ� �Է½� �ִ����

		eSBYTE					= 1,
		eBYTE					= 1,
		eWCHAR					= 2,

		eSHORT					= 2,
		eUSHORT					= 2,
		eINT					= 4,
		eUINT					= 4,
		eFLOAT					= 4,

		eINT64					= 8,		// C# ���� long ��.
		eUINT64					= 8
	};

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
			CNPacket();
			CNPacket(int iBufferSize);
			CNPacket(const CNPacket &clSrcPacket);

	virtual	~CNPacket();


	//////////////////////////////////////////////////////////////////////////
	// �� �ý����� ���� ���� üũ.
	//
	// Parameters: ����.
	// Return: (bool)true,false.
	//////////////////////////////////////////////////////////////////////////
	static bool	_ValueSizeCheck(void)
	{
		// 1 byte ���� 
		if ( eSBYTE != sizeof(char) || eBYTE != sizeof(unsigned char) ) return false;

		// 2 byte ���� 
		if ( eWCHAR != sizeof(WCHAR) || eSHORT != sizeof(short) || eUSHORT != sizeof(unsigned short)) return false;

		// 4 byte ���� 
		if ( eINT != sizeof(int) || eUINT != sizeof(unsigned int) || eFLOAT != sizeof(float)) return false;

		// 8 byte ���� 
		if ( eINT64 != sizeof(__int64) || eUINT64 != sizeof(unsigned __int64)) return false;
		return true;
	}


	//////////////////////////////////////////////////////////////////////////
	// ��Ŷ �ʱ�ȭ.
	//
	// �޸� �Ҵ��� ���⼭ �ϹǷ�, �Ժη� ȣ���ϸ� �ȵȴ�. 
	//
	// Parameters: (int)BufferSize.
	// Return: ����.
	//////////////////////////////////////////////////////////////////////////
	void	Initial(int iBufferSize = eBUFFER_DEFAULT);
	//////////////////////////////////////////////////////////////////////////
	// ��Ŷ  �ı�.
	//
	// Parameters: ����.
	// Return: ����.
	//////////////////////////////////////////////////////////////////////////
	void	Release(void);



	//////////////////////////////////////////////////////////////////////////
	// ��Ŷ û��.
	//
	// Parameters: ����.
	// Return: ����.
	//////////////////////////////////////////////////////////////////////////
	void	Clear(void);


	//////////////////////////////////////////////////////////////////////////
	// ���� ������ ���.
	//
	// Parameters: ����.
	// Return: (int)��Ŷ ���� ������ ���.
	//////////////////////////////////////////////////////////////////////////
	int		GetBufferSize(void) { return m_iBufferSize; }
	//////////////////////////////////////////////////////////////////////////
	// ���� ������� ������ ���.
	//
	// Parameters: ����.
	// Return: (int)������� ����Ÿ ������.
	//////////////////////////////////////////////////////////////////////////
	const int		GetDataSize(void) const { return m_iDataSize; }



	//////////////////////////////////////////////////////////////////////////
	// ���� ������ ���.
	//
	// Parameters: ����.
	// Return: (unsigned char *)���� ������.
	//////////////////////////////////////////////////////////////////////////
	unsigned char *GetBufferPtr(void) const { return m_chpBuffer; }

	//////////////////////////////////////////////////////////////////////////
	// ���� Pos �̵�. (�����̵��� �ȵ�)
	// GetBufferPtr �Լ��� �̿��Ͽ� �ܺο��� ������ ���� ������ ������ ��� ���. 
	//
	// Parameters: (int) �̵� ������.
	// Return: (int) �̵��� ������.
	//////////////////////////////////////////////////////////////////////////
	int		MoveWritePos(int iSize);
	int		MoveReadPos(int iSize);






	/* ============================================================================= */
	// ������ ���۷�����.
	/* ============================================================================= */
	CNPacket	&operator = (CNPacket &clSrcPacket);

	void		operator << (char chValue);
	void		operator << (unsigned char byValue);
	void		operator << (WCHAR wchValue);
	void		operator << (short shValue);
	void		operator << (int iValue);
	void		operator << (unsigned int iValue);
	void		operator << (float fValue);
	void		operator << (__int64 i64Value);
	void		operator << (unsigned __int64 i64Value);
	void		operator << (WCHAR *szString);

	void		operator >> (char &chValue);
	void		operator >> (unsigned char &byValue);
	void		operator >> (WCHAR &wchValue);
	void		operator >> (short &shValue);
	void		operator >> (int &iValue);
	void		operator >> (unsigned int &iValue);
	void		operator >> (float &fValue);
	void		operator >> (__int64 &i64Value);
	void		operator >> (unsigned __int64 &i64Value);
	void		operator >> (WCHAR *szString);
	//////////////////////////////////////////////////////////////////////////
	// �ֱ�.	�� ���� Ÿ�Ը��� ��� ����.
	//////////////////////////////////////////////////////////////////////////
	int Put(char chValue);				// C# - sbyte	// signed 1Byte
	int Put(unsigned char byValue);		// C# - byte	// unsigned 1Byte
	int Put(WCHAR wchValue);			// C# - char	// unicode char 2Byte

	int Put(short shValue);				// C# - short	// signed 2Byte
	int Put(unsigned short wValue);		// C# - ushort	// unsigned 2Byte

	int Put(int iValue);				// C# - int		// signed 4Byte
	int Put(unsigned int iValue);		// C# - uint	// unsigned 4Byte
	int Put(float fValue);				// C# - float	// float 4Byte

	int Put(__int64 i64Value);			// C# - long	// signed 8Byte
	int Put(unsigned __int64 i64Value);	// C# - ulong	// unsigned 8Byte

	//int	Put(Proud::String &String);		// ���ڿ� �ֱ�
	int	Put(WCHAR *szString);

	int Put(char *Lump, int iSize);
	//////////////////////////////////////////////////////////////////////////
	// ����.	�� ���� Ÿ�Ը��� ��� ����.
	//////////////////////////////////////////////////////////////////////////

	int Get(unsigned char &byValue) const;
	int Get(char &chValue) const;
	int Get(WCHAR &wchValue) const;

	int Get(short &shValue) const;
	int Get(unsigned short &wValue) const;

	int Get(int &iValue) const;
	int Get(unsigned int &iValue) const;
	int Get(float &fValue) const;

	int Get(__int64 &i64Value) const;			// C# - long
	int Get(unsigned __int64 &i64Value) const;	// C# - ulong

	//int	Get(Proud::String &String) const;		// ���ڿ� ����
	int	Get(WCHAR *szString, int iBuffer) const;

	int Get(char *Lump, int iSize);
	//////////////////////////////////////////////////////////////////////////
	// ����Ÿ ���.
	//
	// Parameters: (unsigned char *)Dest ������. (int)Size.
	// Return: (int)������ ������.
	//////////////////////////////////////////////////////////////////////////
	int		GetData(unsigned char *bypDest, int iSize) const;

	//////////////////////////////////////////////////////////////////////////
	// ����Ÿ ����.
	//
	// Parameters: (unsigned char *)Src ������. (int)SrcSize.
	// Return: (int)������ ������.
	//////////////////////////////////////////////////////////////////////////
	int		PutData(unsigned char *bypSrc, int iSrcSize);




protected:

	//------------------------------------------------------------
	// ��Ŷ���� / ���� ������.
	//------------------------------------------------------------
	unsigned char	*m_chpBufferExpansion;

	unsigned char	*m_chpBuffer;
	int				m_iBufferSize;
	//------------------------------------------------------------
	// ��Ŷ���� ���� ��ġ.	(�� Ŭ���� ������ ������� ������, Ȯ�强�� ���� ���)
	//------------------------------------------------------------
	unsigned char	*m_chpDataFieldStart;
	unsigned char	*m_chpDataFieldEnd;


	//------------------------------------------------------------
	// ������ ���� ��ġ, ���� ��ġ.
	//------------------------------------------------------------
	mutable unsigned char	*m_chpReadPos;
	mutable unsigned char	*m_chpWritePos;


	//------------------------------------------------------------
	// ���� ���ۿ� ������� ������.
	//------------------------------------------------------------
	mutable int		m_iDataSize;


};


#endif
