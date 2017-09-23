#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

#include "..\..\ȫ�ֶ���\datastream.h"
#include <vector>
#include <map>

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//��¼����

#define MDM_GP_LOGON				1									//�㳡��¼

//��¼ģʽ
#define SUB_GP_LOGON_GAMEID			1									//I D ��¼
#define SUB_GP_LOGON_ACCOUNTS		2									//�ʺŵ�¼
#define SUB_GP_REGISTER_ACCOUNTS	3									//ע���ʺ�
#define SUB_GP_LOGON_MANAGETOOL		4									//������
#define SUB_GP_VERIFY_INDIVIDUAL	5									//��֤����
#define SUB_GP_LOGON_VISITOR		6									//�ο͵�¼
#define SUB_GP_QUERY_VERIFY_CODE	7									//��ѯ��֤��

//��¼���
#define SUB_GP_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_GP_LOGON_FAILURE		101									//��¼ʧ��
#define SUB_GP_LOGON_FINISH			102									//��¼���
#define SUB_GP_VALIDATE_MBCARD      103                                 //��¼ʧ��
#define SUB_GP_VALIDATE_PASSPORT	104									//��¼ʧ��	
#define SUB_GP_VERIFY_RESULT		105									//��֤���
#define SUB_GP_MATCH_SIGNUPINFO		106									//������Ϣ
#define SUB_GP_GROWLEVEL_CONFIG		107									//�ȼ�����
#define SUB_GP_VERIFY_CODE_RESULT	108									//��֤���

//������ʾ
#define SUB_GP_UPDATE_NOTIFY		200									//������ʾ

//////////////////////////////////////////////////////////////////////////////////
//
#define MB_VALIDATE_FLAGS           0x01                                //Ч���ܱ�
#define LOW_VER_VALIDATE_FLAGS      0x02                                //Ч��Ͱ汾

//У������
#define VERIFY_ACCOUNTS				0x01								//У���˺�
#define VERIFY_NICKNAME				0x02								//У���ǳ�

//I D ��¼
struct CMD_GP_LogonGameID
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//��¼��Ϣ
	DWORD							dwGameID;							//��Ϸ I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	BYTE							cbValidateFlags;			        //У���ʶ
};

//�ʺŵ�¼
struct CMD_GP_LogonAccounts
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//��¼��Ϣ
	BYTE							cbValidateFlags;			        //У���ʶ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//���֤��
};

//ע���ʺ�
struct CMD_GP_RegisterAccounts
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����

	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szSpreader[LEN_ACCOUNTS];			//�Ƽ��ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����
	TCHAR							szVerifyCode[LEN_VERIFY_CODE];		//��֤��
	BYTE							cbValidateFlags;			        //У���ʶ
};

//��֤����
struct CMD_GP_VerifyIndividual
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾

	//��֤��Ϣ
	WORD							wVerifyMask;						//��֤����
};

//�ο͵�¼
struct CMD_GP_LogonVisitor
{
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE							cbValidateFlags;			        //У���ʶ

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//��¼�ɹ�
struct CMD_GP_LogonSuccess
{
	//��������
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwGroupID;							//���ű�ʶ
	DWORD							dwCustomID;							//�Զ���ʶ	
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����

	//�û��ɼ�
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//�û�����
	SCORE							lUserIngot;							//�û�Ԫ��
	DOUBLE							dUserBeans;							//�û���Ϸ��

	//�û���Ϣ
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMoorMachine;						//��������
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	TCHAR							szDynamicPass[LEN_PASSWORD];		//��̬����
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������
	TCHAR							szHeadUrl[LEN_URL];					//ͷ���ַ

	//������Ϣ
	BYTE							cbInsureEnabled;					//����ʹ�ܱ�ʶ
	BYTE                            cbShowServerStatus;                 //��ʾ������״̬

