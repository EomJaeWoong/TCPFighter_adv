#include "stdafx.h"

BOOL InitialNetwork(SOCKET *sock, HWND *hWnd)
{
	int retval;
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return FALSE;

	*sock = socket(AF_INET, SOCK_STREAM, 0);
	if (*sock == INVALID_SOCKET)
		return FALSE;

	retval = WSAAsyncSelect(*sock, *hWnd, WM_NETWORK, FD_CONNECT | FD_CLOSE | FD_READ | FD_WRITE);
	if (retval == SOCKET_ERROR)
		return FALSE;

	SOCKADDR_IN sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(5000);
	InetPton(AF_INET, L"127.0.0.1", &sockaddr.sin_addr.s_addr);

	retval = connect(*sock, (SOCKADDR *)&sockaddr, sizeof(sockaddr));
	if (retval == SOCKET_ERROR)
	{
		retval = WSAGetLastError();
		if (retval != WSAEWOULDBLOCK)
			return FALSE;
	}

	return TRUE;
}

/*----------------------------------------------------------------------------*/
// Receive Packet

//------------------------------------------------------------------------------
// 클라이언트 자신의 캐릭터 할당
//------------------------------------------------------------------------------
BOOL recvProc_CreateMyCharacter(CNPacket *pPacket)
{
	unsigned int	ID;
	BYTE			Direction;
	short		X;
	short		Y;
	BYTE			HP;

	*pPacket >> ID;
	*pPacket >> Direction;
	*pPacket >> X;
	*pPacket >> Y;
	*pPacket >> HP;

	g_pPlayerObject = new CPlayerObject(TRUE, ID, eTYPE_PLAYER, HP, Direction);
	g_pPlayerObject->SetOldPosition(X, Y);
	g_pPlayerObject->SetPosition(X, Y);

	g_Object.push_back(g_pPlayerObject);

	return TRUE;
}

//------------------------------------------------------------------------------
// 다른 클라이언트의 캐릭터 생성
//------------------------------------------------------------------------------
BOOL recvProc_CreateOtherCharacter(CNPacket *pPacket)
{
	CPlayerObject *pPlayerObject;

	unsigned int	ID;
	BYTE			Direction;
	short		X;
	short		Y;
	BYTE			HP;

	*pPacket >> ID;
	*pPacket >> Direction;
	*pPacket >> X;
	*pPacket >> Y;
	*pPacket >> HP;

	pPlayerObject = new CPlayerObject(FALSE, ID, eTYPE_PLAYER, HP, Direction);
	pPlayerObject->SetPosition(X, Y);

	g_Object.push_back(pPlayerObject);

	return TRUE;
}

