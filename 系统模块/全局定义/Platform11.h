#ifndef PLATFORM_HEAD_FILE
#define PLATFORM_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

//�����ļ�
#include "Macro.h"
#include "Define.h"

//�ṹ�ļ�
#include "Struct.h"
#include "Packet.h"
#include "Property.h"

//ģ���ļ�
#include "Array.h"
#include "Module.h"
#include "PacketAide.h"
#include "ServerRule.h"
#include "RightDefine.h"

//////////////////////////////////////////////////////////////////////////////////

//����汾
#define VERSION_FRAME				PROCESS_VERSION(7,0,1)				//��ܰ汾
#define VERSION_PLAZA				PROCESS_VERSION(7,0,1)				//�����汾
#define VERSION_MOBILE_ANDROID		PROCESS_VERSION(7,0,1)				//�ֻ��汾
#define VERSION_MOBILE_IOS			PROCESS_VERSION(7,0,1)				//�ֻ��汾

//�汾����
#define VERSION_EFFICACY			0									//Ч��汾
#define VERSION_FRAME_SDK			INTERFACE_VERSION(7,1)				//��ܰ汾

//////////////////////////////////////////////////////////////////////////////////
//�����汾

#ifndef _DEBUG

//ƽ̨����
const TCHAR szProduct[]=TEXT("389��������");							//��Ʒ����
const TCHAR szPlazaClass[]=TEXT("389GamePlaza");						//�㳡����
const TCHAR szProductKey[]=TEXT("389GamePlatform");					//��Ʒ����

//��ַ����
const TCHAR szCookieUrl[]=TEXT("http://www.389dw.com");					//��¼��ַ
const TCHAR szLogonServer[]=TEXT("120.27.7.57");						//��¼��ַ
const TCHAR szPlatformLink[]=TEXT("http://www.389dw.com");				//ƽ̨��վ
const TCHAR szValidateKey[]=TEXT("JDSyncLoginKey");						//��֤��Կ
const TCHAR szValidateLink[]=TEXT("SyncLogin.aspx?userid=%d&time=%d&signature=%s&url=/"); //��֤��ַ 

#else

//////////////////////////////////////////////////////////////////////////////////
//�ڲ�汾

const TCHAR szProduct[]=TEXT("389��������");							//��Ʒ����
const TCHAR szPlazaClass[]=TEXT("389GamePlaza");						//�㳡����
const TCHAR szProductKey[]=TEXT("389GamePlatform");					//��Ʒ����

//��ַ����
const TCHAR szCookieUrl[]=TEXT("http://www.389dw.com");					//��¼��ַ
const TCHAR szLogonServer[]=TEXT("120.27.7.57");						//��¼��ַ
const TCHAR szPlatformLink[]=TEXT("http://www.389dw.com");				//ƽ̨��վ
const TCHAR szValidateKey[]=TEXT("JDSyncLoginKey");						//��֤��Կ
const TCHAR szValidateLink[]=TEXT("SyncLogin.aspx?userid=%d&time=%d&signature=%s&url=/"); //��֤��ַ 

#endif

//////////////////////////////////////////////////////////////////////////////////

//���ݿ���
const TCHAR szPlatformDB[]=TEXT("THPlatformDB");						//ƽ̨���ݿ�
const TCHAR szAccountsDB[]=TEXT("THAccountsDB");						//�û����ݿ�
const TCHAR szTreasureDB[]=TEXT("THTreasureDB");						//�Ƹ����ݿ�
const TCHAR szGameMatchDB[]=TEXT("THGameMatchDB");						//�������ݿ�
const TCHAR szExerciseDB[]=TEXT("THExerciseDB");						//��ϰ���ݿ�

//////////////////////////////////////////////////////////////////////////////////

//��Ȩ��Ϣ
const TCHAR szCompilation[]=TEXT("217F8DFB-FE8A-4C3B-A023-C35489A1C0F5");

//////////////////////////////////////////////////////////////////////////////////

#endif