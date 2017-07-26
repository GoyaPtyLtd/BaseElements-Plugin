/*
 BESQLCommand.h
 BaseElements Plug-In
 
 Copyright 2011-2017 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#if !defined(BESQLCOMMAND_H)
#define BESQLCOMMAND_H


#include "BEPluginGlobalDefines.h"


class BESQLCommand
{
	
public:
	
	BESQLCommand ( const fmx::Text& expression, const fmx::Text& filename );

	void execute ( void );
	void execute ( const fmx::ExprEnv& _environment );
	fmx::TextUniquePtr get_text_result ( void );
	
	void set_column_separator ( const fmx::Text& new_column_separator );
	void set_row_separator ( const fmx::Text& new_row_separator );
	
	void wait ( ) { waiting = true; };
	
protected:
	
	fmx::TextUniquePtr expression;
	fmx::TextUniquePtr filename;
	fmx::DataVectUniquePtr parameters;
	fmx::DataUniquePtr result;
	
	fmx::uint16 column_separator;
	fmx::uint16 row_separator;
	
	bool is_ddl_command ( void ) const;
	
	bool waiting;
	
};


typedef std::unique_ptr<BESQLCommand> BESQLCommandUniquePtr;


#endif // BESQLCOMMAND_H