	DWORD							dwAddress;

};

//��¼ʧ��
struct CMD_GP_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��½���
struct CMD_GP_LogonFinish
{
	WORD							wIntermitTime;						//�ж�ʱ��
	WORD							wOnLineCountTime;					//����ʱ��
};

//��¼ʧ��
struct CMD_GP_ValidateMBCard
{
	UINT								uMBCardID;						//��������
};

//��֤���
struct CMD_GP_VerifyIndividualResult
{
	bool							bVerifyPassage;						//��֤ͨ��
	WORD							wVerifyMask;						//��֤����
	TCHAR							szErrorMsg[128];					//������Ϣ
};

//������ʾ
struct CMD_GP_UpdateNotify
{
	BYTE							cbMustUpdate;						//ǿ������
	BYTE							cbAdviceUpdate;						//��������
	DWORD							dwCurrentVersion;					//��ǰ�汾
};

//��ѯ��֤��
struct CMD_GP_QueryVerifyCode
{
	bool								bCloseSocket;					//������ʶ
	TCHAR								szMachineID[LEN_MACHINE_ID];	//������ (C2S)
	TCHAR								szVerifyCode[LEN_VERIFY_CODE];	//��֤�� (S2C)
};

//��֤���
struct CMD_GP_VerifyCodeResult
{
	BYTE								cbResultCode;					//�����ʶ��1�ɹ���0ʧ�ܣ�2�ظ����룩
	TCHAR								szDescString[64];				//����ַ���
};

//////////////////////////////////////////////////////////////////////////////////
//Я����Ϣ CMD_GP_LogonSuccess

#define DTP_GP_GROUP_INFO			1									//������Ϣ
#define DTP_GP_MEMBER_INFO			2									//��Ա��Ϣ
#define	DTP_GP_UNDER_WRITE			3									//����ǩ��
#define DTP_GP_STATION_URL			4									//��ҳ��Ϣ

//������Ϣ
struct DTP_GP_GroupInfo
{
	DWORD							dwGroupID;							//��������
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������
};

//��Ա��Ϣ
struct DTP_GP_MemberInfo
{
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	SYSTEMTIME						MemberOverDate;						//����ʱ��
};

//////////////////////////////////////////////////////////////////////////////////
//�б�����

#define MDM_GP_SERVER_LIST			2									//�б���Ϣ

//��ȡ����
#define SUB_GP_GET_LIST				1									//��ȡ�б�
#define SUB_GP_GET_SERVER			2									//��ȡ����
#define SUB_GP_GET_MATCH			3									//��ȡ����
#define SUB_GP_GET_ONLINE			4									//��ȡ����
#define SUB_GP_GET_COLLECTION		5									//��ȡ�ղ�

//�б���Ϣ
#define SUB_GP_LIST_TYPE			100									//�����б�
#define SUB_GP_LIST_KIND			101									//�����б�
#define SUB_GP_LIST_NODE			102									//�ڵ��б�
#define SUB_GP_LIST_PAGE			103									//�����б�
#define SUB_GP_LIST_SERVER			104									//�����б�
#define SUB_GP_LIST_MATCH			105									//�����б�
#define SUB_GP_VIDEO_OPTION			106									//��Ƶ����

//�����Ϣ
#define SUB_GP_LIST_FINISH			200									//�������
#define SUB_GP_SERVER_FINISH		201									//�������
#define SUB_GP_MATCH_FINISH			202									//�������

//������Ϣ
#define SUB_GR_KINE_ONLINE			300									//��������
#define SUB_GR_SERVER_ONLINE		301									//��������

//////////////////////////////////////////////////////////////////////////////////

//��ȡ����
struct CMD_GP_GetOnline
{
	WORD							wServerCount;						//������Ŀ
	WORD							wOnLineServerID[MAX_SERVER];		//�����ʶ
};

//��������
struct CMD_GP_KindOnline
{
	WORD							wKindCount;							//������Ŀ
	tagOnLineInfoKind				OnLineInfoKind[MAX_KIND];			//��������
};

//��������
struct CMD_GP_ServerOnline
{
	WORD							wServerCount;						//������Ŀ
	tagOnLineInfoServer				OnLineInfoServer[MAX_SERVER];		//��������
};

//////////////////////////////////////////////////////////////////////////////////
//��������

#define MDM_GP_USER_SERVICE				3								//�û�����

//�˺ŷ���
#define SUB_GP_MODIFY_MACHINE			100								//�޸Ļ���
#define SUB_GP_MODIFY_LOGON_PASS		101								//�޸�����
#define SUB_GP_MODIFY_INSURE_PASS		102								//�޸�����
#define SUB_GP_MODIFY_UNDER_WRITE		103								//�޸�ǩ��
#define SUB_GP_USER_CHOUJIANG			104								//�û��齱
#define SUB_GP_QUERY_ROOM_LIST			105								//�����б�
#define SUB_GP_QUERY_ACCOUNTINFO		304								//������Ϣ


////////////////////2017-02-20 ��//////////////////////////////
//�Զ������
#define SUB_GP_QUERY_INGAME_SEVERID		110								//��Ϸ״̬
#define SUB_GP_GAME_RECORD_LIST			111								//��Ϸ��¼
#define SUB_GP_GAME_RECORD_TOTAL		112								//��Ϸ��¼
///////////////////////////////////////////////////////////////

//�޸�ͷ��
#define SUB_GP_USER_FACE_INFO			120								//ͷ����Ϣ
#define SUB_GP_SYSTEM_FACE_INFO			122								//ϵͳͷ��
#define SUB_GP_CUSTOM_FACE_INFO			123								//�Զ�ͷ��

//��������
#define SUB_GP_USER_INDIVIDUAL			140								//��������
#define	SUB_GP_QUERY_INDIVIDUAL			141								//��ѯ��Ϣ
#define SUB_GP_MODIFY_INDIVIDUAL		152								//�޸�����

//���з���
#define SUB_GP_USER_ENABLE_INSURE		160								//��ͨ����
#define SUB_GP_USER_SAVE_SCORE			161								//������
#define SUB_GP_USER_TAKE_SCORE			162								//ȡ�����
#define SUB_GP_USER_TRANSFER_SCORE		163								//ת�˲���
#define SUB_GP_USER_INSURE_INFO			164								//��������
#define SUB_GP_QUERY_INSURE_INFO		165								//��ѯ����
#define SUB_GP_USER_INSURE_SUCCESS		166								//���гɹ�
#define SUB_GP_USER_INSURE_FAILURE		167								//����ʧ��
#define SUB_GP_QUERY_USER_INFO_REQUEST	168								//��ѯ�û�
#define SUB_GP_QUERY_USER_INFO_RESULT	169								//�û���Ϣ
#define SUB_GP_USER_INSURE_ENABLE_RESULT 170							//��ͨ���	

//��������
#define SUB_GP_MATCH_SIGNUP				200								//��������
#define SUB_GP_MATCH_UNSIGNUP			201								//ȡ������
#define SUB_GP_MATCH_SIGNUP_RESULT		202								//�������

//ǩ������
#define SUB_GP_CHECKIN_QUERY			220								//��ѯǩ��
#define SUB_GP_CHECKIN_INFO				221								//ǩ����Ϣ
#define SUB_GP_CHECKIN_DONE				222								//ִ��ǩ��
#define SUB_GP_CHECKIN_RESULT			223								//ǩ�����

//�������
#define SUB_GP_TASK_LOAD				240								//��������
#define SUB_GP_TASK_TAKE				241								//��ȡ����
#define SUB_GP_TASK_REWARD				242								//������
#define SUB_GP_TASK_INFO				243								//������Ϣ
#define SUB_GP_TASK_LIST				244								//������Ϣ
#define SUB_GP_TASK_RESULT				245								//������

//�ͱ�����
#define SUB_GP_BASEENSURE_LOAD			260								//���صͱ�
#define SUB_GP_BASEENSURE_TAKE			261								//��ȡ�ͱ�
#define SUB_GP_BASEENSURE_PARAMETER		262								//�ͱ�����
#define SUB_GP_BASEENSURE_RESULT		263								//�ͱ����

//�ƹ����
#define SUB_GP_SPREAD_QUERY				280								//�ƹ㽱��
#define SUB_GP_SPREAD_INFO				281								//��������

//�ȼ�����
#define SUB_GP_GROWLEVEL_QUERY			300								//��ѯ�ȼ�
#define SUB_GP_GROWLEVEL_PARAMETER		301								//�ȼ�����
#define SUB_GP_GROWLEVEL_UPGRADE		302								//�ȼ�����

//�һ�����
#define SUB_GP_EXCHANGE_QUERY			320								//�һ�����
#define SUB_GP_EXCHANGE_PARAMETER		321								//�һ�����
#define SUB_GP_PURCHASE_MEMBER			322								//�����Ա
#define SUB_GP_PURCHASE_RESULT			323								//������
#define SUB_GP_EXCHANGE_SCORE_BYINGOT	324								//�һ���Ϸ��
#define SUB_GP_EXCHANGE_SCORE_BYBEANS	325								//�һ���Ϸ��
#define SUB_GP_EXCHANGE_RESULT			326								//�һ����

//�齱����
#define SUB_GP_LOTTERY_CONFIG_REQ		340								//��������
#define SUB_GP_LOTTERY_CONFIG			341								//�齱����
#define SUB_GP_LOTTERY_USER_INFO		342								//�齱��Ϣ
#define SUB_GP_LOTTERY_START			343								//��ʼ�齱
#define SUB_GP_LOTTERY_RESULT			344								//�齱���

#define SUB_GP_SIMPLE_LOTERY			345								//�򵥳齱
#define SUB_GP_SIMPLE_LOTTERY_RESULT	346								//�齱���
#define SUB_GP_LOTTERY_NOTI				347								//�齱֪ͨ

//��Ϸ����
#define SUB_GP_QUERY_USER_GAME_DATA		360								//��ѯ����	

//��Ϣ��ȡ
#define SUB_GP_QUERY_SYSTEMQQ			370								//��Ϸ�ͷ�:DCY by 2017.3.9
#define SUB_GP_QUERY_SYSTEMINFO			371								//ϵͳ����:DCY by 2017.3.9
#define SUB_GP_QUERY_SYSTEMCONTACT		372								//��ϵ����:DCY by 2017.3.9
#define SUB_GP_QUERY_SHOP				373								//��Ϸ�̳�:DCY by 2017.3.11
#define SUB_GP_EXCHANGE_SHOP			374								//�һ���Ʒ:DCY by 2017.3.11

//�������
#define SUB_GP_OPERATE_SUCCESS			500								//�����ɹ�
#define SUB_GP_OPERATE_FAILURE			501								//����ʧ��

//ϵͳ��ϵ

//////////////////////////////////////////////////////////////////////////////////

//�޸�����
struct CMD_GP_ModifyLogonPass
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�����
struct CMD_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�޸�ǩ��
struct CMD_GP_ModifyUnderWrite
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��
};

