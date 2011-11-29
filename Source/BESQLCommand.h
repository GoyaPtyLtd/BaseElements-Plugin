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

#include "FMWrapper/FMXExtern.h"
#include "FMWrapper/FMXTypes.h"
#include "FMWrapper/FMXText.h"
#include "FMWrapper/FMXFixPt.h"
#include "FMWrapper/FMXData.h"
#include "FMWrapper/FMXBinaryData.h"
#include "FMWrapper/FMXCalcEngine.h"

#include <string>


using namespace std;
using namespace fmx;


class BESQLCommand
{
	
public:
	
	BESQLCommand ( const TextAutoPtr expression, const TextAutoPtr filename );
	
	void execute ( const ExprEnv& _environment );
	void execute ( const bool needs_environment );
	TextAutoPtr get_text_result ( void );
	
	void set_column_separator ( const TextAutoPtr new_column_separator );
	void set_row_separator ( const TextAutoPtr new_row_separator );
	
protected:
	
	TextAutoPtr expression;
	TextAutoPtr filename;
	DataVectAutoPtr parameters;
	RowVectAutoPtr result;
	
	TextAutoPtr column_separator;
	TextAutoPtr row_separator;
	
	bool is_ddl_command ( void ) const;
	
	bool waiting;
	
};


typedef auto_ptr<BESQLCommand> BESQLCommandAutoPtr;


#endif // BESQLCOMMAND_H

