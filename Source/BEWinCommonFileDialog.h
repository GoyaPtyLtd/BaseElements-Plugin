/*
 BEWinCommonFileDialog.h
 BaseElements Plug-In
 
 Copyright 2014 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEWINCOMMONFILEDIALOG_H)
#define BEWINCOMMONFILEDIALOG_H


#include "BEPluginGlobalDefines.h"
#include "BEPluginUtilities.h"
#include "BEValueList.h"


class BEWinCommonFileOpenDialog
{
	
public:
	
	BEWinCommonFileOpenDialog ( ) {};
	BEWinCommonFileOpenDialog ( WStringAutoPtr prompt, WStringAutoPtr in_folder );

	virtual WStringAutoPtr Show ( );


protected:
	
		OPENFILENAME file_dialog;
		wchar_t path[PATH_MAX];

		wchar_t cfg_prompt[PATH_MAX];
		wchar_t cfg_in_folder[PATH_MAX];
	
};


typedef std::auto_ptr<BEWinCommonFileOpenDialog> BEWinCommonFileOpenDialogAutoPtr;


class BEWinCommonFileSaveDialog : public BEWinCommonFileOpenDialog
{
	
public:
	
	BEWinCommonFileSaveDialog ( ) {};
	BEWinCommonFileSaveDialog ( WStringAutoPtr prompt, WStringAutoPtr file_name, WStringAutoPtr in_folder );

	virtual WStringAutoPtr Show ( );


protected:

};


typedef std::auto_ptr<BEWinCommonFileSaveDialog> BEWinCommonFileSaveDialogAutoPtr;


#endif // BEWINCOMMONFILEDIALOG_H

