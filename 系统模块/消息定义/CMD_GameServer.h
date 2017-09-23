#ifndef CMD_GAME_SERVER_HEAD_FILE
#define CMD_GAME_SERVER_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//��¼����

#define MDM_GR_LOGON				1									//��¼��Ϣ

//��¼ģʽ
#define SUB_GR_LOGON_USERID			1									//I D ��¼
#define SUB_GR_LOGON_MOBILE			2									//�ֻ���¼
#define SUB_GR_LOGON_ACCOUNTS		3									//�ʻ���¼

//��¼���
#define SUB_GR_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_GR_LOGON_FAILURE		101									//��¼ʧ��
#define SUB_GR_LOGON_FINISH			102									//��¼���

//������ʾ
#define SUB_GR_UPDATE_NOTIFY		200									//������ʾ

//////////////////////////////////////////////////////////////////////////////////

//I D ��¼
struct CMD_GR_LogonUserID
{
	//�汾��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	DWORD							dwFrameVersion;						//��ܰ汾
	DWORD							dwProcessVersion;					//���̰汾

	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR                           szServerPasswd[LEN_PASSWORD];       //��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	WORD							wKindID;							//��������
};

//�ֻ���¼
struct CMD_GR_LogonMobile
{
	//�汾��Ϣ
	WORD							wGameID;							//��Ϸ��ʶ
	DWORD							dwProcessVersion;					//���̰汾

	//��������
	BYTE                            cbDeviceType;                       //�豸����
	WORD                            wBehaviorFlags;                     //��Ϊ��ʶ
	WORD                            wPageTableCount;                    //��ҳ����

	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR                           szServerPasswd[LEN_PASSWORD];       //��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//�ʺŵ�¼
struct CMD_GR_LogonAccounts
{
	//�汾��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	DWORD							dwFrameVersion;						//��ܰ汾
	DWORD							dwProcessVersion;					//���̰汾

	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��¼�ɹ�
struct CMD_GR_LogonSuccess
{
	DWORD							dwUserRight;						//�û�Ȩ��
	DWORD							dwMasterRight;						//����Ȩ��
};

//��¼ʧ��
struct CMD_GR_LogonFailure
{
	LONG							lErrorCode;							//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��¼���
struct CMD_GR_LogonFinish
{
	bool							bGuideTask;							//��������
};

//������ʾ
struct CMD_GR_UpdateNotify
{
	//������־
	BYTE							cbMustUpdatePlaza;					//ǿ������
	BYTE							cbMustUpdateClient;					//ǿ������
	BYTE							cbAdviceUpdateClient;				//��������

	//��ǰ�汾
	DWORD							dwCurrentPlazaVersion;				//��ǰ�汾
	DWORD							dwCurrentFrameVersion;				//��ǰ�汾
	DWORD							dwCurrentClientVersion;				//��ǰ�汾
};

//////////////////////////////////////////////////////////////////////////////////
//��������

#define MDM_GR_CONFIG				2									//������Ϣ

#define SUB_GR_CONFIG_COLUMN		100									//�б�����
#define SUB_GR_CONFIG_SERVER		101									//��������
#define SUB_GR_CONFIG_PROPERTY		102									//��������
#define SUB_GR_CONFIG_FINISH		103									//�������
#define SUB_GR_CONFIG_USER_RIGHT	104									//���Ȩ��

//////////////////////////////////////////////////////////////////////////////////

//�б�����
struct CMD_GR_ConfigColumn
{
	BYTE							cbColumnCount;						//�б���Ŀ
	tagColumnItem					ColumnItem[MAX_COLUMN];				//�б�����
};

//��������
struct CMD_GR_ConfigServer
{
	//��������
	WORD							wTableCount;						//������Ŀ
	WORD							wChairCount;						//������Ŀ