//////////////////////////////////////////////////////////////////////////////////

//�û�ͷ��
struct CMD_GP_UserFaceInfo
{
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwCustomID;							//�Զ���ʶ
};

//�޸�ͷ��
struct CMD_GP_SystemFaceInfo
{
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�ͷ��
struct CMD_GP_CustomFaceInfo
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//ͼƬ��Ϣ
};

//////////////////////////////////////////////////////////////////////////////////

//�󶨻���
struct CMD_GP_ModifyMachine
{
	BYTE							cbBind;								//�󶨱�־
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�����б�
struct CMD_GP_QueryRoomList
{
	DWORD							dwUserID;							//�û� I D
};


//////////////////////////////////////////////////////////////////////////////////

//��������
struct CMD_GP_UserIndividual
{
	DWORD							dwUserID;							//�û� I D
};

//��ѯ��Ϣ
struct CMD_GP_QueryIndividual
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
};

//��Ϸ״̬
struct CMD_GP_UserInGameServerID
{
	DWORD							dwUserID;							//�û� I D
};

//�޸�����
struct CMD_GP_ModifyIndividual
{
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
};

//��ѯQQ
struct CMD_GP_QuerySystemQQ
{
	TCHAR							szSystemQQ[0xFF];					//��Ϸ�ͷ�
};

