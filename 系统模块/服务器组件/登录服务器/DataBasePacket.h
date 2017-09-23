#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE
 

//////////////////////////////////////////////////////////////////////////////////
//�������ݰ�

//��¼����
#define	DBR_GP_LOGON_GAMEID			1									//I D ��¼
#define	DBR_GP_LOGON_ACCOUNTS		2									//�ʺŵ�¼
#define DBR_GP_REGISTER_ACCOUNTS	3									//ע���ʺ�
#define DBR_GP_VERIFY_INDIVIDUAL	4									//��֤����
#define DBR_GP_LOGON_VISITOR		5									//�ο͵�½

//�ʺŷ���
#define DBR_GP_MODIFY_MACHINE		10									//�޸Ļ���
#define DBR_GP_MODIFY_LOGON_PASS	11									//�޸�����
#define DBR_GP_MODIFY_INSURE_PASS	12									//�޸�����
#define DBR_GP_MODIFY_UNDER_WRITE	13									//�޸�ǩ��
#define DBR_GP_MODIFY_INDIVIDUAL	14									//�޸�����

//ͷ������
#define DBR_GP_MODIFY_SYSTEM_FACE	20									//�޸�ͷ��
#define DBR_GP_MODIFY_CUSTOM_FACE	21									//�޸�ͷ��

//��������
#define DBR_GP_USER_ENABLE_INSURE	30									//��ͨ����
#define DBR_GP_USER_SAVE_SCORE		31									//������Ϸ��
#define DBR_GP_USER_TAKE_SCORE		32									//��ȡ��Ϸ��
#define DBR_GP_USER_TRANSFER_SCORE	33									//ת����Ϸ��

//��ѯ����
#define DBR_GP_QUERY_INDIVIDUAL		40									//��ѯ����
#define DBR_GP_QUERY_INSURE_INFO	41									//��ѯ����
#define DBR_GP_QUERY_USER_INFO	    42									//��ѯ�û�
#define DBR_GP_QUERY_ACCOUNTINFO	44									//��ѯ����
#define DBR_GP_USER_INGAME_SERVERID	45									//��Ϸ״̬

//��������
#define DBR_GP_MATCH_SIGNUP			50									//��������
#define DBR_GP_MATCH_UNSIGNUP		51									//ȡ������

//ϵͳ����
#define DBR_GP_ONLINE_COUNT_INFO	60									//������Ϣ
#define DBR_GP_LOAD_GAME_LIST		61									//�����б�
#define DBR_GP_LOAD_CHECKIN_REWARD	62									//ǩ������
#define DBR_GP_LOAD_TASK_LIST		63									//��������
#define DBR_GP_LOAD_BASEENSURE		64									//���صͱ�
#define DBR_GP_LOAD_MEMBER_PARAMETER 66									//��Ա����
#define DBR_GP_LOAD_PLATFORM_PARAMETER 67								//ƽ̨����
#define DBR_GP_LOAD_GROWLEVEL_CONFIG 68									//�ȼ�����

//��������
#define DBR_GP_GET_PARAMETER		70									//��ȡ����
#define DBR_GP_ADD_PARAMETER		71									//��Ӳ���
#define DBR_GP_MODIFY_PARAMETER		72									//�޸Ĳ���
#define DBR_GP_DELETE_PARAMETER		73									//ɾ������

//ǩ������
#define DBR_GP_CHECKIN_DONE			80									//ִ��ǩ��
#define DBR_GP_CHECKIN_QUERY_INFO	81									//��ѯ��Ϣ

//��������
#define DBR_GP_TASK_TAKE			90									//��ȡ����
#define DBR_GP_TASK_REWARD			91									//������
#define DBR_GP_TASK_QUERY_INFO		92									//��������	

//�ͱ�����
#define DBR_GP_BASEENSURE_TAKE		100									//��ȡ�ͱ�

//�ȼ�����
#define DBR_GP_GROWLEVEL_QUERY_IFNO	 110								//��ѯ�ȼ�
#define DBO_GP_USER_ACCOUNTINFO		112									//�û���Ϣ

