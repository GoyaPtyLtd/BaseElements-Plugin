/*
 BESMTPEmailMessage.h
 BaseElements Plug-In

 Copyright 2014-2022 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

 http://www.goya.com.au/baseelements/plugin

 */


#ifndef __BaseElements__BESMTPEmailMessage__
#define __BaseElements__BESMTPEmailMessage__


#include "BEPluginGlobalDefines.h"
#include "BEValueList.h"
#include "BESMTPContainerAttachments.h"

#include <string>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#include <Poco/Net/MailMessage.h>
#pragma GCC diagnostic pop


class BESMTPEmailMessage {

public:

	BESMTPEmailMessage ( const std::string& from, BEValueListStringSharedPtr to, const std::string& subject, const std::string& message_body, const std::string& html_alternative = "" );
	~BESMTPEmailMessage ( );

	const std::string from_address() { return message.getSender(); };
	BEValueListStringUniquePtr recipients();
	
	void set_cc_addresses ( const BEValueListStringSharedPtr email_addresses );
	void set_bcc_addresses ( const BEValueListStringSharedPtr email_addresses );
	void set_reply_to ( const std::string& reply_to_address );
	void set_attachments ( const BESMTPContainerAttachmentVector& attachments );

	const std::string as_string();


protected:

	Poco::Net::MailMessage message;
	std::string text;
	std::string html;
	BESMTPContainerAttachmentVector attachments;

	void set_addresses ( const Poco::Net::MailRecipient::RecipientType recipient_type, const BEValueListStringSharedPtr email_addresses );
	void add_attachments();
	
};


#endif /* defined(__BaseElements__BESMTPEmailMessage__) */
