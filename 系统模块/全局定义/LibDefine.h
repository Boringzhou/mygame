#ifndef LIB_DEFINE_HEAD_FILE
#define LIB_DEFINE_HEAD_FILE
//////////////////////////////////////////////////////////////////////////////////
// ���ӿⶨ��
#ifdef _DEBUG
	#define LIB_TYPE "Debug"
#else
	#define LIB_TYPE "Release"
#endif // _DEBUG

#ifdef _UNICODE
	#define LIB_CHARSET "Unicode"
#else
	#define LIB_CHARSET "Ansi"
#endif // _UNICODE

#define LIB_NAME(libname)			"../../���ӿ�/"LIB_TYPE"_"LIB_CHARSET"/"##libname##".lib"
#define LIB_COMMON(libname)			"../../���ӿ�/Common/"##libname##"_"LIB_TYPE".lib"
#define LIB_NAME4GAME(libname)		"../../../../ϵͳģ��/���ӿ�/"LIB_TYPE"_"LIB_CHARSET"/"##libname##".lib"
#define LIB_COMMON4GAME(libname)	"../../../../ϵͳģ��/���ӿ�/Common/"##libname##"_"LIB_TYPE".lib"

#endif // LIB_DEFINE_HEAD_FILE