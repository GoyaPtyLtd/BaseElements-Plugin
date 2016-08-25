/*
 BEWinIFileDialog.h
 BaseElements Plug-In
 
 Copyright 2014-2016 Goya. All rights reserved.
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
	BEWinIFileOpenDialog ( const std::wstring& prompt, const std::wstring& in_folder );

	virtual const std::wstring Show ( );


protected:
	
	virtual void SetDefaultFolder ( );
	virtual const std::wstring GetSelection ( );
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
	
	BEWinIFileSaveDialog ( const std::wstring& prompt, const std::wstring& file_name, const std::wstring& in_folder );
	virtual const std::wstring Show ( );


protected:
	
	virtual void SetDefaultFolder ( );
	virtual const std::wstring GetSelection ( );

	IFileSaveDialog * file_dialog;
	std::wstring file_name;

};


typedef std::auto_ptr<BEWinIFileSaveDialog> BEWinIFileSaveDialogAutoPtr;


#endif // BEWINIFILEDIALOG_H