//��ѯ����
struct CMD_GP_QuerySystemInfo
{
	TCHAR							szSystemInfo[0xFF];					//ϵͳ����
};

//��ѯ����
struct CMD_GP_QuerySystemContact
{
	TCHAR							szSystemContact[0xFF];				//��ϵ����
};

//������Ʒ
struct CMD_GS_ExchangeShop
{
	DWORD							dwUserID;							//�û����
	DWORD							dwItemID;							//��Ʒ���
	DWORD							dwCount;							//�һ�����
	SCORE							llNumber;							//�绰����
	TCHAR							strName[32];						//�ռ�����
	TCHAR							strAddress[0x7F];					//�ջ���ַ
};
//////////////////////////////////////////////////////////////////////////////////
//Я����Ϣ CMD_GP_UserIndividual
#define DTP_GP_UI_ACCOUNTS			1									//�û��˺�	
#define DTP_GP_UI_NICKNAME			2									//�û��ǳ�
#define DTP_GP_UI_USER_NOTE			3									//�û�˵��
#define DTP_GP_UI_UNDER_WRITE		4									//����ǩ��
#define DTP_GP_UI_QQ				5									//Q Q ����
#define DTP_GP_UI_EMAIL				6									//�����ʼ�
#define DTP_GP_UI_SEAT_PHONE		7									//�̶��绰
#define DTP_GP_UI_MOBILE_PHONE		8									//�ƶ��绰
#define DTP_GP_UI_COMPELLATION		9									//��ʵ����
#define DTP_GP_UI_DWELLING_PLACE	10									//��ϵ��ַ

