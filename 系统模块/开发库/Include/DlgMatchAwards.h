#ifndef DLG_MATCH_AWARDS_HEAD_FILE
#define DLG_MATCH_AWARDS_HEAD_FILE

#pragma once

#include "afxwin.h"
#include "GameFrameHead.h"

//////////////////////////////////////////////////////////////////////////

//������״
class GAME_FRAME_CLASS CDlgMatchAwards : public CFGuiDialog
{
	//�ؼ�����
public:
	CMD_GR_MatchResult				*m_pMatchResult;					//�������

	//����ؼ�
protected:
	CButtonUI *						m_pbtClose;							//�رհ�ť

	//����ؼ�
protected:
	CSkinLayered					m_SkinLayered;						//�ֲ㴰��

	//��������
public:
	//���캯��
	CDlgMatchAwards();
	//��������
	virtual ~CDlgMatchAwards();

	//���غ���
protected:
	//ȷ������
	virtual VOID OnCancel();
	//��ʼ������
	virtual BOOL OnInitDialog();

    //�ӿں���
public:
	//��ʼ�ؼ�
	virtual VOID InitControlUI();
	//��Ϣ����
	virtual VOID Notify(TNotifyUI &  msg);	

	//�滭�ӿ�
public:
	//��ʼ�滭
	virtual VOID OnBeginPaintWindow(HDC hDC);
	
	//���ܺ���
public:
	//���ò���
	VOID SetMatchResult(CMD_GR_MatchResult *pMatchResult);
	//���ƶ����ı�
	VOID DrawMultLineText(CDC *pDC, CString strText, CRect rect, int nRowDis, UINT nFromat);
};

#endif