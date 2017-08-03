/*
 BESQLCommand.cpp
 BaseElements Plug-In
 
 Copyright 2011-2017 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#include "BESQLCommand.h"
#include "BEPluginGlobalDefines.h"


using namespace std;
using namespace fmx;


thread_local extern errcode g_last_ddl_error;
BESQLCommandUniquePtr g_ddl_command;


BESQLCommand::BESQLCommand ( const Text& _expression, const Text& _filename )
{
	expression->SetText ( _expression );
	filename->SetText ( _filename );

	column_separator = '\t';
	row_separator = FILEMAKER_END_OF_LINE_CHAR;
	
	waiting = false;
}


void BESQLCommand::execute ( )
{
	const ExprEnvUniquePtr environment;
	FMX_SetToCurrentEnv ( &(*environment) );
	execute ( *environment );
}



void BESQLCommand::execute ( const ExprEnv& environment )
{	
	bool ddl = is_ddl_command();
	
	if ( ddl && !waiting ) {

		// unique_ptrs : do not use the copy constructor
		BESQLCommandUniquePtr command ( new BESQLCommand ( *expression, *filename ) );
		command->wait();
		
		g_ddl_command.swap ( command );

	} else {
		
		errcode error = environment.ExecuteFileSQLTextResult ( *expression, *filename, *parameters, *result, column_separator, row_separator );
		
		if ( ddl ) {
			g_last_ddl_error = error;
		} else {
			g_last_error = error;
		}
	}
}



TextUniquePtr BESQLCommand::get_text_result ( void )
{
	TextUniquePtr	text_result;

	if ( !waiting ) {
		text_result->AppendText( result->GetAsText() );
	}
	
	return text_result;

}



void BESQLCommand::set_column_separator ( const Text& new_column_separator )
{
	new_column_separator.GetUnicode ( &column_separator, 0, 1 );
}



void BESQLCommand::set_row_separator ( const Text& new_row_separator )
{
	new_row_separator.GetUnicode ( &row_separator, 0, 1 );
}



bool BESQLCommand::is_ddl_command ( void ) const 
{
	const TextUniquePtr alter;
	alter->Assign ( "ALTER" );
	
	const TextUniquePtr create;
	create->Assign ( "CREATE" );
	
	const TextUniquePtr drop;
	drop->Assign ( "DROP" );
		
	bool is_ddl = expression->FindIgnoringCase ( *alter, 0 ) == 0 ||
				expression->FindIgnoringCase ( *create, 0 ) == 0 ||
				expression->FindIgnoringCase ( *drop, 0 ) == 0;

	return is_ddl;
	
}
