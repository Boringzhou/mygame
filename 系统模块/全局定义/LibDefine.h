#ifndef LIB_DEFINE_HEAD_FILE
#define LIB_DEFINE_HEAD_FILE
//////////////////////////////////////////////////////////////////////////////////
// 链接库定义
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

#define LIB_NAME(libname)			"../../链接库/"LIB_TYPE"_"LIB_CHARSET"/"##libname##".lib"
#define LIB_COMMON(libname)			"../../链接库/Common/"##libname##"_"LIB_TYPE".lib"
#define LIB_NAME4GAME(libname)		"../../../../系统模块/链接库/"LIB_TYPE"_"LIB_CHARSET"/"##libname##".lib"
#define LIB_COMMON4GAME(libname)	"../../../../系统模块/链接库/Common/"##libname##"_"LIB_TYPE".lib"

#endif // LIB_DEFINE_HEAD_FILE