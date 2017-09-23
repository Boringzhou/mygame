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

//���һλ����ʾ���� �м�λ��ǿ�Ƹ���  ��һλǿ�Ƹ���
//����汾
#define MainVer 8
#define SubVer 0
#define BuildVer 1

#define VERSION_FRAME				PROCESS_VERSION(MainVer,SubVer,BuildVer)				//��ܰ汾
#define VERSION_PLAZA				PROCESS_VERSION(MainVer,SubVer,BuildVer)				//�����汾
#define VERSION_MOBILE_ANDROID		PROCESS_VERSION(MainVer,SubVer,BuildVer)				//�ֻ��汾
#define VERSION_MOBILE_IOS			PROCESS_VERSION(MainVer,SubVer,BuildVer)				//�ֻ��汾

//�汾����
#define VERSION_EFFICACY			1									//Ч��汾
#define VERSION_FRAME_SDK			INTERFACE_VERSION(6,1)				//��ܰ汾

//////////////////////////////////////////////////////////////////////////////////
//�����汾

const TCHAR szProduct[]=TEXT("һ�������");							//��Ʒ����
const TCHAR szProductKey[]=TEXT("YDDGamePlatform");					//��Ʒ����

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