//�һ�����
#define DBR_GP_PURCHASE_MEMBER		120									//�����Ա
#define DBR_GP_EXCHANGE_SCORE_INGOT 121									//�һ���Ϸ��
#define DBR_GP_EXCHANGE_SCORE_BEANS 122									//�һ���Ϸ��

//�ƹ�����
#define DBR_GP_QUERY_SPREAD_INFO	130									//�ƹ���Ϣ

//�齱����
#define DBR_GP_LOTTERY_CONFIG_REQ	140									//��������
#define DBR_GP_LOTTERY_START		141									//��ʼ�齱

#define DBR_GP_SIMPLE_LOTTERY		142									//�򵥳齱

//��Ϸ����
#define DBR_GP_QUERY_USER_GAME_DATA	150									//��Ϸ����

//ϵͳ��Ϣ
#define DBR_GP_QUERY_SYSTEM_QQ		160									//��ѯ�ͷ�:DCY by 2017.3.9
#define DBR_GP_QUERY_SYSTEM_INFO	161									//��ѯϵͳ����:DCY by 2017.3.9
#define DBR_GP_QUERY_SYSTEM_CONTACT	162									//��ѯ��ϵ����:DCY by 2017.3.9
#define DBR_GP_QUERY_SYSTEM_SHOP	163									//��ѯ��Ϸ�̳�:DCY by 2017.3.11
#define DBR_GP_EXCHANGE_GAME_SHOP	164									//�һ���Ϸ��Ʒ:DCY by 2017.3.11

//////////////////////////////////////////////////////////////////////////////////

//ID ��¼
struct DBR_GP_LogonGameID
{
	//��¼��Ϣ
	DWORD							dwGameID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	BYTE							cbNeeValidateMBCard;				//�ܱ�У��

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺŵ�¼
struct DBR_GP_LogonAccounts
{
	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//���֤��
	BYTE							cbNeeValidateMBCard;				//�ܱ�У��

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���

	//��չ��Ϣ
	DWORD							dwCheckUserRight;					//���Ȩ��
};

//�ο͵�½
struct DBR_GP_LogonVisitor
{
	//��¼��Ϣ
	BYTE							cbPlatformID;						//ƽ̨���

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺ�ע��
struct DBR_GP_RegisterAccounts
{
	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	TCHAR							szSpreader[LEN_ACCOUNTS];			//�Ƽ��ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

	//�������
	TCHAR							szLogonPass[LEN_PASSWORD];			//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//��֤����
struct DBR_GP_VerifyIndividual
{
	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���

	WORD							wVerifyMask;						//У����֤
	TCHAR							szVerifyContent[LEN_ACCOUNTS];		//��֤����
};

//�޸Ļ���
struct DBR_GP_ModifyMachine
{
	BYTE							cbBind;								//�󶨱�־
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ѯ������Ϣ
struct DBR_GP_QueryAccountInfo
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//�޸�����
struct DBR_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�����
struct DBR_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�ǩ��
struct DBR_GP_ModifyUnderWrite
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��
};

//�޸�ͷ��
struct DBR_GP_ModifySystemFace
{
	//�û���Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�ͷ��
struct DBR_GP_ModifyCustomFace
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//ͼƬ��Ϣ

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�����
struct DBR_GP_ModifyIndividual
{
	//��֤����
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����

	//�ʺ�����
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��

	//�û���Ϣ
	TCHAR							szUserNote[LEN_USER_NOTE];			//�û�˵��
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

	//�绰����
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//�̶��绰
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ƶ��绰