	//��������
	WORD							wServerType;						//��������
	DWORD							dwServerRule;						//�������
};

//��������
struct CMD_GR_ConfigProperty
{
	BYTE							cbPropertyCount;					//������Ŀ
	tagPropertyInfo					PropertyInfo[MAX_PROPERTY];			//��������
};

//���Ȩ��
struct CMD_GR_ConfigUserRight
{
	DWORD							dwUserRight;						//���Ȩ��
};
//////////////////////////////////////////////////////////////////////////////////
//�û�����

#define MDM_GR_USER					3									//�û���Ϣ

//�û�����
#define SUB_GR_USER_RULE			1									//�û�����
#define SUB_GR_USER_LOOKON			2									//�Թ�����
#define SUB_GR_USER_SITDOWN			3									//��������
#define SUB_GR_USER_STANDUP			4									//��������
#define SUB_GR_USER_INVITE			5									//�û�����
#define SUB_GR_USER_INVITE_REQ		6									//��������
#define SUB_GR_USER_REPULSE_SIT  	7									//�ܾ��������
#define SUB_GR_USER_KICK_USER       8                                   //�߳��û�
#define SUB_GR_USER_INFO_REQ        9                                   //�����û���Ϣ
#define SUB_GR_USER_CHAIR_REQ       10                                  //�������λ��
#define SUB_GR_USER_CHAIR_INFO_REQ  11                                  //���������û���Ϣ
#define SUB_GR_USER_WAIT_DISTRIBUTE 12									//�ȴ�����
#define SUB_GR_USER_CREATE_ROOM		13									//������������
#define SUB_GR_USER_JOIN_ROOM		14									//���뷿������
#define SUB_GR_USER_DISMISS_ROOM	15									//��ɢ����
#define SUB_GR_USER_CONTINUE		16									//����

//�û�״̬
#define SUB_GR_USER_ENTER			100									//�û�����
#define SUB_GR_USER_SCORE			101									//�û�����
#define SUB_GR_USER_STATUS			102									//�û�״̬
#define SUB_GR_REQUEST_FAILURE		103									//����ʧ��
#define SUB_GR_USER_GAME_DATA		104									//�û���Ϸ����
#define SUB_GR_CREATE_SUCCESS		105									//�����ɹ�
#define SUB_GR_DISMISS_SUCCESS		106									//��ɢ�ɹ�
#define SUB_GR_DISMISS_MSG			107									//��ɢ��Ϣ
#define SUB_GR_FANGZHU_DISMISS		108									//������ɢ
#define SUB_GR_DISMISS_FAIL			109									//��ɢʧ��
#define SUB_GR_POSITION				110									//IP��ַ

//��������
#define SUB_GR_USER_CHAT			201									//������Ϣ
#define SUB_GR_USER_EXPRESSION		202									//������Ϣ
#define SUB_GR_WISPER_CHAT			203									//˽����Ϣ
#define SUB_GR_WISPER_EXPRESSION	204									//˽�ı���
#define SUB_GR_COLLOQUY_CHAT		205									//�Ự��Ϣ
#define SUB_GR_COLLOQUY_EXPRESSION	206									//�Ự����

//��������
#define SUB_GR_PROPERTY_BUY			300									//�������
#define SUB_GR_PROPERTY_SUCCESS		301									//���߳ɹ�
#define SUB_GR_PROPERTY_FAILURE		302									//����ʧ��
#define SUB_GR_PROPERTY_MESSAGE     303                                 //������Ϣ
#define SUB_GR_PROPERTY_EFFECT      304                                 //����ЧӦ
#define SUB_GR_PROPERTY_TRUMPET		305                                 //������Ϣ


//�ȼ�����
#define SUB_GR_GROWLEVEL_QUERY			410								//��ѯ�ȼ�
#define SUB_GR_GROWLEVEL_PARAMETER		411								//�ȼ�����
#define SUB_GR_GROWLEVEL_UPGRADE		412								//�ȼ�����
//////////////////////////////////////////////////////////////////////////////////

//�Թ�����
struct CMD_GR_UserLookon
{
	WORD							wTableID;							//����λ��
	WORD							wChairID;							//����λ��
};

//��������
struct CMD_GR_UserSitDown
{
	WORD							wTableID;							//����λ��
	WORD							wChairID;							//����λ��
	TCHAR							szPassword[LEN_PASSWORD];			//��������
};

//��������
struct CMD_GR_UserStandUp
{
	WORD							wTableID;							//����λ��
	WORD							wChairID;							//����λ��
	BYTE							cbForceLeave;						//ǿ���뿪
};

//�����û� 
struct CMD_GR_UserInvite
{
	WORD							wTableID;							//���Ӻ���
	DWORD							dwUserID;							//�û� I D
};

//�����û����� 
struct CMD_GR_UserInviteReq
{
	WORD							wTableID;							//���Ӻ���
	DWORD							dwUserID;							//�û� I D
};

//�û�����
struct CMD_GR_UserScore
{
	DWORD							dwUserID;							//�û���ʶ
	tagUserScore					UserScore;							//������Ϣ
};

//�û�����
struct CMD_GR_MobileUserScore
{
	DWORD							dwUserID;							//�û���ʶ
	tagMobileUserScore				UserScore;							//������Ϣ
};

//�û�״̬
struct CMD_GR_UserStatus
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szSendNickName[32];				    //����ǳ�
	tagUserStatus					UserStatus;							//�û�״̬
	DWORD							dwUserIP;							//�û�ip
	DWORD							dwUserScore;
};

//�û���λ
struct CMD_GR_Position
{
	DWORD							dwUserID;							//�û���ʶ
	double							dLongitude;							//����
	double							dLatitude;							//γ��
};

//�û���Ϸ����
struct CMD_GR_UserGameData
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szUserGameData[LEN_GAME_DATA];		//��Ϸ����					
};

//�����ɹ�
struct CMD_GR_CreateSuccess
{
	DWORD							dwRoomID;
	WORD							wChairID;
};

//��������
struct CMD_GR_Create_Room
{
	bool							bRefuseSameIP;							//����ͬIP
	bool							bDistanceIn300;							//����300��
	double							dLongitude;								//����
	double							dLatitude;								//γ��
	BYTE							cbGameCount;							//����
	BYTE							cbPayType;								//֧����ʽ
	BYTE							cbPlayerCount;							//��Ϸ����
	WORD							wKind;									//��Ϸ����
	bool							bKing;									//��С��
};

//���뷿��
struct CMD_GR_Join_Room
{
	DWORD							dwRoomNum;								//����ID
	double							dLongitude;								//����
	double							dLatitude;								//γ��
};

//��ɢ����
struct CMD_GR_DismissRoom
{
	DWORD							wUserID;
	bool							bAgree;
	DWORD							dwTimeLeave;
};


//����ʧ��
struct CMD_GR_RequestFailure
{
	LONG							lErrorCode;							//�������
	TCHAR							szDescribeString[256];				//������Ϣ
};


//�û�����
struct CMD_GR_C_UserChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�����
struct CMD_GR_S_UserChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�����
struct CMD_GR_C_UserExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�û�����
struct CMD_GR_S_UserExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�û�˽��
struct CMD_GR_C_WisperChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�˽��
struct CMD_GR_S_WisperChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//˽�ı���
struct CMD_GR_C_WisperExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//˽�ı���
struct CMD_GR_S_WisperExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�û��Ự
struct CMD_GR_ColloquyChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwConversationID;					//�Ự��ʶ
	DWORD							dwTargetUserID[16];					//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�����û�
struct CMD_GR_C_InviteUser
{
	WORD							wTableID;							//���Ӻ���
	DWORD							dwSendUserID;						//�����û�
};

//�����û�
struct CMD_GR_S_InviteUser
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�������
struct CMD_GR_C_PropertyBuy
{
	BYTE                            cbRequestArea;						//����Χ
	BYTE							cbConsumeScore;						//��������
	WORD							wItemCount;							//������Ŀ
	WORD							wPropertyIndex;						//��������	
	DWORD							dwTargetUserID;						//ʹ�ö���
};

//���߳ɹ�
struct CMD_GR_S_PropertySuccess
{
	BYTE                            cbRequestArea;						//ʹ�û���
	WORD							wItemCount;							//������Ŀ
	WORD							wPropertyIndex;						//��������
	DWORD							dwSourceUserID;						//Ŀ�����
	DWORD							dwTargetUserID;						//ʹ�ö���
};

//����ʧ��
struct CMD_GR_PropertyFailure
{
	WORD                            wRequestArea;                       //��������
	LONG							lErrorCode;							//�������
	TCHAR							szDescribeString[256];				//������Ϣ
};

//������Ϣ
struct CMD_GR_S_PropertyMessage
{
	//������Ϣ
	WORD                            wPropertyIndex;                     //��������
	WORD                            wPropertyCount;                     //������Ŀ
	DWORD                           dwSourceUserID;                     //Ŀ�����
	DWORD                           dwTargerUserID;                     //ʹ�ö���
};


//����ЧӦ
struct CMD_GR_S_PropertyEffect
{
	DWORD                           wUserID;					        //�� ��I D
	BYTE							cbMemberOrder;						//��Ա�ȼ�
};

//��������
struct CMD_GR_C_SendTrumpet
{
	BYTE                           cbRequestArea;                        //����Χ 
	WORD                           wPropertyIndex;                      //�������� 
	DWORD                          TrumpetColor;                        //������ɫ
	TCHAR                          szTrumpetContent[TRUMPET_MAX_CHAR];  //��������
};

//��������
struct CMD_GR_S_SendTrumpet
{
	WORD                           wPropertyIndex;                      //�������� 
	DWORD                          dwSendUserID;                         //�û� I D
	DWORD                          TrumpetColor;                        //������ɫ
	TCHAR                          szSendNickName[32];				    //����ǳ� 
	TCHAR                          szTrumpetContent[TRUMPET_MAX_CHAR];  //��������
};


//�û��ܾ�����������
struct CMD_GR_UserRepulseSit
{
	WORD							wTableID;							//���Ӻ���
	WORD							wChairID;							//����λ��
	DWORD							dwUserID;							//�û� I D
	DWORD							dwRepulseUserID;					//�û� I D
};

//////////////////////////////////////////////////////////////////////////////////

//�����־
#define UR_LIMIT_SAME_IP			0x01								//���Ƶ�ַ
#define UR_LIMIT_WIN_RATE			0x02								//����ʤ��
#define UR_LIMIT_FLEE_RATE			0x04								//��������
#define UR_LIMIT_GAME_SCORE			0x08								//���ƻ���

//�û�����
struct CMD_GR_UserRule
{
	BYTE							cbRuleMask;							//��������
	WORD							wMinWinRate;						//���ʤ��
	WORD							wMaxFleeRate;						//�������
	LONG							lMaxGameScore;						//��߷��� 
	LONG							lMinGameScore;						//��ͷ���
};

//�����û���Ϣ
struct CMD_GR_UserInfoReq
{
	DWORD                           dwUserIDReq;                        //�����û�
	WORD							wTablePos;							//����λ��
};

//�����û���Ϣ
struct CMD_GR_ChairUserInfoReq
{
	WORD							wTableID;							//���Ӻ���
	WORD							wChairID;							//����λ��
};

//////////////////////////////////////////////////////////////////////////////////
//�ȼ�����

//��ѯ�ȼ�
struct CMD_GR_GrowLevelQueryInfo
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�ȼ�����
struct CMD_GR_GrowLevelConfig
{
	WORD							wLevelCount;						//�ȼ���Ŀ
	tagGrowLevelConfig				GrowLevelItem[60];					//�ȼ�����
};

//�ȼ�����
struct CMD_GR_GrowLevelParameter
{
	WORD							wCurrLevelID;						//��ǰ�ȼ�
	DWORD							dwExperience;						//��ǰ����
	DWORD							dwUpgradeExperience;				//�¼�����
	SCORE							lUpgradeRewardGold;					//��������
	SCORE							lUpgradeRewardIngot;				//��������
};

//�ȼ�����
struct CMD_GR_GrowLevelUpgrade
{
	SCORE							lCurrScore;							//��ǰ��Ϸ��
	SCORE							lCurrIngot;							//��ǰԪ��
	TCHAR							szNotifyContent[128];				//��ʾ����
};

//////////////////////////////////////////////////////////////////////////////////
//״̬����

#define MDM_GR_STATUS				4									//״̬��Ϣ

#define SUB_GR_TABLE_INFO			100									//������Ϣ
#define SUB_GR_TABLE_STATUS			101									//����״̬

//////////////////////////////////////////////////////////////////////////////////

//������Ϣ
struct CMD_GR_TableInfo
{
	WORD							wTableCount;						//������Ŀ
	tagTableStatus					TableStatusArray[512];				//����״̬
};

//����״̬
struct CMD_GR_TableStatus
{
	WORD							wTableID;							//���Ӻ���
	tagTableStatus					TableStatus;						//����״̬
};

//////////////////////////////////////////////////////////////////////////////////
//��������

#define MDM_GR_INSURE				5									//�û���Ϣ

//��������
#define SUB_GR_ENABLE_INSURE_REQUEST 1									//��ͨ����
#define SUB_GR_QUERY_INSURE_INFO	2									//��ѯ����
#define SUB_GR_SAVE_SCORE_REQUEST	3									//������
#define SUB_GR_TAKE_SCORE_REQUEST	4									//ȡ�����
#define SUB_GR_TRANSFER_SCORE_REQUEST	5								//ȡ�����
#define SUB_GR_QUERY_USER_INFO_REQUEST	6								//��ѯ�û�

#define SUB_GR_USER_INSURE_INFO		100									//��������
#define SUB_GR_USER_INSURE_SUCCESS	101									//���гɹ�
#define SUB_GR_USER_INSURE_FAILURE	102									//����ʧ��
#define SUB_GR_USER_TRANSFER_USER_INFO	103								//�û�����
#define SUB_GR_USER_INSURE_ENABLE_RESULT 104							//��ͨ���

//////////////////////////////////////////////////////////////////////////////////

//��ͨ����
struct CMD_GR_C_EnableInsureRequest
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwUserID;							//�û�I D
	TCHAR							szLogonPass[LEN_PASSWORD];			//��¼����
	TCHAR							szInsurePass[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ѯ����
struct CMD_GR_C_QueryInsureInfoRequest
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	TCHAR							szInsurePass[LEN_PASSWORD];			//��������
};

//�������
struct CMD_GR_C_SaveScoreRequest
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	SCORE							lSaveScore;							//�����Ŀ
};

