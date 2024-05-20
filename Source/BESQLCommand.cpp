/*
 BESQLCommand.cpp
 BaseElements Plug-In

 Copyright 2011-2024 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#include "BESQLCommand.h"
#include "BEPluginGlobalDefines.h"
#include "BEPluginUtilities.h"


using namespace std;
using namespace fmx;


BESQLCommandUniquePtr g_ddl_command;


BESQLCommand::BESQLCommand ( const Text& _expression, const Text& _filename )
{
	expression->SetText ( _expression );
	filename->SetText ( _filename );
}


BESQLCommand::BESQLCommand ( const std::string& _expression, const std::string& _filename )
{
	expression->Assign ( _expression.c_str(), fmx::Text::kEncoding_UTF8 );
	filename->Assign ( _filename.c_str(), fmx::Text::kEncoding_UTF8 );
}


const fmx::errcode BESQLCommand::execute ( )
{
	ExprEnvUniquePtr environment;
	FMX_SetToCurrentEnv ( &(*environment) );
	return execute ( *environment );
}



const fmx::errcode BESQLCommand::execute ( const ExprEnv& environment, const bool text_result_wanted )
{
	bool ddl = is_ddl_command();

	if ( ddl && !waiting ) {

		// unique_ptrs : do not use the copy constructor
		BESQLCommandUniquePtr command ( new BESQLCommand ( *expression, *filename ) );
		command->wait();

#pragma TODO Remove global dependency
		
		g_ddl_command.swap ( command );

	} else {
		
		if ( text_result_wanted ) {
			
			error = environment.ExecuteFileSQLTextResult ( *expression, *filename, *parameters, *result, column_separator, row_separator );

		} else {
		
			RowVectUniquePtr records_found;
			error = environment.ExecuteFileSQL ( *expression, *filename, *parameters, *records_found );
			auto rows = records_found->Size();
	
			if ( rows > 0 ) {
				
				auto& this_row = records_found->At(0);
				auto columns = this_row.Size();

				if ( columns == 0 ) {
					; // do nothing
				} else if ( rows == 1 &&  columns == 1 ) {
					
					// take the first field/column of the first record
					auto& container = this_row.At(0);
					if ( container.GetNativeType() == fmx::Data::kDTBinary ) {
						result->SetBinaryData ( container.GetBinaryData() );
					} else {
						error = kInvalidFieldType;
					}
					
				} else {
					error = kInvalidFieldType;
				}

			}

		} // if ( text_result_wanted )
		
		if ( ddl ) {
			ddl_error = error;
		}

	}
	
	return error;

} // execute



DataUniquePtr BESQLCommand::get_data_result ( void ) const
{
	DataUniquePtr out;
	out->SetBinaryData ( result->GetBinaryData() );
	return out;
}


TextUniquePtr BESQLCommand::get_text_result ( void )
{
	TextUniquePtr text_result;

	if ( !waiting ) {
		text_result->AppendText ( result->GetAsText() );
	}

	return text_result;

}


std::vector<char> BESQLCommand::get_vector_result ( void ) const
{
	std::vector<char> vector_result;

	if ( !waiting ) {
		vector_result = DataAsVectorChar ( *result );
	}

	return vector_result;

}


void BESQLCommand::set_column_separator ( const Text& new_column_separator )
{
	if ( new_column_separator.GetSize() >= 1 ) {
		new_column_separator.GetUnicode ( &column_separator, 0, 1 );
	} else {
		column_separator = '\0';
	}
}



void BESQLCommand::set_row_separator ( const Text& new_row_separator )
{
	if ( new_row_separator.GetSize() >= 1 ) {
		new_row_separator.GetUnicode ( &row_separator, 0, 1 );
	} else {
		row_separator = '\0';
	}
}



bool BESQLCommand::is_ddl_command ( void ) const
{
	const TextUniquePtr alter;
	alter->Assign ( "ALTER", fmx::Text::kEncoding_UTF8 );

	const TextUniquePtr create;
	create->Assign ( "CREATE", fmx::Text::kEncoding_UTF8 );

	const TextUniquePtr drop;
	drop->Assign ( "DROP", fmx::Text::kEncoding_UTF8 );

	bool is_ddl = expression->FindIgnoringCase ( *alter, 0 ) == 0 ||
				expression->FindIgnoringCase ( *create, 0 ) == 0 ||
				expression->FindIgnoringCase ( *drop, 0 ) == 0;

	return is_ddl;

}