//////////////////////////////////////////////////////////////////////////////////

//��������
struct CMD_GP_UserInsureInfo
{
	BYTE							cbEnjoinTransfer;					//ת�˿���
	WORD							wRevenueTake;						//˰�ձ���
	WORD							wRevenueTransfer;					//˰�ձ���
	WORD							wRevenueTransferMember;				//˰�ձ���
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
	SCORE							lTransferPrerequisite;				//ת������
};

//��ͨ����
struct CMD_GP_UserEnableInsure
{
	DWORD							dwUserID;							//�û�I D
	TCHAR							szLogonPass[LEN_PASSWORD];			//��¼����
	TCHAR							szInsurePass[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//������
struct CMD_GP_UserSaveScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSaveScore;							//������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ȡ���
struct CMD_GP_UserTakeScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lTakeScore;							//��ȡ���
	TCHAR							szPassword[LEN_MD5];				//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ת�˽��
struct CMD_GP_UserTransferScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lTransferScore;						//ת�˽��
	TCHAR							szPassword[LEN_MD5];				//��������
	TCHAR							szAccounts[LEN_NICKNAME];			//Ŀ���û�
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szTransRemark[LEN_TRANS_REMARK];	//ת�˱�ע
};

//���гɹ�
struct CMD_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct CMD_GP_UserInsureFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��ȡ���
struct CMD_GP_UserTakeResult
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
};

//��ѯ����
struct CMD_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��������
};

//��ѯ�û�
struct CMD_GP_QueryUserInfoRequest
{
	BYTE                            cbByNickName;                       //�ǳ�����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//Ŀ���û�
};

//�û���Ϣ
struct CMD_GP_UserTransferUserInfo
{
	DWORD							dwTargetGameID;						//Ŀ���û�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//Ŀ���û�
};

//��ͨ���
struct CMD_GP_UserInsureEnableResult
{
	BYTE							cbInsureEnabled;					//ʹ�ܱ�ʶ
	TCHAR							szDescribeString[128];				//������Ϣ
};

//////////////////////////////////////////////////////////////////////////////////
//��ѯǩ��
struct CMD_GP_CheckInQueryInfo
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
};

//ǩ����Ϣ
struct CMD_GP_CheckInInfo
{	
	WORD							wSeriesDate;						//��������
	bool							bTodayChecked;						//ǩ����ʶ
	SCORE							lRewardGold[LEN_WEEK];				//�������	
};

//ִ��ǩ��
struct CMD_GP_CheckInDone
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ǩ�����
struct CMD_GP_CheckInResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	SCORE							lScore;								//��ǰ���
	TCHAR							szNotifyContent[128];				//��ʾ����
};

//////////////////////////////////////////////////////////////////////////////////
//�������

//��������
struct CMD_GP_TaskLoadInfo
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
};

//��ȡ����
struct CMD_GP_TaskTake
{
	WORD							wTaskID;							//�����ʶ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ȡ����
struct CMD_GP_TaskReward
{
	WORD							wTaskID;							//�����ʶ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//������Ϣ
struct CMD_GP_TaskInfo
{
	WORD							wTaskCount;							//��������
	tagTaskStatus					TaskStatus[TASK_MAX_COUNT];			//����״̬
};

//������
struct CMD_GP_TaskResult
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
//�ͱ�����

//��ȡ�ͱ�
struct CMD_GP_BaseEnsureTake
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�ͱ�����
struct CMD_GP_BaseEnsureParamter
{
	SCORE							lScoreCondition;					//��Ϸ������
	SCORE							lScoreAmount;						//��Ϸ������
	BYTE							cbTakeTimes;						//��ȡ����	
};

//�ͱ����
struct CMD_GP_BaseEnsureResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	SCORE							lGameScore;							//��ǰ��Ϸ��
	TCHAR							szNotifyContent[128];				//��ʾ����
};

//////////////////////////////////////////////////////////////////////////////////
//�ƹ����

//�ƹ��ѯ
struct CMD_GP_UserSpreadQuery
{
	DWORD							dwUserID;							//�û���ʶ
};

//�ƹ����
struct CMD_GP_UserSpreadInfo
{
	DWORD							dwSpreadCount;						//�ƹ�����
	SCORE							lSpreadReward;						//�ƹ㽱��
};

//////////////////////////////////////////////////////////////////////////////////
//�ȼ�����

//��ѯ�ȼ�
struct CMD_GP_GrowLevelQueryInfo
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�ȼ�����
struct CMD_GP_GrowLevelConfig
{
	WORD							wLevelCount;						//�ȼ���Ŀ
	tagGrowLevelConfig				GrowLevelItem[60];					//�ȼ�����
};

//�ȼ�����
struct CMD_GP_GrowLevelParameter
{
	WORD							wCurrLevelID;						//��ǰ�ȼ�
	DWORD							dwExperience;						//��ǰ����
	DWORD							dwUpgradeExperience;				//�¼�����
	SCORE							lUpgradeRewardGold;					//��������
	SCORE							lUpgradeRewardIngot;				//��������
};

//�ȼ�����
struct CMD_GP_GrowLevelUpgrade
{
	SCORE							lCurrScore;							//��ǰ��Ϸ��
	SCORE							lCurrIngot;							//��ǰԪ��
	TCHAR							szNotifyContent[128];				//��ʾ����
};

//////////////////////////////////////////////////////////////////////////////////
//�һ�����

//��ѯ����
struct CMD_GP_ExchangeParameter
{
	DWORD							dwExchangeRate;						//Ԫ����Ϸ�Ҷһ�����
	DWORD							dwPresentExchangeRate;				//������Ϸ�Ҷһ���
	DWORD							dwRateGold;							//��Ϸ����Ϸ�Ҷһ���
	WORD							wMemberCount;						//��Ա��Ŀ
	tagMemberParameter				MemberParameter[10];				//��Ա����
};

//�����Ա
struct CMD_GP_PurchaseMember
{
	DWORD							dwUserID;							//�û���ʶ
	BYTE							cbMemberOrder;						//��Ա��ʶ
	WORD							wPurchaseTime;						//����ʱ��
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//������
struct CMD_GP_PurchaseResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	BYTE							cbMemberOrder;						//��Աϵ��
	SCORE							lCurrScore;							//��ǰ��Ϸ��
	DOUBLE							dCurrBeans;							//��ǰ��Ϸ��
	TCHAR							szNotifyContent[128];				//��ʾ����
};

//�һ���Ϸ��
struct CMD_GP_ExchangeScoreByIngot
{
	DWORD							dwUserID;							//�û���ʶ
	SCORE							lExchangeIngot;						//Ԫ������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//�һ���Ϸ��
struct CMD_GP_ExchangeScoreByBeans
{
	DWORD							dwUserID;							//�û���ʶ
	double							dExchangeBeans;						//��Ϸ������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//�һ����
struct CMD_GP_ExchangeResult
{
	bool							bSuccessed;							//�ɹ���ʶ
	SCORE							lCurrScore;							//��ǰ��Ϸ��
	SCORE							lCurrIngot;							//��ǰԪ��
	double							dCurrBeans;							//��ǰ��Ϸ��
	TCHAR							szNotifyContent[128];				//��ʾ����
};

//////////////////////////////////////////////////////////////////////////////////
//�齱����

//��������
struct CMD_GP_LotteryConfigReq
{
	WORD							wKindID;							//��Ϸ��ʶ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
};

//�齱����
struct CMD_GP_LotteryConfig
{
	WORD							wLotteryCount;						//�������
	tagLotteryItem					LotteryItem[MAX_LOTTERY];			//��������
};

//�齱��Ϣ
struct CMD_GP_LotteryUserInfo
{
	BYTE							cbFreeCount;						//��Ѵ���
	BYTE							cbAlreadyCount;						//�������
	WORD							wKindID;							//��Ϸ��ʶ
	DWORD							dwUserID;							//�û���ʶ
	SCORE							lChargeFee;							//�齱����
};

//��ʼ�齱
struct CMD_GP_LotteryStart
{
	WORD							wKindID;							//��Ϸ��ʶ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//֪ͨ��Ϣ
struct CMD_GP_Noti
{
	WORD							wNotiID;
	char							szStr[512];
};

struct CMD_GP_SimpleLotteryStart
{
	DWORD							dwUserID;							//�û���ʶ
	char							szName[LEN_NICKNAME];				//�û��ǳ�
};

struct CMD_GP_SimpleLotteryResult
{
	LONGLONG					    lLottery;							//�н�
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwIndex;					
	LONGLONG						lInsure;
	LONGLONG						lExp;
	DWORD							dwErrorCode;
};

//������Ϣ
struct CMD_GP_UserAccountInfo
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

//�齱���
struct CMD_GP_LotteryResult
{
	bool							bWined;								//�н���ʶ
	WORD							wKindID;							//��Ϸ��ʶ
	DWORD							dwUserID;							//�û���ʶ
	SCORE							lUserScore;							//�û�����
	DOUBLE							dUserBeans;							//�û���Ϸ��
	tagLotteryItem					LotteryItem;						//�н�����
};

//////////////////////////////////////////////////////////////////////////////////
//��Ϸ����
struct CMD_GP_QueryUserGameData
{
	WORD							wKindID;							//��Ϸ��ʶ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szDynamicPass[LEN_MD5];				//�û�����	
};

//�����ɹ�
struct CMD_GP_InGameSeverID
{
	DWORD							LockKindID;			
	DWORD							LockServerID;	
};

//������Ϣ
#define DTP_GP_UI_USER_GAME_DATA	1									//��Ϸ����	

//////////////////////////////////////////////////////////////////////////////////
//��������
struct CMD_GP_MatchSignup
{
	//������Ϣ
	WORD							wServerID;							//�����ʶ
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������

	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ȡ������
struct CMD_GP_MatchUnSignup
{
	//������Ϣ
	WORD							wServerID;							//�����ʶ
	DWORD							dwMatchID;							//������ʶ
	DWORD							dwMatchNO;							//��������

	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�������
struct CMD_GP_MatchSignupResult
{
	bool							bSignup;							//������ʶ
	bool							bSuccessed;							//�ɹ���ʶ
	WORD							wServerID;							//�����ʶ
	SCORE							lCurrScore;							//��ǰ���
	TCHAR							szDescribeString[128];				//������Ϣ
};

//////////////////////////////////////////////////////////////////////////////////

//����ʧ��
struct CMD_GP_OperateFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�����ɹ�
struct CMD_GP_OperateSuccess
{
	LONG							lResultCode;						//��������
	TCHAR							szDescribeString[128];				//�ɹ���Ϣ
};


//////////////////////////////////////////////////////////////////////////////////
//Զ�̷���

#define MDM_GP_REMOTE_SERVICE		4									//Զ�̷���

//���ҷ���
#define SUB_GP_C_SEARCH_DATABASE	100									//���ݲ���
#define SUB_GP_C_SEARCH_CORRESPOND	101									//Э������

//���ҷ���
#define SUB_GP_S_SEARCH_DATABASE	200									//���ݲ���
#define SUB_GP_S_SEARCH_CORRESPOND	201									//Э������

//////////////////////////////////////////////////////////////////////////////////

//Э������
struct CMD_GP_C_SearchCorrespond
{
	DWORD							dwGameID;							//��Ϸ��ʶ
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
};

//Э������
struct CMD_GP_S_SearchCorrespond
{
	WORD							wUserCount;							//�û���Ŀ
	tagUserRemoteInfo				UserRemoteInfo[16];					//�û���Ϣ
};

//////////////////////////////////////////////////////////////////////////////////
//��������

#define MDM_GP_ANDROID_SERVICE		5									//��������

//��������
#define SUB_GP_GET_PARAMETER		100									//��ȡ����
#define SUB_GP_ADD_PARAMETER		101									//��Ӳ���
#define SUB_GP_MODIFY_PARAMETER	102									//�޸Ĳ���
#define SUB_GP_DELETE_PARAMETER	103									//ɾ������

//������Ϣ
#define SUB_GP_ANDROID_PARAMETER  200									//��������		

//////////////////////////////////////////////////////////////////////////////////
//��ȡ����
struct CMD_GP_GetParameter
{
	WORD							wServerID;							//�����ʶ
};

//��Ӳ���
struct CMD_GP_AddParameter
{
	WORD							wServerID;							//�����ʶ
	tagAndroidParameter				AndroidParameter;					//��������
};

//�޸Ĳ���
struct CMD_GP_ModifyParameter
{
	WORD							wServerID;							//�����ʶ
	tagAndroidParameter				AndroidParameter;					//��������
};

//ɾ������
struct CMD_GP_DeleteParameter
{
	WORD							wServerID;							//�����ʶ
	DWORD							dwBatchID;							//���α�ʶ
};

//////////////////////////////////////////////////////////////////////////////////
//��������
struct CMD_GP_AndroidParameter
{
	WORD							wSubCommdID;						//��������
	WORD							wParameterCount;					//������Ŀ
	tagAndroidParameter				AndroidParameter[MAX_BATCH];		//��������
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//��¼����
#define MDM_MB_LOGON				100									//�㳡��¼

//��¼ģʽ
#define SUB_MB_LOGON_GAMEID			1									//I D ��¼
#define SUB_MB_LOGON_ACCOUNTS		2									//�ʺŵ�¼
#define SUB_MB_REGISTER_ACCOUNTS	3									//ע���ʺ�
#define SUB_MB_LOGON_OTHERPLATFORM	4									//������¼
#define SUB_MB_LOGON_VISITOR		5									//�ο͵�¼

//��¼���
#define SUB_MB_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_MB_LOGON_FAILURE		101									//��¼ʧ��
#define SUB_MB_MATCH_SIGNUPINFO		102									//������Ϣ

//������ʾ
#define SUB_MB_UPDATE_NOTIFY		200									//������ʾ

//////////////////////////////////////////////////////////////////////////////////

//I D ��¼
struct CMD_MB_LogonGameID
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