//ȡ������
struct CMD_GR_C_TakeScoreRequest
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	SCORE							lTakeScore;							//ȡ����Ŀ
	TCHAR							szInsurePass[LEN_PASSWORD];			//��������
};

//ת�˽��
struct CMD_GP_C_TransferScoreRequest
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	SCORE							lTransferScore;						//ת�˽��
	TCHAR							szAccounts[LEN_ACCOUNTS];			//Ŀ���û�
	TCHAR							szInsurePass[LEN_PASSWORD];			//��������
	TCHAR							szTransRemark[LEN_TRANS_REMARK];	//ת�˱�ע
};

//��ѯ�û�
struct CMD_GR_C_QueryUserInfoRequest
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	BYTE                            cbByNickName;                       //�ǳ�����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//Ŀ���û�
};

//��������
struct CMD_GR_S_UserInsureInfo
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	BYTE							cbEnjoinTransfer;					//ת�˿���
	WORD							wRevenueTake;						//˰�ձ���
	WORD							wRevenueTransfer;					//˰�ձ���
	WORD							wRevenueTransferMember;				//˰�ձ���	
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
	SCORE							lTransferPrerequisite;				//ת������
};

//���гɹ�
struct CMD_GR_S_UserInsureSuccess
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	SCORE							lUserScore;							//���Ͻ��
	SCORE							lUserInsure;						//���н��
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct CMD_GR_S_UserInsureFailure
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	LONG							lErrorCode;							//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�û���Ϣ
struct CMD_GR_S_UserTransferUserInfo
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwTargetGameID;						//Ŀ���û�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//Ŀ���û�
};

