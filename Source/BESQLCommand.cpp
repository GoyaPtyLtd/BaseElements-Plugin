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


extern errcode g_last_ddl_error;
BESQLCommandAutoPtr g_ddl_command;


BESQLCommand::BESQLCommand ( const Text& _expression, const Text& _filename )
{
	expression->SetText ( _expression );
	filename->SetText ( _filename );

	column_separator->Assign ( "\t" );
	row_separator->Assign ( FILEMAKER_END_OF_LINE );
	
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

		// auto_ptrs : do not use the copy constructor
		BESQLCommandAutoPtr command ( new BESQLCommand ( *expression, *filename ) );
		command->set_column_separator ( *column_separator );
		command->set_row_separator ( *row_separator );
//		BESQLCommandAutoPtr command ( this );
		command->wait();
		
		g_ddl_command.swap ( command );
//		g_ddl_command.reset ( command );

	} else {
		
		errcode error = environment.ExecuteFileSQL ( *expression, *filename, *parameters, *result );
		
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
		const FMX_UInt32 number_of_rows = result->Size();
		const FMX_UInt32 last_row = number_of_rows - 1;
		
		for ( FMX_UInt32 row = 0; row < number_of_rows; ++row ) { 
			
			const DataVect& this_row = result->At( row ); 
			const FMX_UInt32 number_of_columns = this_row.Size();
			const FMX_UInt32 last_column = number_of_columns - 1;
			
			for ( FMX_UInt32 column = 0; column < number_of_columns; ++column ) {
				// should escape this ??? the old API doesn't so leave it to the FM developers
				text_result->AppendText ( this_row.At( column ).GetAsText() );
				// skip last time
				if ( column < last_column ) {
					text_result->AppendText( *column_separator );
				}
			}
			// skip last time
			if ( row < last_row ) {
				text_result->AppendText ( *row_separator );
			}
		}
	}
	
	return text_result;

}



void BESQLCommand::set_column_separator ( const Text& new_column_separator )
{
	column_separator->SetText ( new_column_separator );
}



void BESQLCommand::set_row_separator ( const Text& new_row_separator )
{
	row_separator->SetText ( new_row_separator );
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
