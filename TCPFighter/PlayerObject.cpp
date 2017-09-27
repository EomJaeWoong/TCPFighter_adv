#include "stdafx.h"
#include "BaseObject.h"
#include "PlayerObject.h"

/*---------------------------------------------------------------------------------------------------------*/
// ������
// �����Ǵ� ĳ���Ϳ� ���� ����
/*---------------------------------------------------------------------------------------------------------*/
CPlayerObject::CPlayerObject(BOOL PlayerCharacter, int iObjectID, int iObjectType, char chHP, int iDirection)
	: CBaseObject(iObjectID, iObjectType), m_bPlayerCharacter(PlayerCharacter), m_chHP(chHP),
	m_dwActionCur(dfACTION_STAND),
	m_dwActionOld(dfACTION_STAND),  
	m_iDirCur(iDirection), 
	m_iDirOld(iDirection)
{
	ActionInput(dfACTION_STAND);
	if (iDirection == dfACTION_MOVE_RR)
		SetSprite(ePLAYER_STAND_R01, ePLAYER_STAND_R_MAX, 5);
	else if (iDirection == dfACTION_MOVE_LL)
		SetSprite(ePLAYER_STAND_L01, ePLAYER_STAND_L_MAX, 5);
}

CPlayerObject::~CPlayerObject()
{

}

/*---------------------------------------------------------------------------------------------------------*/
// Action
// ���� �׼�
/*---------------------------------------------------------------------------------------------------------*/
DWORD CPlayerObject::Action(DWORD dwParam)
{
	NextFrame();
	ActionProc();
	return FALSE;
}

/*---------------------------------------------------------------------------------------------------------*/
// �׼� ó����
// �Էµ� �׼��� ó���Ѵ�
/*---------------------------------------------------------------------------------------------------------*/
void CPlayerObject::ActionProc()
{
	// ������ �ȳ����� ��
	if ((m_dwActionCur == dfACTION_ATTACK1 || m_dwActionCur == dfACTION_ATTACK2 || m_dwActionCur == dfACTION_ATTACK3)
		&& (!isEndFrame()))
	{
		m_dwActionInput = m_dwActionCur;
		return;
	}

	// ������ ������ ��
	else if ((m_dwActionCur == dfACTION_ATTACK1 || m_dwActionCur == dfACTION_ATTACK2 || m_dwActionCur == dfACTION_ATTACK3)
		&& isEndFrame())
	{
		SetActionStand();
		m_dwActionInput = dfACTION_STAND;
	}

	switch (m_dwActionInput)
	{
	case dfACTION_ATTACK1 :
		SetActionAttack1();
		break;

	case dfACTION_ATTACK2 :
		SetActionAttack2();
		break;

	case dfACTION_ATTACK3 :
		SetActionAttack3();
		break;

	default :
		InputActionProc();
		break;
	}
}

/*---------------------------------------------------------------------------------------------------------*/
// Draw
// �ش� ��ü�� Screen�� �׸���
/*---------------------------------------------------------------------------------------------------------*/
void CPlayerObject::Draw(CSpriteDib *pSprite, BYTE* bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	int iDrawX = GetCurX() - g_cTileMap.GetDrawPosX();
	int	iDrawY = GetCurY() - g_cTileMap.GetDrawPosY();

	pSprite->DrawSprite50(eSHADOW, iDrawX, iDrawY, bypDest, iDestWidth, iDestHeight, iDestPitch);
	
	if (m_bPlayerCharacter)
		pSprite->DrawSpriteRed(GetSprite(), iDrawX, iDrawY, bypDest, iDestWidth, iDestHeight, iDestPitch);
	else
		pSprite->DrawSprite(GetSprite(), iDrawX, iDrawY, bypDest, iDestWidth, iDestHeight, iDestPitch);

	pSprite->DrawSprite(eGUAGE_HP, iDrawX - 35, iDrawY + 9, bypDest, iDestWidth, iDestHeight, iDestPitch, m_chHP);
}

