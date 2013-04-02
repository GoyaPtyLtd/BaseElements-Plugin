/*
 *  BESQL.h
 *  BaseElements
 *
 *  Created by Mark Banks on 28/11/11.
 *  Copyright 2011 Goya. All rights reserved.
 *
 */


#if !defined(BESQLCOMMAND_H)
#define BESQLCOMMAND_H

#include "BEPluginGlobalDefines.h"

#include <string>


class BESQLCommand
{
	
public:
	
	BESQLCommand ( const fmx::TextAutoPtr expression, const fmx::TextAutoPtr filename );
	
	void execute ( void );
	void execute ( const fmx::ExprEnv& _environment );
	fmx::TextAutoPtr get_text_result ( void );
	
	void set_column_separator ( const fmx::TextAutoPtr new_column_separator );
	void set_row_separator ( const fmx::TextAutoPtr new_row_separator );
	
protected:
	
	fmx::TextAutoPtr expression;
	fmx::TextAutoPtr filename;
	fmx::DataVectAutoPtr parameters;
	fmx::RowVectAutoPtr result;
	
	fmx::TextAutoPtr column_separator;
	fmx::TextAutoPtr row_separator;
	
	bool is_ddl_command ( void ) const;
	
	bool waiting;
	
};


typedef std::auto_ptr<BESQLCommand> BESQLCommandAutoPtr;


#endif // BESQLCOMMAND_H

