## BE_CurlSetOption

    BE_CurlSetOption ( { option ; value } )

**Description**  

Sets one of the Curl library optional variables to be used in all subsequent calls to the HTTP/FTP/SMTP functions.

The *value* depends on the type of *option*. Some require explicit values ( such as CURLOPT_USERAGENT ) and others require a boolean. 

The full list of options and their possible values is in the curl documentation : [http://curl.haxx.se/libcurl/c/curl_easy_setopt.html](http://curl.haxx.se/libcurl/c/curl_easy_setopt.html) 

**Parameters**

* *option* ( optional ) : The option name as text from the list below.
* *value* ( optional ) : The value to pass to the option.

* When the *value* parameter is left out then the *option* is cleared.
* When the *value* parameter is empty and the *option* supports an empty value the option will be set to the empty string.
* When no parameters are included or *option* is empty, then all values are cleared out and set back to defaults.

**Keywords**  

HTTP Curl Option Set

**Version History**

* 2.1.0 : First Release.
* 3.1.0 : Made the *option* value optional and added named constants.
* 4.0.2 : Renamed from BE_Curl_Set_Option.

**Notes**



**Compatibility** 

| Platform | Compatibility |
|-----------|-----------|
| Status | Active |  
| Mac FMP | Yes  |  
| Win FMP | Yes  |  
| FMS | Yes  |  
| iOS | Yes  |  
| Linux | Yes  |  

**Example Code**

Restoring all values to their default can be done with :

	BE_CurlSetOption

To return an option to it's default call the option name with no parameter :

	BE_CurlSetOption ( "CURLOPT_HTTPAUTH" )

The *CURLOPT_HTTPAUTH* and *CURLOPT_PROXYAUTH* options can also be set with these constants :

	CURLAUTH_BASIC
	CURLAUTH_DIGEST
	CURLAUTH_DIGEST_IE
	CURLAUTH_NEGOTIATE
	CURLAUTH_NTLM
	CURLAUTH_NTLM_WB
	CURLAUTH_ANY
	CURLAUTH_ANYSAFE
	CURLAUTH_ONLY

**Common use cases**

Follow redirects ( 301 response codes )

	BE_CurlSetOption ( "CURLOPT_FOLLOWLOCATION" ; True )

Use "Basic" Authentication

	BE_CurlSetOption ( "CURLOPT_HTTPAUTH" ; 1 ) 
	BE_CurlSetOption ( "CURLOPT_HTTPAUTH" ; "CURLAUTH_BASIC" )

**Certificate based Authentication**

To use Certificate based Authentication for HTTP function calls you would do :

	BE_CurlSetOption ( "CURLOPT_SSLCERT" ; "/path/to/cert.pem" )
	BE_CurlSetOption ( "CURLOPT_SSLCERTTYPE" ; "PEM" )
	BE_CurlSetOption ( "CURLOPT_SSLKEY" ; "/path/to/key.pem" )
	BE_CurlSetOption ( "CURLOPT_SSLKEYTYPE" ; "PEM" )

With certificates, "PEM" is the default type, so can be left out. Other possible options for the types are 

	PEM
	DER
	P12
	
P12 is for PKCS#12-encoded files. PEM files that will work with curl doing SFTP are **RSA PRIVATE KEY** files. You can open them in a text editor and you should see that the first line of the text has **RSA PRIVATE KEY** at the beginning. If it says **OPENSSH PRIVATE KEY** instead, it may not work. You can convert keys from one type to another using these instructions : 

[https://federicofr.wordpress.com/2019/01/02/how-to-convert-openssh-private-keys-to-rsa-pem/](https://federicofr.wordpress.com/2019/01/02/how-to-convert-openssh-private-keys-to-rsa-pem/) 

**Using Cookies**

To allow curl to store and re-use cookies, set the cookie jar ( for where to store them ) and the cookie file ( for where to read them to send ).

	BE_CurlSetOption ( "CURLOPT_COOKIEFILE" ; "/path/to/cookieFile.txt" )
	BE_CurlSetOption ( "CURLOPT_COOKIEJAR" ; "/path/to/cookieFile.txt" )

You can use the temp folder for storing the cookie files, but make sure you're using a plugin path and not a FileMaker path created from any of the *Get* function calls.

**Amazon Authentication**

There's now a native option for the complex AWS authentication method using the **CURLOPT_AWS_SIGV4** option. It appears to require a string matching either "provider1:provider2" or "provider1:provider2:region:service" and you set the keys via the option values *CURLOPT_USERPWD* and "MY_ACCESS_KEY:MY_SECRET_KEY". See here for more detail :

[https://curl.se/libcurl/c/CURLOPT_AWS_SIGV4.html](https://curl.se/libcurl/c/CURLOPT_AWS_SIGV4.html) 

Complete List of supported CURL options :

	CURLOPT_PROXY
	CURLOPT_NOPROXY
	CURLOPT_SOCKS5_GSSAPI_SERVICE
	CURLOPT_INTERFACE
	CURLOPT_NETRC_FILE
	CURLOPT_USERPWD
	CURLOPT_PROXYUSERPWD
	CURLOPT_USERNAME
	CURLOPT_PASSWORD
	CURLOPT_PROXYUSERNAME
	CURLOPT_PROXYPASSWORD
	CURLOPT_TLSAUTH_USERNAME
	CURLOPT_TLSAUTH_PASSWORD
	CURLOPT_ACCEPT_ENCODING
	CURLOPT_COPYPOSTFIELDS
	CURLOPT_REFERER
	CURLOPT_USERAGENT
	CURLOPT_COOKIE
	CURLOPT_COOKIEFILE
	CURLOPT_COOKIEJAR
	CURLOPT_COOKIELIST
	CURLOPT_HTTPGET
	CURLOPT_MAIL_FROM
	CURLOPT_MAIL_AUTH
	CURLOPT_FTPPORT
	CURLOPT_FTP_ALTERNATIVE_TO_USER
	CURLOPT_FTP_ACCOUNT
	CURLOPT_RTSP_SESSION_ID
	CURLOPT_RTSP_STREAM_URI
	CURLOPT_RTSP_TRANSPORT
	CURLOPT_RANGE
	CURLOPT_CUSTOMREQUEST
	CURLOPT_DNS_SERVERS
	CURLOPT_SSLCERT
	CURLOPT_SSLCERTTYPE
	CURLOPT_SSLKEY
	CURLOPT_SSLKEYTYPE
	CURLOPT_KEYPASSWD
	CURLOPT_SSLENGINE
	CURLOPT_CAINFO
	CURLOPT_ISSUERCERT
	CURLOPT_CAPATH
	CURLOPT_CRLFILE
	CURLOPT_RANDOM_FILE
	CURLOPT_EGDSOCKET
	CURLOPT_SSL_CIPHER_LIST
	CURLOPT_KRBLEVEL
	CURLOPT_SSH_HOST_PUBLIC_KEY_MD5
	CURLOPT_SSH_PUBLIC_KEYFILE
	CURLOPT_SSH_PRIVATE_KEYFILE
	CURLOPT_SSH_KNOWNHOSTS
	CURLOPT_VERBOSE
	CURLOPT_HEADER
	CURLOPT_NOSIGNAL
	CURLOPT_WILDCARDMATCH
	CURLOPT_FAILONERROR
	CURLOPT_PROXYPORT
	CURLOPT_PROXYTYPE
	CURLOPT_HTTPPROXYTUNNEL
	CURLOPT_SOCKS5_GSSAPI_NEC
	CURLOPT_LOCALPORT
	CURLOPT_LOCALPORTRANGE
	CURLOPT_DNS_CACHE_TIMEOUT // CURLOPT_DNS_USE_GLOBAL_CACHE - removed in 4.2.0
	CURLOPT_BUFFERSIZE
	CURLOPT_PORT
	CURLOPT_TCP_NODELAY
	CURLOPT_ADDRESS_SCOPE
	CURLOPT_TCP_KEEPALIVE
	CURLOPT_TCP_KEEPIDLE
	CURLOPT_TCP_KEEPINTVL
	CURLOPT_NETRC
	CURLOPT_HTTPAUTH
	CURLOPT_TLSAUTH_TYPE
	CURLOPT_PROXYAUTH
	CURLOPT_AUTOREFERER
	CURLOPT_TRANSFER_ENCODING
	CURLOPT_FOLLOWLOCATION
	CURLOPT_UNRESTRICTED_AUTH
	CURLOPT_MAXREDIRS
	CURLOPT_PUT
	CURLOPT_POST
	CURLOPT_POSTFIELDSIZE
	CURLOPT_COOKIESESSION
	CURLOPT_HTTP_VERSION
	CURLOPT_IGNORE_CONTENT_LENGTH
	CURLOPT_HTTP_CONTENT_DECODING
	CURLOPT_HTTP_TRANSFER_DECODING
	CURLOPT_TFTP_BLKSIZE
	CURLOPT_FTPPORT
	CURLOPT_DIRLISTONLY
	CURLOPT_APPEND
	CURLOPT_FTP_USE_EPRT
	CURLOPT_FTP_USE_EPSV
	CURLOPT_FTP_USE_PRET
	CURLOPT_FTP_CREATE_MISSING_DIRS
	CURLOPT_FTP_RESPONSE_TIMEOUT
	CURLOPT_FTP_SKIP_PASV_IP
	CURLOPT_FTPSSLAUTH
	CURLOPT_FTP_SSL_CCC
	CURLOPT_FTP_FILEMETHOD
	CURLOPT_RTSP_REQUEST
	CURLOPT_RTSP_CLIENT_CSEQ
	CURLOPT_RTSP_SERVER_CSEQ
	CURLOPT_TRANSFERTEXT
	CURLOPT_PROXY_TRANSFER_MODE
	CURLOPT_CRLF
	CURLOPT_RESUME_FROM
	CURLOPT_FILETIME
	CURLOPT_NOBODY
	CURLOPT_INFILESIZE
	CURLOPT_UPLOAD
	CURLOPT_MAXFILESIZE
	CURLOPT_TIMECONDITION
	CURLOPT_TIMEVALUE
	CURLOPT_TIMEOUT
	CURLOPT_TIMEOUT_MS
	CURLOPT_LOW_SPEED_TIME
	CURLOPT_MAXCONNECTS
	CURLOPT_FRESH_CONNECT
	CURLOPT_FORBID_REUSE
	CURLOPT_CONNECTTIMEOUT
	CURLOPT_CONNECTTIMEOUT_MS
	CURLOPT_IPRESOLVE
	CURLOPT_CONNECT_ONLY
	CURLOPT_USE_SSL
	CURLOPT_ACCEPTTIMEOUT_MS
	CURLOPT_SSLENGINE_DEFAULT
	CURLOPT_SSLVERSION
	CURLOPT_SSL_VERIFYPEER
	CURLOPT_SSL_VERIFYHOST
	CURLOPT_CERTINFO
	CURLOPT_SSL_SESSIONID_CACHE
	CURLOPT_GSSAPI_DELEGATION
	CURLOPT_SSH_AUTH_TYPES
	CURLOPT_NEW_FILE_PERMS
	CURLOPT_NEW_DIRECTORY_PERMS
	CURLOPT_POSTFIELDSIZE_LARGE
	CURLOPT_RESUME_FROM_LARGE
	CURLOPT_INFILESIZE_LARGE
	CURLOPT_MAXFILESIZE_LARGE
	CURLOPT_MAX_SEND_SPEED_LARGE
	CURLOPT_MAX_RECV_SPEED_LARGE

Added in 4.2.0 :

	CURLOPT_CONNECT_TO
	CURLOPT_TCP_FASTOPEN
	CURLOPT_KEEP_SENDING_ON_ERROR
	CURLOPT_SUPPRESS_CONNECT_HEADERS
	CURLOPT_SOCKS5_AUTH
	CURLOPT_REQUEST_TARGET
	CURLOPT_SSH_COMPRESSION
	CURLOPT_HAPPY_EYEBALLS_TIMEOUT_MS
	CURLOPT_DNS_SHUFFLE_ADDRESSES
	CURLOPT_HAPROXYPROTOCOL
	CURLOPT_DISALLOW_USERNAME_IN_URL
	CURLOPT_TLS13_CIPHERS
	CURLOPT_PROXY_TLS13_CIPHERS
	CURLOPT_UPLOAD_BUFFERSIZE
	CURLOPT_DOH_URL
	CURLOPT_MAXAGE_CONN
	CURLOPT_MAIL_RCPT_ALLLOWFAILS
	CURLOPT_PROXY_ISSUERCERT
	CURLOPT_AWS_SIGV4




