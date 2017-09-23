#ifndef CMD_LIANGFUTOU_HEAD_FILE
#define CMD_LIANGFUTOU_HEAD_FILE
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID							502									//��Ϸ  ID
#define GAME_PLAYER						4									//��Ϸ����
#define GAME_NAME						TEXT("ʮ��ˮ")						//��Ϸ����
#define GAME_GENRE						(GAME_GENRE_SCORE|GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����
#define HAND_CARD_COUNT					13									//�˿���Ŀ

#define FONT_NUM						5									//����������Ŀ

#define VERSION_SERVER					PROCESS_VERSION(7,0,1)				//����汾
#define VERSION_CLIENT					PROCESS_VERSION(7,0,1)				//����汾

//��Ϸ״̬
#define GS_WK_FREE				    	GAME_STATUS_FREE					//�ȴ���ʼ
#define GS_WK_START				    	GAME_STATUS_PLAY					//��Ϸ����
#define GS_WK_BIPAI						GAME_STATUS_PLAY+1					//����
#define GS_WK_END						GAME_STATUS_PLAY+2					//����
#define GS_WK_SENDCARD					GAME_STATUS_PLAY+3					//����

//////////////////////////////////////////////////////////////////////////////////
//״̬����

#define GAME_SCENE_PLACE_JETTON				GAME_STATUS_PLAY					//�ȴ���ʼ
#define GAME_SCENE_GAME_END					GAME_STATUS_PLAY+1					//��Ϸ����

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//����������ṹ
#define SUB_S_GAME_START				(0x0004)								//��Ϸ��ʼ
#define SUB_S_GAME_COUNT				(0x0008)								//��ǰ����
#define SUB_S_BANKER					(0x0010)								//��ׯ����
#define SUB_S_SEND_CARD					(0x0020)								//��������
#define SUB_S_SHOW_CARD					(0x0040)								//���̯��
#define SUB_S_GAME_COMPARE              (0x0080)								//��Ϸ����
#define SUB_S_GAME_END					(0x0100)								//��Ϸ����
#define SUB_S_GAME_CONCLUDE				(0x0200)								//��Ϸ��ɢ
#define SUB_S_START_TIME				(0x0400)								//��Ϸ��ʼ����ʱ
#define SUB_S_STOP_TIME					(0x4000)								//ֹͣ��ʱ��
#define SUB_S_CONTINUE					(0x8000)								//��Ҽ���
#define TH_SUB_S_GAME_CONCLUDE_OFFLINE			(0x0800)						//��Ϸ��ɢ���ߺ�
//////////////////////////////////////////////////////////////////////////

//��Ϸ��ʼ
struct CMD_S_GameStart 
{
	SCORE								lUserScore;							//��һ���
	WORD								wSpecialType;						//��������
	BYTE								cbBossCount;						//������
};

//������Ϸ
struct CMD_S_Continue
{
	WORD								wUser;								//׼�����
};


//����
struct CMD_S_GameCount
{
	BYTE								cbGameCount;						//��ǰ����
	BYTE								cbAllGameCount;						//�ܾ���
	SCORE								lGameScore[GAME_PLAYER];			//��Ϸ����
	bool								bPlayer[GAME_PLAYER];				//�Ƿ�����Ҵ���
	int									nToltalWinDaoShu[GAME_PLAYER];		//�ܹ�����
};

//�����˿�
struct CMD_S_SendCard
{
	WORD								wBanker;							//��ǰׯ��
	BYTE                                cbHandCardData[HAND_CARD_COUNT];	//�˿�����
	bool								bPlayer[GAME_PLAYER];				//�Ƿ�����Ҵ���
	WORD								wSpecialType;						//��������
	DWORD								dwTimeLeave;						//ʣ��ʱ��
};

//���̯��
struct CMD_S_ShowCard
{
	WORD								wChairID;							//̯�����
};

//��ұ���
struct CMD_S_Compare
{
	bool								bPlayer[GAME_PLAYER];				//�Ƿ�������
	WORD								wSpecialType[GAME_PLAYER];			//��������
	int									nSpecialCompareResult[GAME_PLAYER];	//�������ͱȽϽ��
	BYTE								cbFrontCard[GAME_PLAYER][3];		//���ǰ��
	BYTE								cbMidCard[GAME_PLAYER][5];			//����ж�
	BYTE								cbBackCard[GAME_PLAYER][5];			//��Һ��
	int									nCompareResult[GAME_PLAYER][3];		//ÿһ���ȽϽ��
	bool								bShoot[GAME_PLAYER][GAME_PLAYER];	//��ǹ
	bool								bDaoshui[GAME_PLAYER];				//��ˮ
};

struct CMD_S_StartTime
{
	DWORD									cbStartTime;
};

//��Ϸ����
struct CMD_S_GameEnd
{
	SCORE								lGameScore[GAME_PLAYER];			//��Ϸ����
	int									nToltalWinDaoShu[GAME_PLAYER];		//�ܹ�����
	WORD								wRunUser;							//ȫ�ݴ��û�
	SCORE								lUserScore[GAME_PLAYER];			//��ҷ���
};

//��Ϸ״̬
struct CMD_S_Conclude
{
	SCORE								lUserScore[GAME_PLAYER];			//��ҷ���
	BYTE								cbShootCount[GAME_PLAYER];			//��ǹ����
	BYTE								cbRunCount[GAME_PLAYER];			//ȫ�ݴ����
	BYTE								cbSpecialCount[GAME_PLAYER];		//�����ƴ���
	bool								bDismiss;							//��ɢ
	bool								bJixv;
};

//���׼��
struct CMD_GF_User_Ready
{
	WORD							wChairID;							//�����λID
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ
#define SUB_C_GETBANK					301									//��ׯ����
#define SUB_C_SHOWCARD					302									//���̯��
#define SUB_C_CONTINUE					303									//������Ϸ
#define SUB_C_READY_MESSAGE				310									//�ͻ��˷���׼����Ϣ
#define SUB_C_READY						311									//���׼��

struct CMD_C_GetBank
{
	bool bGet;
};

struct CMD_C_Ready
{
	bool								bReady;
};

//�ֶ���Ϣ
struct CMD_C_ShowCard
{
	BYTE								cbFrontCard[3];						//ǰ���˿�
	BYTE								cbMidCard[5];						//�ж��˿�
	BYTE								cbBackCard[5];						//����˿�
	WORD                                wSpecialType;                       //�Ƿ���������
};
//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif