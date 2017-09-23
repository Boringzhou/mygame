#ifndef FGUI_LIB_HEAD_FILE
#define FGUI_LIB_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�궨��
#ifdef FGUI_LIB_DLL
#define FGUI_LIB_CLASS __declspec(dllexport)
#else
#define FGUI_LIB_CLASS __declspec(dllimport)
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define FGUILIB_DLL_NAME	TEXT("FGuilib.dll")			//�������
#else
	#define FGUILIB_DLL_NAME	TEXT("FGuilibD.dll")		//�������
#endif

//////////////////////////////////////////////////////////////////////////////////

//�����ļ�
#include "UIBase.h"
#include "UIManager.h"
#include "UIRender.h"	
#include "UIDelegate.h"
#include "UIControl.h"	
#include "UIContainer.h"
#include "UICommon.h"
//#include "UIRichEdit.h"
#include "UIList.h"
#include "UICombo.h"	
#include "UIMarkup.h"
#include "UIResLoader.h"
#include "UIDlgBuilder.h"

//////////////////////////////////////////////////////////////////////////////////

#endif
