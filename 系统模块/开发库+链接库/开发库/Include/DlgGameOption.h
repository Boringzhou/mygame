#ifndef DLG_GAME_OPTION_HEAD_FILE
#define DLG_GAME_OPTION_HEAD_FILE

#pragma once

#include "GameOptionItem.h"
#include "GameFrameHead.h"

//////////////////////////////////////////////////////////////////////////

//��������
typedef CWHArray<IGameOptionItem *, IGameOptionItem *> CGameOptionItemArray;

//////////////////////////////////////////////////////////////////////////

//����ѡ��
class CGeneralOptionItem : public CGameOptionItem
{
	//�Թ�ѡ��
public:
	bool							m_bAllowLookon;						//�����Թ�
	bool							m_bAllowFriendLookon;				//�����Թ�

	//����ѡ��
public:
	bool							m_bAllowSound;						//��������
	bool							m_bAllowBackGroundSound;			//����������
	bool							m_bFullGameView;					//ȫ��ģʽ
	bool							m_bDebarDetestChat;					//������Ϣ
	bool							m_bNotifyUserInOut;					//������Ϣ

	//��������
public:
	//���캯��
	CGeneralOptionItem();

	//��Ϣ����
protected:
	//��ȡ����
	virtual LPCTSTR __cdecl GetItemTitle() { return TEXT("����ѡ��"); }

	//������
protected:
	//��������
	virtual bool __cdecl SaveParameter();
	//Ĭ�ϲ���
	virtual bool __cdecl DefaultParameter();
	//�����ؼ�
	virtual bool __cdecl CreateOptionItem(CWnd * pParentWnd);

	//�ڲ�����
private:
	//���¿���
	VOID UpdateItemControl();
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class GAME_FRAME_CLASS CDlgGameOption : public CSkinDialog
{
	//��������
protected:
	IGameOptionItem *				m_pIGameOptionItem;					//��ǰѡ��
	CGameOptionItemArray			m_GameOptionItemArray;				//��������

	//����ؼ�
protected:
	CButtonUI *						m_pbtOk;							//ȷ����ť
	CButtonUI *						m_pbtCancel;						//ȡ����ť
	CButtonUI *						m_pbtDefault;						//Ĭ�ϰ�ť

	//�ؼ�����
public:
	CSkinTabCtrl					m_SkinTabControl;					//����ѡ��
	CGeneralOptionItem				m_GeneralOptionItem;				//����ѡ��

	//��������
public:
	//���캯��
	CDlgGameOption();
	//��������
	virtual ~CDlgGameOption();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual VOID OnOK();

    //�ӿں���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	

	//���ܺ���
public:
	//��Ϸ����
	bool CreateGameOption(IUnknownEx * pIUnknownEx[], WORD wItemCount);

	//��Ϣӳ��
protected:
	//������Ϣ
	VOID OnNcDestroy();
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//����ı�
	VOID OnTcnSelchangeTabControl(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif