/*
 BESMTPContainerAttachments.h
 BaseElements Plug-In
 
 Copyright 2016~2024 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp
 
 http://www.goya.com.au/baseelements/plugin
 
 */


#ifndef BESMTPContainerAttachments_hpp
#define BESMTPContainerAttachments_hpp


#include <vector>

#include <filesystem>


typedef std::pair<std::filesystem::path, std::string> BESMTPContainerAttachment;
typedef std::vector<BESMTPContainerAttachment> BESMTPContainerAttachmentVector;


#define BE_DEFAULT_SMTP_CONTENT_TYPE "application/octet-stream"


class BESMTPContainerAttachments {
	
	
public:
	
	BESMTPContainerAttachments ( void ) {};
	~BESMTPContainerAttachments();
	
	void add ( const std::filesystem::path path, const std::string content_type = BE_DEFAULT_SMTP_CONTENT_TYPE );
	void add ( const std::string file_name, const std::vector<char>& data, const std::string content_type = BE_DEFAULT_SMTP_CONTENT_TYPE );
	BESMTPContainerAttachmentVector get_file_list ( void ) { return files; };
	void clear ( void );
	
private:
	
	BESMTPContainerAttachmentVector files;
	
	const std::string temporary_attachments_directory ( void );

};


#endif /* BESMTPContainerAttachment_hpp */
