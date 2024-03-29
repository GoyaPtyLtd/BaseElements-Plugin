/*
 BESQLCommand.h
 BaseElements Plug-In

 Copyright 2011-2021 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#if !defined(BESQLCOMMAND_H)
#define BESQLCOMMAND_H


#include "BEPluginGlobalDefines.h"

#include <string>
#include <vector>


class BESQLCommand
{

public:

	BESQLCommand ( const fmx::Text& expression, const fmx::Text& filename );
	BESQLCommand ( const std::string& expression, const std::string& filename );

	const fmx::errcode execute ( void );
	const fmx::errcode execute ( const fmx::ExprEnv& _environment, const bool text_result_wanted = true );
	fmx::TextUniquePtr get_text_result ( void );
	fmx::DataUniquePtr get_data_result ( void ) const;
	std::vector<char> get_vector_result ( void ) const;

	void set_column_separator ( const fmx::Text& new_column_separator );
	void set_row_separator ( const fmx::Text& new_row_separator );

	void wait ( ) { waiting = true; };
	const fmx::errcode get_ddl_error ( ) { return ddl_error; }


protected:

	fmx::TextUniquePtr expression;
	fmx::TextUniquePtr filename;
	fmx::DataVectUniquePtr parameters;
	fmx::DataUniquePtr result;

	fmx::uint16 column_separator = '\t';
	fmx::uint16 row_separator = FILEMAKER_END_OF_LINE_CHAR;

	bool is_ddl_command ( void ) const;

	bool waiting = false;

	fmx::errcode error = kNoError;
	fmx::errcode ddl_error = kNoError;

};


typedef std::unique_ptr<BESQLCommand> BESQLCommandUniquePtr;


#endif // BESQLCOMMAND_H

