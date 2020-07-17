/*
 
 Copyright © 1998 - 2020  Claris International Inc.
 All rights reserved.
 
 Claris International Inc. grants you a non-exclusive limited license to use this file solely
 to enable licensees of Claris FileMaker Pro to compile plug-ins for use with Claris products.
 Redistribution and use in source and binary forms, without modification, are permitted provided
 that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this list of
 conditions and the following disclaimer.
 
 * The name Claris International Inc. may not be used to endorse or promote products derived
 from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY CLARIS INTERNATIONAL INC. ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL FILEMAKER, INC. BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 */

#ifndef _h_Extern_
#define _h_Extern_

// This will bring in standard C++ macros we reference
#include <cstddef>
#include <memory>

// Platform specific macros

#if defined(__GNUC__)

	#define FMX_PACK                    __attribute__ ((packed))
	#define FMX_PACK_ON
	#define FMX_PACK_OFF
	#define FMX_PROC(retType)           retType
	#define FMX_PROCPTR(retType, name)  typedef retType (*name)
	#define FMX_ENTRYPT
	#define FMX_API

	#if !defined(DEPRECATED)
		#define DEPRECATED              __attribute__((deprecated))
	#endif

	#if defined (_LIBCPP_STD_VER) && _LIBCPP_STD_VER > 14
		#define FMX_USE_AUTO_PTR    0
	#else
		#define FMX_USE_AUTO_PTR    1
	#endif

	#if defined(_UNIQUE_PTR_H) || defined(_LIBCPP_VERSION)
		#define FMX_USE_UNIQUE_PTR      1
	#endif

	#if defined(_LP64)
		#define FMX_64BIT_PLUGIN_API    1
	#endif

#elif defined(_MSC_VER)

	#define FMX_PACK
	#define FMX_PACK_ON                 pack (push, 1)
	#define FMX_PACK_OFF                pack (pop)
	#define FMX_PROC(retType)           retType __stdcall
	#define FMX_PROCPTR(retType, name)  typedef retType (__stdcall *name)
	#define FMX_ENTRYPT                 __declspec(dllexport)
	#define FMX_API                     __declspec(dllimport)

	#if !defined(DEPRECATED)
		#define DEPRECATED              __declspec(deprecated)
	#endif

	#if defined(_MSVC_LANG) && _MSVC_LANG <= 201402
		#define FMX_USE_AUTO_PTR    1
	#endif

	#if _MSC_VER >= 1800
		#define FMX_USE_UNIQUE_PTR      1
	#endif

	#if defined(_M_X64)
		#define FMX_64BIT_PLUGIN_API    1
	#endif

#else

	#error "Unsupported compiler"

#endif

// Original memorary manager callbacks and External() call exists only on Mac and Win 32 bit targets

#if defined(FMX_64BIT_PLUGIN_API) || defined(__linux__) || TARGET_OS_IPHONE
	#define FMX_OBSOLETE_API    0
#else
	#define FMX_OBSOLETE_API    1
#endif

// Forward definitions

extern "C++"
{
	namespace fmx
	{
		class Text;
		class Data;
		class ExprEnv;
		typedef unsigned short      uint16;
		typedef short               int16;
		typedef unsigned char       uchar;

		typedef int                 int32;
		typedef unsigned int        uint32;

	#if defined( _MSC_VER )
		typedef __int64             int64;
		typedef unsigned __int64    uint64;
		
		#if defined( _M_X64)
		typedef uint64              ptrtype;
		#else
		typedef uint32              ptrtype;
		#endif
		typedef int16               unusedid;
	#else
		#if defined( _LP64 )
		typedef long                int64;
		typedef unsigned long       uint64;
		typedef uint64              ptrtype;
		typedef int32               unusedid;
		#else
		typedef long long           int64;
		typedef unsigned long long  uint64;
		typedef uint32              ptrtype;
		typedef int16               unusedid;
		#endif
	#endif
		typedef short               errcode;
		typedef unsigned short      unichar16;

		typedef DEPRECATED unichar16 unichar; // DEPRECATED in version 15. unichar conflicts often with Cocoa's definition of it
		
#if FMX_OBSOLETE_API

		// DEPRECATED in version 15. These are provided for backwards compatibility. Newly developed plugins(both 32-bit and 64-bit)
		// should be specific about the sizes of integers and not rely on ambiguous "long" data types.
		typedef DEPRECATED fmx::uint32     ulong;
		typedef DEPRECATED fmx::uint16     ushort;
		typedef DEPRECATED fmx::int64      longlong;

#endif // FMX_OBSOLETE_API
		
	}
}
extern "C"
{
	typedef fmx::Text       FMX_Text;
	typedef fmx::Data       FMX_Data;
	typedef fmx::ExprEnv    FMX_ExprEnv;
	typedef fmx::uchar      FMX_UChar;
	typedef fmx::int16      FMX_Int16;
	typedef fmx::uint16     FMX_UInt16;
	typedef fmx::int32      FMX_Int32;
	typedef fmx::uint32     FMX_UInt32;
	typedef fmx::unichar16  FMX_Unichar;
	typedef fmx::errcode    FMX_ErrorCode;
	typedef fmx::ptrtype    FMX_PtrType;
	typedef char            FMX_Char;
	typedef char            FMX_Boolean;

#if FMX_OBSOLETE_API

// DEPRECATED in version 15. Old style definitions; never were supported in 64-bit plugins;

// These are provided for backwards compatibility. Newly developed plugins(both 32-bit and 64-bit) should be specific about the sizes of
// integers and not rely on ambiguous "long" data types.
typedef DEPRECATED FMX_UInt32      FMX_ULong;
typedef DEPRECATED FMX_UInt16      FMX_UShort;
typedef DEPRECATED FMX_Int32       FMX_Long;
typedef DEPRECATED FMX_Int16       FMX_Short;
typedef DEPRECATED FMX_Char*       FMX_Ptr;
typedef DEPRECATED FMX_Char**      FMX_Hdl;

// Memory accessors for old style plug-ins
// New style plugins should not use these.  When creating new objects (BinaryData, Text, etc), use the autoptr classes provided in the SDK.

FMX_PROCPTR(char**,     FMX_NewHandleCall)(FMX_Int32 size);
FMX_PROCPTR(void,       FMX_SetHandleSizeCall)(char** h, FMX_Int32 size);
FMX_PROCPTR(FMX_Int32,  FMX_GetHandleSizeCall)(char** h);
FMX_PROCPTR(void,       FMX_DisposeHandleCall)(char** h);
FMX_PROCPTR(void,       FMX_MoveHHiCall)(char** h);
FMX_PROCPTR(void,       FMX_LockHandleCall)(char** h);
FMX_PROCPTR(void,       FMX_UnlockHandleCall)(char** h);
FMX_PROCPTR(char*,      FMX_NewPointerCall)(FMX_Int32 size);
FMX_PROCPTR(void,       FMX_DisposePointerCall)(char* p);
FMX_PROCPTR(FMX_Int16,  FMX_MemoryErrorCall)(void);

#define FMX_NewHandle(size)         (gFMX_ExternCallPtr->cNewHandle)(size)
#define FMX_SetHandleSize(h, size)  (gFMX_ExternCallPtr->cSetHandleSize)(h, size)
#define FMX_GetHandleSize(h)        (gFMX_ExternCallPtr->cGetHandleSize)(h)
#define FMX_DisposeHandle(h)        (gFMX_ExternCallPtr->cDisposeHandle)(h)
#define FMX_MoveHHi(h)              (gFMX_ExternCallPtr->cMoveHHi)(h)
#define FMX_LockHandle(h)           (gFMX_ExternCallPtr->cLockHandle)(h)
#define FMX_UnlockHandle(h)         (gFMX_ExternCallPtr->cUnlockHandle)(h)
#define FMX_NewPointer(size)        (gFMX_ExternCallPtr->cNewPointer)(size)
#define FMX_DisposePointer(p)       (gFMX_ExternCallPtr->cDisposePointer)(p)
#define FMX_MemoryError()           (gFMX_ExternCallPtr->cMemoryError)()

#endif // FMX_OBSOLETE_API

// New public callbacks

typedef FMX_UChar   FMX_ScriptControl;
enum 
{
	kFMXT_Halt,
	kFMXT_Exit,
	kFMXT_Resume,
	kFMXT_Pause
};

FMX_PROCPTR(FMX_ErrorCode,  FMX_StartScriptCall)(const FMX_Text* fileName, const FMX_Text* scriptName, FMX_ScriptControl control, const FMX_Data* parameter);
FMX_PROCPTR(FMX_ErrorCode,  FMX_CurrentEnvCall)(FMX_ExprEnv* env);

#define FMX_StartScript(fileName, scriptName, control, parameter)       (gFMX_ExternCallPtr->cStartScript)(fileName, scriptName, control, parameter)
#define FMX_SetToCurrentEnv(env)                                        (gFMX_ExternCallPtr->cCurrentEnv)(env)

//::::::::::::::::::::::::::::    Call Back Struct    ::::::::::::::::::::::::::::

enum
{
	kBadExtnVersion     = -1,
	kDoNotEnable        = -2,
	k40ExtnVersion      = 11,
	k41ExtnVersion      = 12,
	k50ExtnVersion      = 14,
	k60ExtnVersion      = 17,
	k70ExtnVersion      = 50,           // Jumping to 50
	k80ExtnVersion      = 51,
	k110ExtnVersion     = 52,
	k120ExtnVersion     = 53,           // Support for 64-bit plugins
	k130ExtnVersion     = 54,
	k140ExtnVersion     = 55,
	k150ExtnVersion     = 56,
	k160ExtnVersion     = 57,
	k170ExtnVersion     = 59,
	k180ExtnVersion     = 60,
	k190ExtnVersion     = 62,
	kCurrentExtnVersion = 62,
	kMinExtnVersion     = 4,
	kMaxExtnVersion     = 255
};

typedef FMX_UChar   FMX_ExternCallSwitch;
enum
{
	kFMXT_Init              = 0,        // Enabled by kFMXT_OptionsStr character 8
	kFMXT_Idle              = 1,        // Enabled by kFMXT_OptionsStr character 9
#if FMX_OBSOLETE_API
	kFMXT_External          = 3,        // DEPRECATED External function callback
#endif
	kFMXT_Shutdown          = 4,        // Enabled by kFMXT_OptionsStr character 8
	kFMXT_DoAppPreferences  = 5,        // Enabled by kFMXT_OptionsStr character 6
	kFMXT_GetString         = 7,        // REQUIRED to be handled

	// New to FileMaker Pro 15 (API VERSION 56) and later
	kFMXT_SessionShutdown   = 8,        // Enabled by kFMXT_OptionsStr character 10
	kFMXT_FileShutdown      = 9         // Enabled by kFMXT_OptionsStr character 10
};

typedef FMX_UChar   FMX_Strings;        // Different strings that may be asked for by kFMXT_GetString
enum    
{
	kFMXT_NameStr           = 128,      // Name of the plug-in
	kFMXT_AppConfigStr      = 129,      // Help text to display in FileMaker application preferences
	kFMXT_OptionsStr        = 131,      // The option string

	// New to FileMaker Pro 16 (API VERSION 57) and later
	kFMXT_HelpURLStr        = 132       // Help URL for External Functions and Steps. If ends with "=" the Function/Step ID will be appended.
};

typedef FMX_UChar   FMX_IdleLevel;
enum
{
	kFMXT_UserIdle          = 0,        // The user hasn't done anything for 30 seconds or more.
	kFMXT_UserNotIdle       = 1,        // The user has done something within 30 seconds.
	kFMXT_ScriptPaused      = 2,        // The user is running a script that is paused.
	kFMXT_ScriptRunning     = 3,        // The user is running a script.
	kFMXT_Unsafe            = 4         // Same as if unsafeCalls is set to true.
};

typedef FMX_UChar   FMX_Application;
enum
{
	kFMXT_Developer         = 0,        // FileMaker Pro Advanced
	kFMXT_Pro               = 1,        // FileMaker Pro
	kFMXT_Runtime           = 2,        // FileMaker Runtime
	kFMXT_Server            = 3,        // This process no longer loads plug-ins
	kFMXT_Web               = 4,        // Web Publishing process
	kFMXT_Mobile            = 5,        // This iOS process is not allowed to load plug-ins
	kFMXT_XDBC              = 6,        // This process does not currently load plug-ins
	kFMXT_SASE              = 7,        // Server scripting process
	kFMXT_IWP               = 8         // This process no longer exists
};

struct              FMX_ExternCallStruct;
typedef struct      FMX_ExternCallStruct *FMX_ExternCallPtr;
typedef             void (*FMX_ExternCallProc)(FMX_ExternCallPtr);

#pragma FMX_PACK_ON
struct FMX_ExternCallStruct
{

	// Passed in every call
		FMX_Int16               extnVersion;                    // Version of this structure, set to the value of kCurrentExtnVersion
		fmx::unusedid           unusedID;                       // Always -1 as of FileMaker Pro version 19 (API VERSION 61)
		FMX_ExternCallProc      entryPoint;                     // The pointer the plug-ins entry point, FMExternCallProc
		FMX_Boolean             cfmCalls;                       // Always false (i.e. 0)
		FMX_ExternCallSwitch    whichCall;                      // Which call FileMaker is making into the plug-in (i.e. kFMXT_Init and others)
		FMX_Boolean             unsafeCalls;                    // True if unsafe for non-memory calls

	// Parameters for the calls; any parameter not used by a call is set to zero.
		FMX_UChar               parm1;
		FMX_PtrType             parm2;  
		FMX_PtrType             parm3;
		
	// The meanings of parm1..parm3 in terms of the various messages:
	//  Msg =                   Parm1                       Parm2                           Parm3
	//  kFMXT_Init              FMX_Application value       App vers unicode c str*         [unused]
	//  kFMXT_Idle              FMX_IdleLevel value         Session ID                      [unused]
	//  kFMXT_External          [unused]                    Funct str index                 Parameter text**
	//  kFMXT_Shutdown          [unused]                    [unused]                        [unused]
	//  kFMXT_DoAppPreferences  [unused]                    [unused]                        [unused]
	//  kFMXT_GetString         FMX_Strings value           Windows lang ID                 Maximum size of string to return
	//  kFMXT_SessionShutdown   [unused]                    Session ID                      [unused]
	//  kFMXT_FileShutdown      [unused]                    Session ID                      File ID
	//
	//(* same as GetAppVersion, e.g. "Pro 13.0v2" )
	//(** Parameter from calculation as text, in kEncoding_ASCII_Mac or kEncoding_ShiftJIS_Mac; Result passed back to FileMaker must match encoding.)
			
	// Passed in every call
		FMX_PtrType             instanceID;                     // ID of the plug-in
																//      On Windows, it is the Instance ID of the plug-in.
																//      On Mac OS X, it is the BundleRef of the plug-in.

	// Result from a call
		FMX_PtrType             result;                         // kFMXT_Init returns it's kCurrentExtnVersion

	// Unused       
		FMX_PtrType             unused;         

#if FMX_OBSOLETE_API
	// DEPRECATED in version 15. Call backs for old plug-ins
		DEPRECATED FMX_NewHandleCall       cNewHandle;
		DEPRECATED FMX_SetHandleSizeCall   cSetHandleSize;
		DEPRECATED FMX_GetHandleSizeCall   cGetHandleSize;
		DEPRECATED FMX_DisposeHandleCall   cDisposeHandle;
		DEPRECATED FMX_MoveHHiCall         cMoveHHi;
		DEPRECATED FMX_LockHandleCall      cLockHandle;
		DEPRECATED FMX_UnlockHandleCall    cUnlockHandle;
		DEPRECATED FMX_NewPointerCall      cNewPointer;
		DEPRECATED FMX_DisposePointerCall  cDisposePointer;
		DEPRECATED FMX_MemoryErrorCall     cMemoryError;
#endif
	
	// New public callbacks
		FMX_StartScriptCall     cStartScript;
		FMX_CurrentEnvCall      cCurrentEnv;

} FMX_PACK;
#pragma FMX_PACK_OFF

// These need to be defined in the plug-in.

extern void FMX_ENTRYPT FMExternCallProc(FMX_ExternCallPtr pb);
extern FMX_ExternCallPtr gFMX_ExternCallPtr;

}


#endif
