/*
 BEWinIFileDialog.h
 BaseElements Plug-In
 
 Copyright 2014 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BEWINIFILEDIALOG_H)
#define BEWINIFILEDIALOG_H


#include "BEPluginGlobalDefines.h"
#include "BEPluginUtilities.h"
#include "BEValueList.h"

#include <ShObjIdl.h>


class BEWinIFileOpenDialog
{
	
public:
	
	BEWinIFileOpenDialog ( ) {};
	BEWinIFileOpenDialog ( WStringAutoPtr prompt, WStringAutoPtr in_folder );

	virtual WStringAutoPtr Show ( );


protected:
	
	virtual void SetDefaultFolder ( );
	virtual WStringAutoPtr GetSelection ( );
	void CheckIfUserCancelled ( );


	std::wstring prompt;
	std::wstring in_folder;
	HRESULT hr;

	IFileOpenDialog * file_dialog;
	DWORD dialog_flags;
	CLSID dialog_type;

};


typedef std::auto_ptr<BEWinIFileOpenDialog> BEWinIFileOpenDialogAutoPtr;


class BEWinIFileSaveDialog : public BEWinIFileOpenDialog
{
	
public:
	
	BEWinIFileSaveDialog ( WStringAutoPtr prompt, WStringAutoPtr file_name, WStringAutoPtr in_folder );
	virtual WStringAutoPtr Show ( );


protected:
	
	virtual void SetDefaultFolder ( );
	virtual WStringAutoPtr GetSelection ( );

	IFileSaveDialog * file_dialog;
	std::wstring file_name;

};


typedef std::auto_ptr<BEWinIFileSaveDialog> BEWinIFileSaveDialogAutoPtr;


#endif // BEWINIFILEDIALOG_H