//------------------------------------------------------------------------------
// 캐릭터 삭제
//------------------------------------------------------------------------------
BOOL recvProc_DeleteCharacter(CNPacket *pPacket)
{
	Object::iterator oIter;
	unsigned int	ID;

	*pPacket >> ID;

	for (oIter = g_Object.begin(); oIter != g_Object.end(); ++oIter)
	{
		if (ID == (*oIter)->GetObjectID()){
			delete (*oIter);
			g_Object.erase(oIter);

			return TRUE;
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------
// 캐릭터 이동시작
//------------------------------------------------------------------------------
BOOL recvProc_MoveStart(CNPacket *pPacket)
{
	Object::iterator oIter;
	unsigned int	ID;
	BYTE			Direction;
	short			X;
	short			Y;

	*pPacket >> ID;
	*pPacket >> Direction;
	*pPacket >> X;
	*pPacket >> Y;

	for (oIter = g_Object.begin(); oIter != g_Object.end(); ++oIter)
	{
		if (ID == (*oIter)->GetObjectID()){
			(*oIter)->ActionInput(Direction);
			(*oIter)->SetPosition(X, Y);

			return TRUE;
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------
// 캐릭터 이동중지
//------------------------------------------------------------------------------
BOOL recvProc_MoveStop(CNPacket *pPacket)
{
	Object::iterator oIter;
	unsigned int	ID;
	BYTE			Direction;
	short		X;
	short		Y;

	*pPacket >> ID;
	*pPacket >> Direction;
	*pPacket >> X;
	*pPacket >> Y;

	for (oIter = g_Object.begin(); oIter != g_Object.end(); ++oIter)
	{
		if (ID == (*oIter)->GetObjectID()){
			(*oIter)->ActionInput(dfACTION_STAND);
			((CPlayerObject *)(*oIter))->SetDirection(Direction);
			(*oIter)->SetPosition(X, Y);

			return TRUE;
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------
// 캐릭터 공격1
//------------------------------------------------------------------------------
BOOL recvProc_Attack1(CNPacket *pPacket)
{
	Object::iterator oIter;
	unsigned int	ID;
	BYTE			Direction;
	short			X;
	short			Y;

	*pPacket >> ID;
	*pPacket >> Direction;
	*pPacket >> X;
	*pPacket >> Y;

	for (oIter = g_Object.begin(); oIter != g_Object.end(); ++oIter)
	{
		if (ID == (*oIter)->GetObjectID()){
			(*oIter)->ActionInput(dfACTION_ATTACK1);
			(*oIter)->SetPosition(X, Y);

			return TRUE;
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------
// 캐릭터 공격2
//------------------------------------------------------------------------------
BOOL recvProc_Attack2(CNPacket *pPacket)
{
	Object::iterator oIter;
	unsigned int	ID;
	BYTE			Direction;
	short			X;
	short			Y;

	*pPacket >> ID;
	*pPacket >> Direction;
	*pPacket >> X;
	*pPacket >> Y;

	for (oIter = g_Object.begin(); oIter != g_Object.end(); ++oIter)
	{
		if (ID == (*oIter)->GetObjectID()){
			(*oIter)->ActionInput(dfACTION_ATTACK2);
			(*oIter)->SetPosition(X, Y);

			return TRUE;
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------
// 캐릭터 공격3
//------------------------------------------------------------------------------
BOOL recvProc_Attack3(CNPacket *pPacket)
{
	Object::iterator oIter;
	unsigned int	ID;
	BYTE			Direction;
	short			X;
	short			Y;

	*pPacket >> ID;
	*pPacket >> Direction;
	*pPacket >> X;
	*pPacket >> Y;

	for (oIter = g_Object.begin(); oIter != g_Object.end(); ++oIter)
	{
		if (ID == (*oIter)->GetObjectID()){
			(*oIter)->ActionInput(dfACTION_ATTACK3);
			(*oIter)->SetPosition(X, Y);

			return TRUE;
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------
// 캐릭터 데미지
//------------------------------------------------------------------------------
BOOL recvProc_Damage(CNPacket *pPacket)
{
	Object::iterator damageIter;
	unsigned int	AttackID;
	unsigned int	DamageID;
	BYTE			DamageHP;

	*pPacket >> AttackID;
	*pPacket >> DamageID;
	*pPacket >> DamageHP;

	for (damageIter = g_Object.begin(); damageIter != g_Object.end(); ++damageIter)
	{
		if (eTYPE_PLAYER == (*damageIter)->GetObjectType())
		{
			CPlayerObject *pDamagePlayer = (CPlayerObject *)(*damageIter);
		
			if (DamageID == (*damageIter)->GetObjectID())
			{
				CBaseObject *pEffectObject = new CEffectObject(0, eTYPE_EFFECT,
					pDamagePlayer->GetCurX(), pDamagePlayer->GetCurY() - 70,
					4, eEFFECT_SPARK_01, eEFFECT_SPARK_04);
				g_Object.push_back(pEffectObject);

				pDamagePlayer->SetHP(DamageHP);
				return TRUE;
			}
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------
// 동기화(캐릭터 좌표 보정)
//------------------------------------------------------------------------------
BOOL recvProc_Sync(CNPacket *pPacket)
{
	Object::iterator oIter;
	unsigned int		ID;
	short			X;
	short			Y;

	*pPacket >> ID;
	*pPacket >> X;
	*pPacket >> Y;

	for (oIter = g_Object.begin(); oIter != g_Object.end(); ++oIter)
	{
		if (ID == (*oIter)->GetObjectID()){
			(*oIter)->SetPosition(X, Y);
			return TRUE;
		}
	}

	return FALSE;
}
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
// Send Packet

//------------------------------------------------------------------------------
// 캐릭터 이동 시작
//------------------------------------------------------------------------------
BOOL sendProc_MoveStart(int iDir, int iX, int iY)
{
	st_NETWORK_PACKET_HEADER header;
	CNPacket cPacket;

	int len = makePacket_MoveStart(&header, &cPacket, iDir, iX, iY);

	SendQ.Put((char *)&header, sizeof(header));
	SendQ.Put((char *)cPacket.GetBufferPtr(), len);

	return TRUE;
}

//------------------------------------------------------------------------------
// 캐릭터 이동 중지
//------------------------------------------------------------------------------
BOOL sendProc_MoveStop(int iDir, int iX, int iY)
{
	st_NETWORK_PACKET_HEADER header;
	CNPacket cPacket;

	int len = makePacket_MoveStop(&header, &cPacket, iDir, iX, iY);

	SendQ.Put((char *)&header, sizeof(header));
	SendQ.Put((char *)cPacket.GetBufferPtr(), len);

	return TRUE;
}

//------------------------------------------------------------------------------
// 캐릭터 공격1
//------------------------------------------------------------------------------
BOOL sendProc_Attack1(int iDir, int iX, int iY)
{
	st_NETWORK_PACKET_HEADER header;
	CNPacket cPacket;

	int len = makePacket_Attack1(&header, &cPacket, iDir, iX, iY);

	SendQ.Put((char *)&header, sizeof(header));
	SendQ.Put((char *)cPacket.GetBufferPtr(), len);

	return TRUE;
}

//------------------------------------------------------------------------------
// 캐릭터 공격2
//------------------------------------------------------------------------------
BOOL sendProc_Attack2(int iDir, int iX, int iY)
{
	st_NETWORK_PACKET_HEADER header;
	CNPacket cPacket;

	int len = makePacket_Attack2(&header, &cPacket, iDir, iX, iY);

	SendQ.Put((char *)&header, sizeof(header));
	SendQ.Put((char *)cPacket.GetBufferPtr(), len);

	return TRUE;
}

//------------------------------------------------------------------------------
// 캐릭터 공격3
//------------------------------------------------------------------------------
BOOL sendProc_Attack3(int iDir, int iX, int iY)
{
	st_NETWORK_PACKET_HEADER header;
	CNPacket cPacket;

	int len = makePacket_Attack3(&header, &cPacket, iDir, iX, iY);

	SendQ.Put((char *)&header, sizeof(header));
	SendQ.Put((char *)cPacket.GetBufferPtr(), len);

	return TRUE;
}
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
// Make Packet

//------------------------------------------------------------------------------
// 캐릭터 이동 시작 패킷
//------------------------------------------------------------------------------
int makePacket_MoveStart(st_NETWORK_PACKET_HEADER *pHeader, CNPacket *pPacket,
	int iDir, int iX, int iY)
{
	*pPacket << (BYTE)iDir;
	*pPacket << (short)iX;
	*pPacket << (short)iY;
	*pPacket << (BYTE)dfNETWORK_PACKET_END;

	pHeader->byCode = dfNETWORK_PACKET_CODE;
	pHeader->bySize = pPacket->GetDataSize() - 1;
	pHeader->byType = dfPACKET_CS_MOVE_START;

	return pPacket->GetDataSize();
}

//------------------------------------------------------------------------------
// 캐릭터 이동 중지 패킷
//------------------------------------------------------------------------------
int makePacket_MoveStop(st_NETWORK_PACKET_HEADER *pHeader, CNPacket *pPacket,
	int iDir, int iX, int iY)
{
	*pPacket << (BYTE)iDir;
	*pPacket << (short)iX;
	*pPacket << (short)iY;
	*pPacket << (BYTE)dfNETWORK_PACKET_END;

	pHeader->byCode = dfNETWORK_PACKET_CODE;
	pHeader->bySize = pPacket->GetDataSize() - 1;
	pHeader->byType = dfPACKET_CS_MOVE_STOP;

	return pPacket->GetDataSize();
}

//------------------------------------------------------------------------------
// 캐릭터 공격1 패킷
//------------------------------------------------------------------------------
int makePacket_Attack1(st_NETWORK_PACKET_HEADER *pHeader, CNPacket *pPacket,
	int iDir, int iX, int iY)
{
	*pPacket << (BYTE)iDir;
	*pPacket << (short)iX;
	*pPacket << (short)iY;
	*pPacket << (BYTE)dfNETWORK_PACKET_END;

	pHeader->byCode = dfNETWORK_PACKET_CODE;
	pHeader->bySize = pPacket->GetDataSize() - 1;
	pHeader->byType = dfPACKET_CS_ATTACK1;

	return pPacket->GetDataSize();
}

//------------------------------------------------------------------------------
// 캐릭터 공격2 패킷
//------------------------------------------------------------------------------
int makePacket_Attack2(st_NETWORK_PACKET_HEADER *pHeader, CNPacket *pPacket,
	int iDir, int iX, int iY)
{
	*pPacket << (BYTE)iDir;
	*pPacket << (short)iX;
	*pPacket << (short)iY;
	*pPacket << (BYTE)dfNETWORK_PACKET_END;

	pHeader->byCode = dfNETWORK_PACKET_CODE;
	pHeader->bySize = pPacket->GetDataSize() - 1;
	pHeader->byType = dfPACKET_CS_ATTACK2;

	return pPacket->GetDataSize();
}

//------------------------------------------------------------------------------
// 캐릭터 공격3 패킷
//------------------------------------------------------------------------------
int makePacket_Attack3(st_NETWORK_PACKET_HEADER *pHeader, CNPacket *pPacket,
	int iDir, int iX, int iY)
{
	*pPacket << (BYTE)iDir;
	*pPacket << (short)iX;
	*pPacket << (short)iY;
	*pPacket << (BYTE)dfNETWORK_PACKET_END;

	pHeader->byCode = dfNETWORK_PACKET_CODE;
	pHeader->bySize = pPacket->GetDataSize() - 1;
	pHeader->byType = dfPACKET_CS_ATTACK3;

	return pPacket->GetDataSize();
}
/*----------------------------------------------------------------------------*/