/*---------------------------------------------------------------------------------------------------------*/
// �Է� �׼� ó��
// �׼� ���ð� ����, ��������Ʈ�� �����Ѵ�.
/*---------------------------------------------------------------------------------------------------------*/
void CPlayerObject::InputActionProc()
{
	switch (m_dwActionInput)
	{
	case dfACTION_STAND:														//������ ��
		SetActionStand();
		break;

	case dfACTION_MOVE_LL :														//����
		SetActionMove(dfACTION_MOVE_LL);

		if (GetCurX() - dfSPEED_PLAYER_X <= dfRANGE_MOVE_LEFT)
			SetPosition(GetCurX(), GetCurY());
		else
			SetPosition(GetCurX() - dfSPEED_PLAYER_X, GetCurY());
		
		break;

	case dfACTION_MOVE_RR :														//������
		SetActionMove(dfACTION_MOVE_RR);

		if (GetCurX() + dfSPEED_PLAYER_X >= dfRANGE_MOVE_RIGHT)
			SetPosition(GetCurX(), GetCurY());
		else
			SetPosition(GetCurX() + dfSPEED_PLAYER_X, GetCurY());
		
		break;

	case dfACTION_MOVE_DD:														//�Ʒ�
		SetActionMove(dfACTION_MOVE_DD);

		if (GetCurY() + dfSPEED_PLAYER_Y >= dfRANGE_MOVE_BOTTOM)
			SetPosition(GetCurX(), GetCurY());
		else
			SetPosition(GetCurX(), GetCurY() + dfSPEED_PLAYER_Y);
		
		break;

	case dfACTION_MOVE_UU :														//��
		SetActionMove(dfACTION_MOVE_UU);

		if (GetCurY() - dfSPEED_PLAYER_Y <= dfRANGE_MOVE_TOP)
			SetPosition(GetCurX(), GetCurY());
		else
			SetPosition(GetCurX(), GetCurY() - dfSPEED_PLAYER_Y);
		
		break;

	case dfACTION_MOVE_LD :														//���ʾƷ�
		SetActionMove(dfACTION_MOVE_LD);

		if ((GetCurX() - dfSPEED_PLAYER_X <= dfRANGE_MOVE_LEFT) ||
			(GetCurY() + dfSPEED_PLAYER_Y >= dfRANGE_MOVE_BOTTOM))
			SetPosition(GetCurX(), GetCurY());
		else
			SetPosition(GetCurX() - dfSPEED_PLAYER_X, GetCurY() + dfSPEED_PLAYER_Y);
		
		break;

	case dfACTION_MOVE_LU :														//������
		SetActionMove(dfACTION_MOVE_LU);

		if ((GetCurX() - dfSPEED_PLAYER_X <= dfRANGE_MOVE_LEFT) ||
			(GetCurY() - dfSPEED_PLAYER_Y <= dfRANGE_MOVE_TOP)) 
			SetPosition(GetCurX(), GetCurY());
		else
			SetPosition(GetCurX() - dfSPEED_PLAYER_X, GetCurY() - dfSPEED_PLAYER_Y);
		
		break;

	case dfACTION_MOVE_RD :														//�����ʾƷ�
		SetActionMove(dfACTION_MOVE_RD);

		if ((GetCurX() + dfSPEED_PLAYER_X >= dfRANGE_MOVE_RIGHT) ||
			(GetCurY() + dfSPEED_PLAYER_Y >= dfRANGE_MOVE_BOTTOM))
			SetPosition(GetCurX(), GetCurY());
		else
			SetPosition(GetCurX() + dfSPEED_PLAYER_X, GetCurY() + dfSPEED_PLAYER_Y);
		
		break;

	case dfACTION_MOVE_RU :														//������ ��
		SetActionMove(dfACTION_MOVE_RU);

		if ((GetCurX() + dfSPEED_PLAYER_X >= dfRANGE_MOVE_RIGHT) ||
			(GetCurY() - dfSPEED_PLAYER_Y <= dfRANGE_MOVE_TOP))
			SetPosition(GetCurX(), GetCurY());
		else
			SetPosition(GetCurX() + dfSPEED_PLAYER_X, GetCurY() - dfSPEED_PLAYER_Y);
		
		break;
	}
}

/*---------------------------------------------------------------------------------------------------------*/
// �÷��̾� Ȯ��
// �ش� �÷��̾����� Ȯ���Ѵ�
/*---------------------------------------------------------------------------------------------------------*/
BOOL CPlayerObject::isPlayer()
{
	if (m_bPlayerCharacter == TRUE)	return true;
	else							return false;
}

/*---------------------------------------------------------------------------------------------------------*/
// ���� Ȯ��
// �÷��̾� ���� Ȯ��
/*---------------------------------------------------------------------------------------------------------*/
int CPlayerObject::GetDirection()
{
	return m_iDirCur;
}

/*---------------------------------------------------------------------------------------------------------*/
// ���� �׼� ����
// �ش� ���� �׼��� �����Ѵ�
/*---------------------------------------------------------------------------------------------------------*/
void CPlayerObject::SetActionAttack1()	
{ 
	m_dwActionOld = m_dwActionCur;
	m_dwActionCur = dfACTION_ATTACK1;

	if (m_dwActionOld != m_dwActionCur){
		if (GetDirection() == dfACTION_MOVE_LL)
			SetSprite(ePLAYER_ATTACK1_L01, ePLAYER_ATTACK1_L_MAX, 3);
		else if (GetDirection() == dfACTION_MOVE_RR)
			SetSprite(ePLAYER_ATTACK1_R01, ePLAYER_ATTACK1_R_MAX, 3);
	}

	if (isPlayer() && m_dwActionCur != m_dwActionOld && m_dwActionCur != dfACTION_STAND)
		sendProc_Attack1(GetDirection(), GetCurX(), GetCurY());
}