//��ͨ���
struct CMD_GR_S_UserInsureEnableResult
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	BYTE							cbInsureEnabled;					//ʹ�ܱ�ʶ
	TCHAR							szDescribeString[128];				//������Ϣ
};

//////////////////////////////////////////////////////////////////////////////////
//��������
#define MDM_GR_TASK					6									//��������

#define SUB_GR_TASK_LOAD_INFO		1									//��������
#define SUB_GR_TASK_TAKE			2									//��ȡ����
#define SUB_GR_TASK_REWARD			3									//������

#define SUB_GR_TASK_INFO			4									//������Ϣ
#define SUB_GR_TASK_FINISH			5									//�������
#define SUB_GR_TASK_LIST			6									//�����б�
#define SUB_GR_TASK_RESULT			7									//������

//////////////////////////////////////////////////////////////////////////////////
//��������
struct CMD_GR_C_LoadTaskInfo
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
};

//��ȡ����
struct CMD_GR_C_TakeTask
{
	WORD							wTaskID;							//�����ʶ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ȡ����
struct CMD_GR_C_TaskReward
{
	WORD							wTaskID;							//�����ʶ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//������Ϣ
struct CMD_GR_S_TaskInfo
{
	WORD							wTaskCount;							//��������
	tagTaskStatus					TaskStatus[TASK_MAX_COUNT];			//����״̬
};

//�������
struct CMD_GR_S_TaskFinish
{
	WORD							wFinishTaskID;						//�����ʶ
	TCHAR							szTaskName[LEN_TASK_NAME];			//��������
};

//������
struct CMD_GR_S_TaskResult
{
	//�����Ϣ
	bool							bSuccessed;							//�ɹ���ʶ
	WORD							wCommandID;							//�����ʶ

	//�Ƹ���Ϣ
	SCORE							lCurrScore;							//��ǰ���
	SCORE							lCurrIngot;							//��ǰԪ��
	
	//��ʾ��Ϣ
	TCHAR							szNotifyContent[128];				//��ʾ����
};


//////////////////////////////////////////////////////////////////////////////////
//�һ�����

#define MDM_GR_EXCHANGE					7								//�һ�����

#define SUB_GR_EXCHANGE_LOAD_INFO		400								//������Ϣ
#define SUB_GR_EXCHANGE_PARAM_INFO		401								//�һ�����
#define SUB_GR_PURCHASE_MEMBER			402								//�����Ա
#define SUB_GR_PURCHASE_RESULT			403								//������
#define SUB_GR_EXCHANGE_SCORE_BYINGOT	404								//�һ���Ϸ��
#define SUB_GR_EXCHANGE_SCORE_BYBEANS	405								//�һ���Ϸ��
#define SUB_GR_EXCHANGE_RESULT			406								//�һ����

//////////////////////////////////////////////////////////////////////////////////

//��ѯ����
struct CMD_GR_ExchangeParameter
{
	DWORD							dwExchangeRate;						//Ԫ����Ϸ�Ҷһ�����
	DWORD							dwPresentExchangeRate;				//������Ϸ�Ҷһ���
	DWORD							dwRateGold;							//��Ϸ����Ϸ�Ҷһ���
	WORD							wMemberCount;						//��Ա��Ŀ
	tagMemberParameter				MemberParameter[10];				//��Ա����
};

//�����Ա
struct CMD_GR_PurchaseMember
{
	DWORD							dwUserID;							//�û���ʶ
	BYTE							cbMemberOrder;						//��Ա��ʶ
	WORD							wPurchaseTime;						//����ʱ��
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//������
struct CMD_GR_PurchaseResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	BYTE							cbMemberOrder;						//��Աϵ��
	SCORE							lCurrScore;							//��ǰ��Ϸ��
	DOUBLE							dCurrBeans;							//��ǰ��Ϸ��
	TCHAR							szNotifyContent[128];				//��ʾ����
};

//�һ���Ϸ��
struct CMD_GR_ExchangeScoreByIngot
{
	DWORD							dwUserID;							//�û���ʶ
	SCORE							lExchangeIngot;						//�һ�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//�һ���Ϸ��
struct CMD_GR_ExchangeScoreByBeans
{
	DWORD							dwUserID;							//�û���ʶ
	double							dExchangeBeans;						//�һ�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//�һ����
struct CMD_GR_ExchangeResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	SCORE							lCurrScore;							//��ǰ��Ϸ��
	SCORE							lCurrIngot;							//��ǰԪ��
	double							dCurrBeans;							//��ǰ��Ϸ��
	TCHAR							szNotifyContent[128];				//��ʾ����
};

//////////////////////////////////////////////////////////////////////////////////
//��������

#define MDM_GR_MANAGE				8									//��������

#define SUB_GR_SEND_WARNING			1									//���;���
#define SUB_GR_SEND_MESSAGE			2									//������Ϣ
#define SUB_GR_LOOK_USER_IP			3									//�鿴��ַ
#define SUB_GR_KILL_USER			4									//�߳��û�
#define SUB_GR_LIMIT_ACCOUNS		5									//�����ʻ�
#define SUB_GR_SET_USER_RIGHT		6									//Ȩ������

//��������
#define SUB_GR_QUERY_OPTION			7									//��ѯ����
#define SUB_GR_OPTION_SERVER		8									//��������
#define SUB_GR_OPTION_CURRENT		9									//��ǰ����
#define SUB_GR_LIMIT_USER_CHAT		10									//��������
#define SUB_GR_KICK_ALL_USER		11									//�߳��û�
#define SUB_GR_DISMISSGAME		    12									//��ɢ��Ϸ

//////////////////////////////////////////////////////////////////////////////////

//���;���
struct CMD_GR_SendWarning
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szWarningMessage[LEN_USER_CHAT];	//������Ϣ
};

//ϵͳ��Ϣ
struct CMD_GR_SendMessage
{
	BYTE							cbGame;								//��Ϸ��Ϣ
	BYTE							cbRoom;								//��Ϸ��Ϣ
	BYTE							cbAllRoom;							//��Ϸ��Ϣ
	BYTE                            cbLoop;                             //ѭ����־
	DWORD                           dwTimeRate;                         //ѭ�����
	__time64_t                      tConcludeTime;                      //����ʱ��
	WORD							wChatLength;						//��Ϣ����
	TCHAR							szSystemMessage[LEN_USER_CHAT];		//ϵͳ��Ϣ
};

//�鿴��ַ
struct CMD_GR_LookUserIP
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�߳��û�
struct CMD_GR_KickUser
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�����ʻ�
struct CMD_GR_LimitAccounts
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//Ȩ������
struct CMD_GR_SetUserRight
{
	//Ŀ���û�
	DWORD							dwTargetUserID;						//Ŀ���û�

