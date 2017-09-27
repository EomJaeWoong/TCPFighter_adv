#ifndef __NETWORK__H__
#define __NETWORK__H__

#define WM_NETWORK (WM_USER+1)

BOOL InitialNetwork(SOCKET *sock, HWND *hWnd);

/*----------------------------------------------------------------------------*/
// Receive Packet
/*----------------------------------------------------------------------------*/
BOOL recvProc_CreateMyCharacter(CNPacket *pPacket);
BOOL recvProc_CreateOtherCharacter(CNPacket *pPacket);
BOOL recvProc_DeleteCharacter(CNPacket *pPacket);
BOOL recvProc_MoveStart(CNPacket *pPacket);
BOOL recvProc_MoveStop(CNPacket *pPacket);
BOOL recvProc_Attack1(CNPacket *pPacket);
BOOL recvProc_Attack2(CNPacket *pPacket);
BOOL recvProc_Attack3(CNPacket *pPacket);
BOOL recvProc_Damage(CNPacket *pPacket);
BOOL recvProc_Sync(CNPacket *pPacket);
/*----------------------------------------------------------------------------*/
// Send Packet
/*----------------------------------------------------------------------------*/
BOOL sendProc_MoveStart(int iDir, int iX, int iY);
BOOL sendProc_MoveStop(int iDir, int iX, int iY);
BOOL sendProc_Attack1(int iDir, int iX, int iY);
BOOL sendProc_Attack2(int iDir, int iX, int iY);
BOOL sendProc_Attack3(int iDir, int iX, int iY);

/*----------------------------------------------------------------------------*/
// Make Packet
/*----------------------------------------------------------------------------*/
int makePacket_MoveStart(st_NETWORK_PACKET_HEADER *pHeader, CNPacket *pPacket,
	int iDir, int iX, int iY);
int makePacket_MoveStop(st_NETWORK_PACKET_HEADER *pHeader, CNPacket *pPacket, 
	int iDir, int iX, int iY);
int makePacket_Attack1(st_NETWORK_PACKET_HEADER *pHeader, CNPacket *pPacket, 
	int iDir, int iX, int iY);
int makePacket_Attack2(st_NETWORK_PACKET_HEADER *pHeader, CNPacket *pPacket, 
	int iDir, int iX, int iY);
int makePacket_Attack3(st_NETWORK_PACKET_HEADER *pHeader, CNPacket *pPacket, 
	int iDir, int iX, int iY);

#endif