	//��ϵ����
	TCHAR							szQQ[LEN_QQ];						//Q Q ����
	TCHAR							szEMail[LEN_EMAIL];					//�����ʼ�
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//��ϵ��ַ
};

//��ѯ����
struct DBR_GP_QueryIndividual
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//��������
};

//��ѯ�û���Ϸ״̬
struct DBR_GP_QueryUserInGameServerID
{
	DWORD							dwUserID;							//�û� I D
};

//�һ���Ϸ��Ʒ
struct DBR_GP_ExchangeGameShop
{
	DWORD							dwUserID;							//�û�
	DWORD							dwItemID;							//��Ʒ���
	DWORD							dwCount;							//�һ�����
	SCORE							llNumber;							//�绰����
	TCHAR							strName[32];						//�ռ�����
	TCHAR							strAddress[0x7F];					//�ջ���ַ
};

//��ͨ����
struct DBR_GP_UserEnableInsure
{
	DWORD							dwUserID;							//�û� I D	
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szLogonPass[LEN_PASSWORD];			//��¼����
	TCHAR							szInsurePass[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//������Ϸ��
struct DBR_GP_UserSaveScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSaveScore;							//������Ϸ��
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ȡ����Ϸ��
struct DBR_GP_UserTakeScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lTakeScore;							//��ȡ��Ϸ��
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ת����Ϸ��
struct DBR_GP_UserTransferScore
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	SCORE							lTransferScore;						//ת����Ϸ��
	TCHAR							szAccounts[LEN_ACCOUNTS];			//�û��ǳ�
	TCHAR							szPassword[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szTransRemark[LEN_TRANS_REMARK];	//ת�ʱ�ע
};

//��ѯ����
struct DBR_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
};

//��ѯ�û�
struct DBR_GP_QueryInsureUserInfo
{
	BYTE                            cbByNickName;                       //�ǳ�����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//Ŀ���û�
};

//�û�����
struct DBO_GP_UserTransferUserInfo
{
	DWORD							dwGameID;							//�û� I D
	TCHAR							szAccounts[LEN_ACCOUNTS];			//�û��ʺ�
};

//������Ϣ
struct DBR_GP_OnLineCountInfo
{
	WORD							wKindCount;							//������Ŀ
	DWORD							dwOnLineCountSum;					//��������
	DWORD							dwAndroidCountSum;					//��������
	tagOnLineInfoKindEx				OnLineCountKind[MAX_KIND];			//��������
};


//��������
struct DBR_GP_MatchSignup
{
	//������Ϣ
	WORD							wServerID;							//�����ʶ
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������

	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��������
struct DBR_GP_MatchUnSignup
{
	//������Ϣ
	WORD							wServerID;							//�����ʶ
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������

	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ȡ����
struct DBR_GP_GetParameter
{
	WORD							wServerID;							//�����ʶ
};

//��Ӳ���
struct DBR_GP_AddParameter
{
	WORD							wServerID;							//�����ʶ
	tagAndroidParameter				AndroidParameter;					//��������
};

//�޸Ĳ���
struct DBR_GP_ModifyParameter
{
	WORD							wServerID;							//�����ʶ
	tagAndroidParameter				AndroidParameter;					//��������
};

//ɾ������
struct DBR_GP_DeleteParameter
{
	WORD							wServerID;							//�����ʶ
	DWORD							dwBatchID;							//���α�ʶ
};

//��ѯǩ��
struct DBR_GP_CheckInQueryInfo
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
};

//ִ��ǩ��
struct DBR_GP_CheckInDone
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ѯ����
struct DBR_GP_TaskQueryInfo
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ	
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
};

//��ȡ����
struct DBR_GP_TaskTake
{
	//�û���Ϣ
	WORD							wTaskID;							//�����ʶ
	DWORD							dwUserID;							//�û���ʶ	
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//������
struct DBR_GP_TaskReward
{
	//�û���Ϣ
	WORD							wTaskID;							//�����ʶ
	DWORD							dwUserID;							//�û���ʶ	
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ȡ�ͱ�
struct DBR_GP_TakeBaseEnsure
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ѯ�ȼ�
struct DBR_GP_GrowLevelQueryInfo
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�����Ա
struct DBR_GP_PurchaseMember
{
	DWORD							dwUserID;							//�û� I D
	BYTE							cbMemberOrder;						//��Ա��ʶ
	WORD							wPurchaseTime;						//����ʱ��

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�һ���Ϸ��
struct DBR_GP_ExchangeScoreByIngot
{
	DWORD							dwUserID;							//�û���ʶ
	SCORE							lExchangeIngot;						//�һ�����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�һ���Ϸ��
struct DBR_GP_ExchangeScoreByBeans
{
	DWORD							dwUserID;							//�û���ʶ
	double							dExchangeBeans;						//�һ�����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�ƹ���Ϣ
struct DBR_GP_QuerySpreadInfo
{
	DWORD							dwUserID;							//�û���ʶ
};

//////////////////////////////////////////////////////////////////////////////////
//�齱����

//��ʼ�齱
struct DBR_GP_LotteryConfigReq
{
	WORD							wKindID;							//��Ϸ��ʶ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szLogonPass[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//��ʼ�齱
struct DBR_GP_LotteryStart
{
	WORD							wKindID;							//��Ϸ��ʶ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szLogonPass[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ʼ�齱
struct DBR_GP_SimpleLotteryStart
{
	LONGLONG					    lLottery;							//�н�
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwIndex;
	char							szName[LEN_NICKNAME];				//�û��ǳ�

};

//////////////////////////////////////////////////////////////////////////////////
//��Ϸ����
struct DBR_GP_QueryUserGameData
{
	WORD							wKindID;							//��Ϸ��ʶ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szDynamicPass[LEN_MD5];				//�û�����	
};

//////////////////////////////////////////////////////////////////////////////////
//�����Ϣ

//��¼���
#define DBO_GP_LOGON_SUCCESS		100									//��¼�ɹ�
#define DBO_GP_LOGON_FAILURE		101									//��¼ʧ��
#define DBO_GP_VALIDATE_MBCARD		102									//��¼ʧ��
#define DBO_GP_VALIDATE_PASSPORT	103									//��¼ʧ��
#define DBO_GP_VERIFY_RESULT		104									//��֤���									

//�������
#define DBO_GP_USER_FACE_INFO		110									//�û�ͷ��
#define DBO_GP_USER_INDIVIDUAL		111									//�û�����

//��������
#define DBO_GP_USER_INSURE_INFO		120									//��������
#define DBO_GP_USER_INSURE_SUCCESS	121									//���гɹ�
#define DBO_GP_USER_INSURE_FAILURE	122									//����ʧ��
#define DBO_GP_USER_INSURE_USER_INFO 123								//�û�����
#define DBO_GP_USER_INSURE_ENABLE_RESULT 124							//��ͨ���
#define DBO_GP_USER_INGAME_SERVER_ID   125								//�û���Ϸ״̬

//�б���
#define DBO_GP_GAME_TYPE_ITEM		130									//������Ϣ
#define DBO_GP_GAME_KIND_ITEM		131									//������Ϣ
#define DBO_GP_GAME_NODE_ITEM		132									//�ڵ���Ϣ
#define DBO_GP_GAME_PAGE_ITEM		133									//������Ϣ
#define DBO_GP_GAME_LIST_RESULT		134									//���ؽ��

//ϵͳ���
#define DBO_GP_PLATFORM_PARAMETER	140									//ƽ̨����

//��������
#define DBO_GP_ANDROID_PARAMETER	150									//��������	

//ǩ������
#define DBO_GP_CHECKIN_REWARD		160									//ǩ������
#define DBO_GP_CHECKIN_INFO			161									//ǩ����Ϣ
#define DBO_GP_CHECKIN_RESULT		162									//ǩ�����

//��������
#define DBO_GP_TASK_LIST			170									//�����б�
#define DBO_GP_TASK_LIST_END		171									//�б����
#define DBO_GP_TASK_INFO			172									//������Ϣ
#define DBO_GP_TASK_RESULT			173									//������

//�ͱ�����
#define DBO_GP_BASEENSURE_PARAMETER	180									//�ͱ�����
#define DBO_GP_BASEENSURE_RESULT	181									//��ȡ���

//�ƹ�����
#define DBO_GP_SPREAD_INFO			190									//�ƹ����

//�ȼ�����
#define DBO_GP_GROWLEVEL_CONFIG		200									//�ȼ�����
#define DBO_GP_GROWLEVEL_PARAMETER	201									//�ȼ�����
#define DBO_GP_GROWLEVEL_UPGRADE	202									//�ȼ�����

//�һ�����
#define DBO_GP_MEMBER_PARAMETER     210									//��Ա����
#define DBO_GP_PURCHASE_RESULT		211									//������
#define DBO_GP_EXCHANGE_RESULT		212									//�һ����

//�������
#define DBO_GP_MATCH_SIGNUP_RESULT	220									//�������

//�齱���
#define DBO_GP_LOTTERY_CONFIG		230									//�齱����
#define DBO_GP_LOTTERY_USER_INFO	231									//�齱��Ϣ
#define DBO_GP_LOTTERY_RESULT		232									//�齱���

#define DBO_GP_SIMPLE_LOTTERY_RESULT		233									//�齱���

//�齱���
#define DBO_GP_QUERY_USER_GAME_DATA	250									//��Ϸ����

//������
#define DBO_GP_OPERATE_SUCCESS		800									//�����ɹ�
#define DBO_GP_OPERATE_FAILURE		801									//����ʧ��

//ϵͳ��Ϣ���
#define DBO_GP_SYSTEM_QQ			810									//ϵͳQQ:DCY by 2017.3.9
#define DBO_GP_SYSTEM_INFO			811									//ϵͳ����:DCY by 2017.3.9
#define DBO_GP_SYSTEM_CONTACT		812									//��ϵ����
#define DBO_GP_SYSTEM_SHOP			813									//��Ϸ�̳�:DCY by 2017.3.11
#define DBO_GP_EXCHANGE_SHOP		814									//�һ���Ʒ:DCY by 2017.3.11

//////////////////////////////////////////////////////////////////////////////////////////

//��¼�ɹ�
struct DBO_GP_LogonSuccess
{
	//��������
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwGameID;							//��Ϸ��ʶ
	DWORD							dwGroupID;							//���ű�ʶ
	DWORD							dwCustomID;							//�Զ�����
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����
	TCHAR							szPassword[LEN_MD5];				//��¼����	
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szDynamicPass[LEN_PASSWORD];		//��̬����
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������

	//�û��ɼ�
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserIngot;							//�û�Ԫ��
	SCORE							lUserInsure;						//�û�����	
	DOUBLE							dUserBeans;							//�û���Ϸ��

	//�û�����
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMoorMachine;						//��������
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��

	//��Ա����
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	SYSTEMTIME						MemberOverDate;						//����ʱ��

	//��չ��Ϣ
	BYTE							cbLogonMode;						//��½��ʽ
	BYTE							cbInsureEnabled;					//ʹ�ܱ�ʶ
	DWORD							dwCheckUserRight;					//���Ȩ��

	//������Ϣ
	WORD							wSignupCount;						//��������
	tagSignupMatchInfo				SignupMatchInfo[32];				//�����б�

	//������Ϣ
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��¼ʧ��
struct DBO_GP_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��¼ʧ��
struct DBO_GP_ValidateMBCard
{
	UINT							uMBCardID;						//��������
};

//��֤���
struct DBO_GP_VerifyIndividualResult
{
	bool							bVerifyPassage;						//��֤ͨ��
	WORD							wVerifyMask;						//��֤����
	TCHAR							szErrorMsg[128];					//������Ϣ
};

//ͷ����Ϣ
struct DBO_GP_UserFaceInfo
{
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwCustomID;							//�Զ�����
};

//��������
struct DBO_GP_UserIndividual
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szUserNote[LEN_USER_NOTE];			//�û�˵��
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