	//��¼��Ϣ
	DWORD							dwGameID;							//��Ϸ I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//�ʺŵ�¼
struct CMD_MB_LogonAccounts
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};


//�ʺŵ�¼
struct CMD_MB_LogonOtherPlatform
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

	//��¼��Ϣ
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbPlatformID;						//ƽ̨���
	TCHAR							szUserUin[LEN_USER_UIN];			//�û�Uin
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//ע���ʺ�
struct CMD_MB_RegisterAccounts
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����

	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//�ο͵�¼
struct CMD_MB_LogonVisitor
{
	//ϵͳ��Ϣ
	WORD							wModuleID;							//ģ���ʶ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//��¼�ɹ�
struct CMD_MB_LogonSuccess
{
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
	
	//�Ƹ���Ϣ
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserIngot;							//�û�Ԫ��
	SCORE							lUserInsure;						//�û�����	
	DOUBLE							dUserBeans;							//�û���Ϸ��

	//��չ��Ϣ
	BYTE							cbInsureEnabled;					//ʹ�ܱ�ʶ
	TCHAR							szLoginIP[LEN_ACCOUNTS];	
};

//��¼ʧ��
struct CMD_MB_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//������ʾ
struct CMD_MB_UpdateNotify
{
	BYTE							cbMustUpdate;						//ǿ������
	BYTE							cbAdviceUpdate;						//��������
	DWORD							dwCurrentVersion;					//��ǰ�汾
};

//////////////////////////////////////////////////////////////////////////////////
//�б�����

#define MDM_MB_SERVER_LIST			101									//�б���Ϣ

//�б���Ϣ
#define SUB_MB_LIST_KIND			100									//�����б�
#define SUB_MB_LIST_SERVER			101									//�����б�
#define SUB_MB_LIST_MATCH			102									//�����б�	
#define SUB_MB_LIST_FINISH			200									//�б����

#define SUB_MB_GET_ONLINE			300									//��ȡ����
#define SUB_MB_KINE_ONLINE			301									//��������
#define SUB_MB_SERVER_ONLINE		302									//��������

//////////////////////////////////////////////////////////////////////////////////

//��ȡ����
struct CMD_MB_GetOnline
{
	WORD							wServerCount;						//������Ŀ
	WORD							wOnLineServerID[MAX_SERVER];		//�����ʶ
};

//��������
struct CMD_MB_KindOnline
{
	WORD							wKindCount;							//������Ŀ
	tagOnLineInfoKind				OnLineInfoKind[MAX_KIND];			//��������
};

//��������
struct CMD_MB_ServerOnline
{
	WORD							wServerCount;						//������Ŀ
	tagOnLineInfoServer				OnLineInfoServer[MAX_SERVER];		//��������
};

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif