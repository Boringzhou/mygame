#ifndef DLG_INSURE_GAME_HEAD_FILE
#define DLG_INSURE_GAME_HEAD_FILE

#pragma once

#include "GameFrameHead.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//��Ϸ����
class CDlgInsureGame : public CSkinDialog
{
	//��ʶ����
public:
	bool							m_bSwitchIng;					//ת����ʶ	
	bool							m_blCanStore;                   //����ʶ
	bool                            m_blCanGetOnlyFree;             //ȡ���ʶ

	//������Ϣ
public:
	WORD							m_wRevenueTake;					//˰�ձ���
	WORD							m_wRevenueTransfer;				//˰�ձ���
	SCORE							m_lUserScore;					//�û���Ϸ��
	SCORE							m_lUserInsure;					//������Ϸ��
	
	//����ؼ�
protected:
	CEditUI *						m_pEditAmount;					//�����༭	
	CEditUI *						m_pEditPassword;				//����༭
	CTextUI *						m_pTextForget;					//��������

	//����ؼ�
protected:
	CButtonUI *						m_pButtonSave;					//��ť
	CButtonUI *						m_pButtonTake;					//ȡ�ť
	CButtonUI *						m_pButtonRefresh;				//ˢ�°�ť

	//�ؼ�����
protected:	
	CPngImage						m_ImageNumber;					//����ͼƬ

	//�ӿڱ���
protected:	
	IClientUserItem	*				m_pIMeUserData;					//�û���Ϣ
	IClientKernel *					m_pIClientKernel;				//�ں˽ӿ�

	//��������
public:
	//���캯��
	CDlgInsureGame(CWnd* pParent = NULL);
	//��������
	virtual ~CDlgInsureGame();

	//���غ���
public:		
	//ȷ����Ϣ
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();	
	//��ʼ������
	virtual BOOL OnInitDialog();
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange* pDX);
	//������Ϣ
	virtual BOOL PreTranslateMessage(MSG* pMsg);

    //�ӿں���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	

	//���غ���
public:
	//�滭��Ϣ
	virtual VOID OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight);

	//��������
public:
	//�����ؼ�
	void RectifyControl(INT nWidth, INT nHeight);

	//��������
public:
	//��ȡ��Ϸ��
	SCORE GetUserControlScore(LPCTSTR pszScore);
	//�滭����
	VOID DrawNumberString(CDC * pDC, SCORE lScore, INT nXPos, INT nYPos);
	//ת���ַ�
	VOID SwitchScoreFormat(SCORE lScore, UINT uSpace, LPTSTR pszBuffer, WORD wBufferSize);

	//�ؼ�����
protected:
	//�������
	VOID OnEnChangeScore();
	//��ť
	VOID OnBnClickedSaveScore();
	//ȡ�ť
	VOID OnBnClickedTakeScore();
	//ˢ�°�ť
	VOID OnBnClickedRefreshScore();
	//��������
	VOID OnBnClickedForgetInsure();

	//�ӿں���
public:
	//��ʾ����
	void ShowItem();
	//����λ��
	void SetPostPoint(CPoint Point);

	//���ܺ���
public:
	//�������
	void SetSendInfo(IClientKernel *pClientKernel,IClientUserItem const*pMeUserDatas);
	//������Ϣ
	void SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);

	//��Ϣ����
protected:	
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//��С��Ϣ
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	DECLARE_MESSAGE_MAP()	
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif