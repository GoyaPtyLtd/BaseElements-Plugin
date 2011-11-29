/*
 *  BESQLCommand.cpp
 *  BaseElements
 *
 *  Created by Mark Banks on 28/11/11.
 *  Copyright 2011 Goya. All rights reserved.
 *
 */

#include "BESQLCommand.h"
#include "BEPluginGlobalDefines.h"


#include <iostream>


extern errcode g_last_ddl_error;
BESQLCommandAutoPtr g_ddl_command;


BESQLCommand::BESQLCommand ( const TextAutoPtr _expression, const TextAutoPtr _filename )
{
	expression->SetText ( *_expression );
	filename->SetText ( *_filename );

	column_separator->Assign ( "\t" );
	row_separator->Assign ( "\r" );
	
	waiting = false;
}



void BESQLCommand::execute ( const bool needs_environment )
{
	const ExprEnvAutoPtr environment;
	FMX_SetToCurrentEnv ( &(*environment) );
	execute ( *environment );
}



void BESQLCommand::execute ( const ExprEnv& environment )
{	
	bool ddl = is_ddl_command();
	
	if ( !waiting && ddl ) {
		BESQLCommandAutoPtr command ( this );
		g_ddl_command = command;
		waiting = true;
	} else {
		errcode error = kNoError;

		error = environment.ExecuteFileSQL ( *expression, *filename, *parameters, *result );

		waiting = false;
		
		if ( ddl ) {
			g_last_ddl_error = error;
		} else {
			g_last_error = error;
		}
	}
}



TextAutoPtr BESQLCommand::get_text_result ( void )
{
	TextAutoPtr	text_result;

	if ( !waiting ) {
		const ulong number_of_rows = result->Size();
		const ulong last_row = number_of_rows - 1;
		
		for ( ulong row = 0; row < number_of_rows; ++row ) { 
			
			const DataVect& this_row = result->At( row ); 
			const ulong number_of_columns = this_row.Size();
			const ulong last_column = number_of_columns - 1;
			
			for ( ulong column = 0; column < number_of_columns; ++column ) {
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



void BESQLCommand::set_column_separator ( const TextAutoPtr new_column_separator )
{
	column_separator->SetText ( *new_column_separator );
}



void BESQLCommand::set_row_separator ( const TextAutoPtr new_row_separator )
{
	row_separator->SetText ( *new_row_separator );
}



bool BESQLCommand::is_ddl_command ( void ) const 
{
	const TextAutoPtr alter;
	alter->Assign ( "ALTER" );
	
	const TextAutoPtr create;
	create->Assign ( "CREATE" );
	
	const TextAutoPtr drop;
	drop->Assign ( "DROP" );
		
	bool is_ddl = expression->FindIgnoringCase ( *alter, 0 ) == 0 ||
				expression->FindIgnoringCase ( *create, 0 ) == 0 ||
				expression->FindIgnoringCase ( *drop, 0 ) == 0;

	return is_ddl;
	
}