	//�绰����
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//�̶��绰
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ƶ��绰

	//��ϵ����
	TCHAR							szQQ[LEN_QQ];						//Q Q ����
	TCHAR							szEMail[LEN_EMAIL];					//�����ʼ�
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//��ϵ��ַ
};

//��������
struct DBO_GP_UserInsureInfo
{
	BYTE							cbEnjoinTransfer;					//ת�ʿ���
	WORD							wRevenueTake;						//˰�ձ���
	WORD							wRevenueTransfer;					//˰�ձ���
	WORD							wRevenueTransferMember;				//˰�ձ���
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserInsure;						//������Ϸ��
	SCORE							lTransferPrerequisite;				//ת������
};

//������Ϣ
struct DBO_GP_UserAccountInfo
{
	//��������
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwGameID;							//��Ϸ��ʶ
	DWORD							dwGroupID;							//���ű�ʶ
	DWORD							dwCustomID;							//�Զ�����
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����
	DWORD							dwSpreaderID;						//�ƹ�ID
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������
	TCHAR							szLogonIp[LEN_ACCOUNTS];			//��¼IP

	//�û��ɼ�
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserInsure;						//�û�����

	//�û�����
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMoorMachine;						//��������
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��

	//��Ա����
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	SYSTEMTIME						MemberOverDate;						//����ʱ��
};

//���гɹ�
struct DBO_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSourceScore;						//ԭ����Ϸ��
	SCORE							lSourceInsure;						//ԭ����Ϸ��
	SCORE							lInsureRevenue;						//����˰��
	SCORE							lVariationScore;					//��Ϸ�ұ仯
	SCORE							lVariationInsure;					//���б仯
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct  DBO_GP_UserInsureFailure
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��ͨ���
struct DBO_GP_UserInsureEnableResult
{
	BYTE							cbInsureEnabled;					//ʹ�ܱ�ʶ
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��������
struct DBO_GP_PlatformParameter
{
	DWORD							dwExchangeRate;						//�һ�����
	DWORD							dwPresentExchangeRate;				//������Ϸ�Ҷһ���
	DWORD							dwRateGold;							//��Ϸ����Ϸ�Ҷһ���
};

//��������
struct DBO_GP_AndroidParameter
{
	WORD							wSubCommdID;						//��������
	WORD							wServerID;							//�����ʶ
	WORD							wParameterCount;					//������Ŀ
	tagAndroidParameter				AndroidParameter[MAX_BATCH];		//��������
};

//ǩ������
struct DBO_GP_CheckInReward
{
	SCORE							lRewardGold[LEN_WEEK];				//�������
};

//ǩ����Ϣ
struct DBO_GP_CheckInInfo
{
	WORD							wSeriesDate;						//��������
	bool							bTodayChecked;						//ǩ����ʶ
};

//ǩ�����
struct DBO_GP_CheckInResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	SCORE							lScore;								//��ǰ����
	TCHAR							szNotifyContent[128];				//��ʾ����
};

//�����б�
struct DBO_GP_TaskListInfo
{
	WORD							wTaskCount;							//������Ŀ
};

//������Ϣ
struct DBO_GP_TaskInfo
{
	WORD							wTaskCount;							//��������
	tagTaskStatus					TaskStatus[TASK_MAX_COUNT];			//����״̬
};

//ǩ�����
struct DBO_GP_TaskResult
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

//�ͱ�����
struct DBO_GP_BaseEnsureParameter
{
	SCORE							lScoreCondition;					//��Ϸ������
	SCORE							lScoreAmount;						//��Ϸ������
	BYTE							cbTakeTimes;						//��ȡ����	
};

//�ͱ����
struct DBO_GP_BaseEnsureResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	SCORE							lGameScore;							//��ǰ��Ϸ��
	TCHAR							szNotifyContent[128];				//��ʾ����
};

//�ƹ���Ϣ
struct DBO_GP_UserSpreadInfo
{
	DWORD							dwSpreadCount;						//�ƹ�����
	SCORE							lSpreadReward;						//�ƹ㽱��
};

//�ȼ�����
struct DBO_GP_GrowLevelConfig
{
	WORD							wLevelCount;						//�ȼ���Ŀ
	tagGrowLevelConfig				GrowLevelConfig[60];				//�ȼ�����
};

//�ȼ�����
struct DBO_GP_GrowLevelParameter
{
	WORD							wCurrLevelID;						//��ǰ�ȼ�	
	DWORD							dwExperience;						//��ǰ����
	DWORD							dwUpgradeExperience;				//�¼�����
	SCORE							lUpgradeRewardGold;					//��������
	SCORE							lUpgradeRewardIngot;				//��������
};

//�ȼ�����
struct DBO_GP_GrowLevelUpgrade
{
	SCORE							lCurrScore;							//��ǰ��Ϸ��
	SCORE							lCurrIngot;							//��ǰԪ��
	TCHAR							szNotifyContent[128];				//������ʾ
};

//��Ա����
struct DBO_GP_MemberParameter
{
	WORD							wMemberCount;						//��Ա��Ŀ
	tagMemberParameter				MemberParameter[64];				//��Ա����
};

//������
struct DBO_GP_PurchaseResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	BYTE							cbMemberOrder;						//��Աϵ��
	SCORE							lCurrScore;							//��ǰ��Ϸ��
	DOUBLE							dCurrBeans;							//��ǰ��Ϸ��
	TCHAR							szNotifyContent[128];				//��ʾ����
};
 
//�һ����
struct DBO_GP_ExchangeResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	SCORE							lCurrScore;							//��ǰ��Ϸ��
	SCORE							lCurrIngot;							//��ǰԪ��
	DOUBLE							dCurrBeans;							//��ǰ��Ϸ��
	TCHAR							szNotifyContent[128];				//��ʾ����
};


//�������
struct DBO_GP_MatchSignupResult
{
	bool							bSignup;							//������ʶ
	bool							bSuccessed;							//�ɹ���ʶ
	WORD							wServerID;							//�����ʶ
	SCORE							lCurrScore;							//��ǰ���
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�齱����
struct DBO_GP_LotteryConfig
{
	WORD							wLotteryCount;						//�������
	tagLotteryItem					LotteryItem[MAX_LOTTERY];			//��������
};

//�齱��Ϣ
struct DBO_GP_LotteryUserInfo
{
	BYTE							cbFreeCount;						//��Ѵ���
	BYTE							cbAlreadyCount;						//�������
	WORD							wKindID;							//��Ϸ��ʶ
	DWORD							dwUserID;							//�û���ʶ
	SCORE							lChargeFee;							//�齱����
};

struct DBO_GP_SimpleLotteryResult
{
	DWORD							dwIndex;						
	DWORD							dwUserID;							//�û���ʶ
	SCORE							lUserScore;							//�û�����
	DWORD							lInsure;
	DWORD							lExp;
	char							szName[LEN_NICKNAME];
	DWORD							dwErrorCode;
};

//�齱���
struct DBO_GP_LotteryResult
{
	bool							bWined;								//�н���ʶ
	WORD							wKindID;							//��Ϸ��ʶ
	DWORD							dwUserID;							//�û���ʶ
	SCORE							lUserScore;							//�û�����
	DOUBLE							dUserBeans;							//�û���Ϸ��
	tagLotteryItem					LotteryItem;						//�н�����
};

////�齱���
//struct DBO_GP_SimpleLotteryResult
//{
//	DWORD							dwIndex;							
//	DWORD							dwUserID;							//�û���ʶ
//	SCORE							lUserScore;							//�û�����
//
//};

//��Ϸ����
struct DBO_GP_QueryUserGameData
{
	WORD							wKindID;							//��Ϸ��ʶ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szUserGameData[1024];				//��Ϸ����
};

//����ʧ��
struct DBO_GP_OperateFailure
{
	bool							bCloseSocket;						//�ر�����
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�����ɹ�
struct DBO_GP_OperateSuccess
{
	bool							bCloseSocket;						//�ر�����
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//�ɹ���Ϣ
};

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
struct DBO_GP_GameType
{
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wTypeID;							//��������
	TCHAR							szTypeName[LEN_TYPE];				//��������
};

//��Ϸ����
struct DBO_GP_GameKind
{
	WORD							wTypeID;							//��������
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wKindID;							//��������
	WORD							wGameID;							//ģ������
	BYTE							cbSuportMobile;						//֧���ֻ�
	TCHAR							szKindName[LEN_KIND];				//��Ϸ����
	TCHAR							szProcessName[LEN_PROCESS];			//��������
};

//��Ϸ�ڵ�
struct DBO_GP_GameNode
{
	WORD							wKindID;							//��������
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wNodeID;							//�ڵ�����
	TCHAR							szNodeName[LEN_NODE];				//�ڵ�����
};

//��������
struct DBO_GP_GamePage
{
	WORD							wKindID;							//��������
	WORD							wNodeID;							//�ڵ�����
	WORD							wSortID;							//��������
	WORD							wPageID;							//��������
	WORD							wOperateType;						//��������
	TCHAR							szDisplayName[LEN_PAGE];			//��ʾ����
};

//���ؽ��
struct DBO_GP_GameListResult
{
	BYTE							cbSuccess;							//�ɹ���־
};

//��ѯ���
struct DBO_GP_UserInGameServerID
{
	DWORD							LockKindID;			
	DWORD							LockServerID;	
};

//ϵͳQQ���
struct DBO_GP_SystemQQ
{
	TCHAR							szSystemQQ[0xFF];				//ϵͳ����
};

//ϵͳ������
struct DBO_GP_SystemInfo
{
	TCHAR							szSystemInfo[0xFF];					//ϵͳ����
};

//ϵͳ������
struct DBO_GP_SystemContact
{
	TCHAR							szSystemContact[0xFF];				//ϵͳ����
};


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//�ֻ����ݰ�

//��¼����
#define	DBR_MB_LOGON_GAMEID			700									//I D ��¼
#define	DBR_MB_LOGON_ACCOUNTS		701									//�ʺŵ�¼
#define DBR_MB_REGISTER_ACCOUNTS	702									//ע���ʺ�
#define	DBR_MB_LOGON_OTHERPLATFORM	703									//������¼
#define	DBR_MB_LOGON_VISITOR		704									//�ο͵�¼


//��¼���
#define DBO_MB_LOGON_SUCCESS		900									//��¼�ɹ�
#define DBO_MB_LOGON_FAILURE		901									//��¼ʧ��

//////////////////////////////////////////////////////////////////////////////////

//ID ��¼
struct DBR_MB_LogonGameID
{
	//��¼��Ϣ
	DWORD							dwGameID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺŵ�¼
struct DBR_MB_LogonAccounts
{
	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺŵ�¼
struct DBR_MB_LogonOtherPlatform
{
	//��¼��Ϣ
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbPlatformID;						//ƽ̨���
	TCHAR							szUserUin[LEN_USER_UIN];			//�û�Uin
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ο͵�¼
struct DBR_MB_LogonVisitor
{
	//��¼��Ϣ
	BYTE							cbPlatformID;						//ƽ̨���

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺ�ע��
struct DBR_MB_RegisterAccounts
{
	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//��¼�ɹ�
struct DBO_MB_LogonSuccess
{
	//�û�����
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwCustomID;							//�Զ�ͷ��
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//�û��ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szDynamicPass[LEN_PASSWORD];		//��̬����

	//�û��ɼ�
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserIngot;							//�û�Ԫ��
	SCORE							lUserInsure;						//�û�����	
	DOUBLE							dUserBeans;							//�û���Ϸ��

	//��չ��Ϣ
	BYTE							cbInsureEnabled;					//ʹ�ܱ�ʶ
	BYTE							cbLogonMode;						//��½��ʽ

	//������Ϣ
	WORD							wSignupCount;						//��������
	tagSignupMatchInfo				SignupMatchInfo[32];				//�����б�

	//������Ϣ
	TCHAR							szDescribeString[128];				//������Ϣ
};


//��¼ʧ��
struct DBO_MB_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//////////////////////////////////////////////////////////////////////////////////

#endif