void CPlayerObject::SetActionAttack2()	
{
	m_dwActionOld = m_dwActionCur;
	m_dwActionCur = dfACTION_ATTACK2;

	if (m_dwActionOld != m_dwActionCur){
		if (GetDirection() == dfACTION_MOVE_LL)
			SetSprite(ePLAYER_ATTACK2_L01, ePLAYER_ATTACK2_L_MAX, 4);
		else if (GetDirection() == dfACTION_MOVE_RR)
			SetSprite(ePLAYER_ATTACK2_R01, ePLAYER_ATTACK2_R_MAX, 4);
	}

	if (isPlayer() && m_dwActionCur != m_dwActionOld && m_dwActionCur != dfACTION_STAND)
		sendProc_Attack2(GetDirection(), GetCurX(), GetCurY());
}

void CPlayerObject::SetActionAttack3()	
{
	m_dwActionOld = m_dwActionCur;
	m_dwActionCur = dfACTION_ATTACK3;
	if (m_dwActionOld != m_dwActionCur){
		if (GetDirection() == dfACTION_MOVE_LL)
			SetSprite(ePLAYER_ATTACK3_L01, ePLAYER_ATTACK3_L_MAX, 4);
		else if (GetDirection() == dfACTION_MOVE_RR)
			SetSprite(ePLAYER_ATTACK3_R01, ePLAYER_ATTACK3_R_MAX, 4);
	}

	if (isPlayer() && m_dwActionCur != m_dwActionOld && m_dwActionCur != dfACTION_STAND)
		sendProc_Attack3(GetDirection(), GetCurX(), GetCurY());
}

/*---------------------------------------------------------------------------------------------*/
// Player Move Setting
/*---------------------------------------------------------------------------------------------*/
void CPlayerObject::SetActionMove(DWORD actionMove)		
{
	m_dwActionOld = m_dwActionCur;
	m_dwActionCur = actionMove;

	switch (actionMove)
	{
	case dfPACKET_MOVE_DIR_RR:
	case dfPACKET_MOVE_DIR_RU:
	case dfPACKET_MOVE_DIR_RD:
		SetDirection(dfPACKET_MOVE_DIR_RR);
		break;

	case dfPACKET_MOVE_DIR_LL:
	case dfPACKET_MOVE_DIR_LU:
	case dfPACKET_MOVE_DIR_LD:
		SetDirection(dfPACKET_MOVE_DIR_LL);
		break;
	}

	if (m_dwActionOld == dfACTION_STAND || GetDirection() != m_iDirOld)
	{
		if (m_iDirCur == dfACTION_MOVE_RR)			SetSprite(ePLAYER_MOVE_R01, ePLAYER_MOVE_R_MAX, 4);
		else if (m_iDirCur == dfACTION_MOVE_LL) 	SetSprite(ePLAYER_MOVE_L01, ePLAYER_MOVE_L_MAX, 4);
	}

	if (isPlayer() && m_dwActionCur != m_dwActionOld && m_dwActionCur != dfACTION_STAND)
		sendProc_MoveStart(m_dwActionCur, GetCurX(), GetCurY());
}

/*---------------------------------------------------------------------------------------------*/
// Player Stand Setting
/*---------------------------------------------------------------------------------------------*/
void CPlayerObject::SetActionStand()	
{
	m_dwActionOld = m_dwActionCur;
	m_dwActionCur = dfACTION_STAND;

	if (m_dwActionOld != dfACTION_STAND){
		if (m_iDirCur == dfACTION_MOVE_LL)			SetSprite(ePLAYER_STAND_L01, ePLAYER_STAND_L_MAX, 5);
		else if (m_iDirCur == dfACTION_MOVE_RR)	SetSprite(ePLAYER_STAND_R01, ePLAYER_STAND_R_MAX, 5);
	}

	if (isPlayer() && m_dwActionCur != m_dwActionOld && m_dwActionCur == dfACTION_STAND &&
		m_dwActionOld != dfACTION_ATTACK1 && m_dwActionOld != dfACTION_ATTACK2 && m_dwActionOld != dfACTION_ATTACK3)
		sendProc_MoveStop(GetDirection(), GetCurX(), GetCurY());
}

/*---------------------------------------------------------------------------------------------*/
// Player Direction Setting
/*---------------------------------------------------------------------------------------------*/
void CPlayerObject::SetDirection(int dir)		
{
	m_iDirOld = m_iDirCur;
	m_iDirCur = dir;
}

/*---------------------------------------------------------------------------------------------*/
// Player HP Setting
/*---------------------------------------------------------------------------------------------*/
void CPlayerObject::SetHP(char hp)				
{
	m_chHP = hp;
}