	//�󶨱���
	BYTE							cbGameRight;						//�ʺ�Ȩ��
	BYTE							cbAccountsRight;					//�ʺ�Ȩ��

	//Ȩ�ޱ仯
	BYTE							cbLimitRoomChat;					//��������
	BYTE							cbLimitGameChat;					//��Ϸ����
	BYTE							cbLimitPlayGame;					//��ϷȨ��
	BYTE							cbLimitSendWisper;					//������Ϣ
	BYTE							cbLimitLookonGame;					//�Թ�Ȩ��
};

//��������
struct CMD_GR_OptionCurrent
{
	DWORD							dwRuleMask;							//��������
	tagServerOptionInfo				ServerOptionInfo;					//��������
};

//��������
struct CMD_GR_ServerOption
{
	tagServerOptionInfo				ServerOptionInfo;					//��������
};

//�߳������û�
struct CMD_GR_KickAllUser
{
	TCHAR							szKickMessage[LEN_USER_CHAT];		//�߳���ʾ
};

//��ɢ��Ϸ
struct CMD_GR_DismissGame
{
	WORD							wDismissTableNum;		            //��ɢ����
};
//////////////////////////////////////////////////////////////////////////////////

//���ñ�־
#define OSF_ROOM_CHAT				1									//��������
#define OSF_GAME_CHAT				2									//��Ϸ����
#define OSF_ROOM_WISPER				3									//����˽��
#define OSF_ENTER_TABLE				4									//������Ϸ
#define OSF_ENTER_SERVER			5									//���뷿��
#define OSF_SEND_BUGLE				12									//��������

//��������
struct CMD_GR_OptionServer
{
	BYTE							cbOptionFlags;						//���ñ�־
	BYTE							cbOptionValue;						//���ñ�־
};

//��������
struct CMD_GR_LimitUserChat
{
	DWORD							dwTargetUserID;						//Ŀ���û�
	BYTE							cbLimitFlags;						//���Ʊ�־
	BYTE							cbLimitValue;						//�������
};

//////////////////////////////////////////////////////////////////////////////////
//��������

#define MDM_GR_MATCH				9									//��������

#define SUB_GR_MATCH_FEE			400									//��������
#define SUB_GR_MATCH_NUM			401									//�ȴ�����
#define SUB_GR_LEAVE_MATCH			402									//�˳�����
#define SUB_GR_MATCH_INFO			403									//������Ϣ
#define SUB_GR_MATCH_WAIT_TIP		404									//�ȴ���ʾ
#define SUB_GR_MATCH_RESULT			405									//�������
#define SUB_GR_MATCH_STATUS			406									//����״̬
#define SUB_GR_USER_MATCH_STATUS	407									//����״̬
#define SUB_GR_MATCH_GOLDUPDATE		409									//��Ҹ���
#define SUB_GR_MATCH_ELIMINATE		410									//������̭

////////////////�Ķ�����ʱ �뽫��Ϸ����CMD_GAME.H��ͬʱ�Ķ�////////////////////////////
//#define SUB_GR_MATCH_INFO_ER_SPARROWS	410									//������Ϣ(2���齫)

//��������
struct CMD_GR_Match_Fee
{
	SCORE							lMatchFee;							//��������
	TCHAR							szNotifyContent[128];				//��ʾ����
};

//��������
struct CMD_GR_Match_Num
{
	DWORD							dwWaitting;							//�ȴ�����
	DWORD							dwTotal;							//��������
};

//������Ϣ
struct CMD_GR_Match_Info
{
	TCHAR							szTitle[4][64];						//��Ϣ����
    WORD							wGameCount;							//��Ϸ����
};

//��ʾ��Ϣ
struct CMD_GR_Match_Wait_Tip
{
	SCORE							lScore;								//��ǰ����
	WORD							wRank;								//��ǰ����
	WORD							wCurTableRank;						//��������
	WORD							wUserCount;							//��ǰ����
	WORD							wCurGameCount;						//��ǰ����
	WORD							wGameCount;							//�ܹ�����
	WORD							wPlayingTable;						//��Ϸ����
	TCHAR							szMatchName[LEN_SERVER];			//��������
};

//�������
struct CMD_GR_MatchResult
{	
	SCORE							lGold;								//��ҽ���
	DWORD							dwIngot;							//Ԫ������
	DWORD							dwExperience;						//���齱��
	TCHAR							szDescribe[256];					//�ý�����
};

#define MAX_MATCH_DESC				4									//�������

//��������
struct CMD_GR_MatchDesc
{
	TCHAR							szTitle[MAX_MATCH_DESC][16];		//��Ϣ����
	TCHAR							szDescribe[MAX_MATCH_DESC][64];		//��������
	COLORREF						crTitleColor;						//������ɫ
	COLORREF						crDescribeColor;					//������ɫ
};

//��Ҹ���
struct CMD_GR_MatchGoldUpdate
{
	SCORE							lCurrGold;							//��ǰ���
	SCORE							lCurrIngot;							//��ǰԪ��
	DWORD							dwCurrExprience;					//��ǰ����
};

//����״̬
struct CMD_GR_UserMatchStatus
{
	DWORD							dwUserID;							//�û���ʶ
	BYTE							cbOldStatus;						//ԭ��״̬
	BYTE							cbCurrStatus;						//����״̬
};

//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//�������

#define MDM_GF_FRAME				100									//�������

//////////////////////////////////////////////////////////////////////////////////
//�������

//�û�����
#define SUB_GF_GAME_OPTION			1									//��Ϸ����
#define SUB_GF_USER_READY			2									//�û�׼��
#define SUB_GF_LOOKON_CONFIG		3									//�Թ�����
#define SUB_GF_HEADURL				4									//ͷ���ַ
#define SUB_GF_XUFEI				5									//������Ϣ

//��������
#define SUB_GF_USER_CHAT			10									//�û�����
#define SUB_GF_USER_EXPRESSION		11									//�û�����
#define SUB_GF_SOUND				12									//��������
#define SUB_GF_HEAD					13									//����ͷ��
#define SUB_GF_NAME					14									//��������


//¼������
#define SUB_GF_START_RECORD			20									//��ʼ¼��
#define SUB_GF_STOP_RECORD			21									//����¼��

//��Ϸ��Ϣ
#define SUB_GF_GAME_STATUS			100									//��Ϸ״̬
#define SUB_GF_GAME_SCENE			101									//��Ϸ����
#define SUB_GF_LOOKON_STATUS		102									//�Թ�״̬

//ϵͳ��Ϣ
#define SUB_GF_SYSTEM_MESSAGE		200									//ϵͳ��Ϣ
#define SUB_GF_ACTION_MESSAGE		201									//������Ϣ

//////////////////////////////////////////////////////////////////////////////////
//¼��ʼ
struct CMD_GF_RECORD
{
	BYTE							cbGameCount;
};

//����
struct CMD_GF_SOUND
{
	WORD							wChairID;
	bool							bEnd;
	WORD							wLength;
	char							szSound[1024];						
};

//����
struct CMD_GF_XUFEI
{
	WORD							wChairID;
	bool							bFangzhu;
};

//ͷ���ַ
struct CMD_GF_HeadAddress
{
	WORD							wChairID;
	char							szDownLoadAddress[MAX_PATH];				//���ص�ַ
};

//�����ɢ
struct CMD_GF_UserDismiss
{
	WORD							wChairID;
	bool							bDismiss;
};

//��Ϸ����
struct CMD_GF_GameOption
{
	BYTE							cbAllowLookon;							//�Թ۱�־
	DWORD							dwFrameVersion;							//��ܰ汾
	DWORD							dwClientVersion;						//��Ϸ�汾
	BYTE							cbOptionType[30];						//��������
};

//�Թ�����
struct CMD_GF_LookonConfig
{
	DWORD							dwUserID;							//�û���ʶ
	BYTE							cbAllowLookon;						//�����Թ�
};

//�Թ�״̬
struct CMD_GF_LookonStatus
{
	BYTE							cbAllowLookon;						//�����Թ�
};

//��Ϸ����
struct CMD_GF_GameStatus
{
	BYTE							cbGameStatus;						//��Ϸ״̬
	BYTE							cbAllowLookon;						//�Թ۱�־
	WORD							wFangzhu;
};

//ͷ���ַ
struct CMD_GF_C_HeadUrl
{
	TCHAR							szHeadUrl[LEN_URL];
};

//ͷ���ַ
struct CMD_GF_S_HeadUrl
{
	WORD							wChairID;
	TCHAR							szHeadUrl[LEN_URL];
};

//�û�����
struct CMD_GF_C_UserChat
{
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�����
struct CMD_GF_S_UserChat
{
	DWORD							dwSendUserID;						//�����û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�����
struct CMD_GF_C_UserExpression
{
	WORD							wItemIndex;							//��������
};

//�û�����
struct CMD_GF_S_UserExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwSendUserID;						//�����û�
	BYTE							bGender;
};

//////////////////////////////////////////////////////////////////////////////////
//��Ϸ����

#define MDM_GF_GAME					200									//��Ϸ����

//////////////////////////////////////////////////////////////////////////////////
//Я����Ϣ

//������Ϣ
#define DTP_GR_TABLE_PASSWORD		1									//��������

//�û�����
#define DTP_GR_NICK_NAME			10									//�û��ǳ�
#define DTP_GR_GROUP_NAME			11									//��������
#define DTP_GR_UNDER_WRITE			12									//����ǩ��

//������Ϣ
#define DTP_GR_USER_NOTE			20									//�û���ע
#define DTP_GR_CUSTOM_FACE			21									//�Զ�ͷ��

//////////////////////////////////////////////////////////////////////////////////

//�������
#define REQUEST_FAILURE_NORMAL		0									//����ԭ��
#define REQUEST_FAILURE_NOGOLD		1									//��Ҳ���
#define REQUEST_FAILURE_NOSCORE		2									//���ֲ���
#define REQUEST_FAILURE_PASSWORD	3									//�������
#define REQUEST_FAILURE_END			4									//��Ϸ����
#define REQUEST_FAILURE_NOZUANSHI	5									//��ʯ����

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif