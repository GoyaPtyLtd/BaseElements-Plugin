//
//  FMX_Exports.h
//
//  Copyright © 2017 FileMaker, Inc. All rights reserved.
//

#ifndef FMX_Exports_h
#define FMX_Exports_h



#ifndef _h_Extern_
typedef unsigned char   FMX_ScriptControl;
enum
{
	kFMXT_Halt,
	kFMXT_Exit,
	kFMXT_Resume,
	kFMXT_Pause
};
#endif


//	FMX_Queue_Script
//
//	Queues script <scriptName> from file <fileName> to be queued to run.
//	Parameter scriptParam will be passed to the script as a parameter.  scriptParam may be nil.
//	The variables dictionary, if non-nil, supplies one or more local variables with values which
//	will be set during execution of the script.
//	The user account must have FMURLScript extended privilege.
//	Returns true if the script was successfully queued.

extern bool FMX_Queue_Script(NSString *fileName, NSString *scriptName, FMX_ScriptControl control, NSString *scriptParam, NSDictionary<NSString *, NSString *> *variables);



#endif /* FMX_Exports_h */
