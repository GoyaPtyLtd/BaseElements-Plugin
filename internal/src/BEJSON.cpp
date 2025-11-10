/*
 BEJSON.cpp
 BaseElements Plug-In

 Copyright 2022 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#include "BEJSON.h"
#include "BEPluginUtilities.h"

#include <jq/jq.h>
#include <jq/jv.h>
#include <jq/version.h>


enum {
  SLURP                 = 1,
  RAW_INPUT             = 2,
  PROVIDE_NULL          = 4,
  RAW_OUTPUT            = 8,
  ASCII_OUTPUT          = 32,
  COLOR_OUTPUT          = 64,
  NO_COLOR_OUTPUT       = 128,
  SORTED_OUTPUT         = 256,
  FROM_FILE             = 512,
  RAW_NO_LF             = 1024,
  UNBUFFERED_OUTPUT     = 2048,
  EXIT_STATUS           = 4096,
  EXIT_STATUS_EXACT     = 8192,
  SEQ                   = 16384,
  RUN_TESTS             = 32768,
  /* debugging only */
  DUMP_DISASM           = 65536,
};


const std::string JSON_jq ( const std::string& jq_json,  const std::string& jq_filter,  const std::string& jq_options )
{
	std::string filter_result;

	std::size_t found = (jq_options.find ( "V" ) != std::string::npos) || (jq_options.find ( "--version" ) != std::string::npos);
	if ( found ) {
		filter_result = "jq-" + std::string ( JQ_VERSION ) + LINE_FEED;
	} else {
		
		auto jq = jq_init();
		if ( jq != NULL ) {

			const int compiled = jq_compile ( jq, jq_filter.c_str() );
			if ( compiled ) {

				auto dumpopts = JV_PRINT_INDENT_FLAGS(2) | JV_PRINT_ASCII;
				auto options = 0;

				found = jq_options.find ( "c" );
				if ( found != std::string::npos ) {
					dumpopts &= ~(JV_PRINT_TAB | JV_PRINT_INDENT_FLAGS(7));
				}
				found = jq_options.find ( "j" );
				if ( found != std::string::npos ) {
					options |= RAW_OUTPUT | RAW_NO_LF;
				}
				found = jq_options.find ( "r" );
				if ( found != std::string::npos ) {
					options |= RAW_OUTPUT;
				}

				jv filter = jv_parse_sized ( jq_json.c_str(), (int)jq_json.length() );
				if ( jv_is_valid ( jv_copy ( filter ) ) ) {

					jq_start ( jq, filter, 0 );

					jv result;
					while ( jv_is_valid ( result = jq_next ( jq ) ) ) {

						if ( options & RAW_OUTPUT && jv_get_kind ( jv_copy ( result ) ) == JV_KIND_STRING ) {
							filter_result.append ( jv_string_value ( jv_copy ( result ) ) );
						} else {

							const auto dumped = jv_dump_string ( jv_copy ( result ), dumpopts );
							if ( jv_is_valid ( dumped ) ) {
								filter_result.append ( jv_string_value ( dumped ) );
							}
							jv_free ( dumped );

						}

						if ( !(options & RAW_NO_LF) ) {
							filter_result.append ( LINE_FEED );
						}

					} // while

					if ( jv_invalid_has_msg ( jv_copy ( result ) ) ) {

						const auto error_message = jv_invalid_get_msg ( jv_copy ( result ) );
						filter_result = jv_string_value ( jv_copy ( error_message ) );
						jv_free ( error_message );

					} else {
						jv_free ( result );
					}

				} else {
					
					filter_result.append ( "parse error: " );
					const auto error_message = jv_invalid_get_msg ( jv_copy ( filter ) );
					filter_result.append ( jv_string_value ( jv_copy ( error_message ) ) );
					jv_free ( error_message );

				}

				jv_free ( filter );

			} else {
				filter_result.append ( "compile error" );
			}

			jq_teardown ( &jq );

		} else {
			filter_result = strerror ( errno );
		}

	}


	return filter_result;

}

