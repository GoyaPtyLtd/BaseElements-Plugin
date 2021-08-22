/*
 BELinuxFunctionStrings.h
 BaseElements Plug-In

 Copyright 2019 Goya. All rights reserved.
 For conditions of distribution and use please see the copyright notice in BEPlugin.cpp

   http://www.goya.com.au/baseelements/plugin
 
 IMPORTANT: this file is automatically generated! Do not edit by hand.

 */


#if !defined(BELINUXFUNCTIONSTRINGS_H)
	#define BELINUXFUNCTIONSTRINGS_H

#include "BEPluginGlobalDefines.h"

#include <map>
#include <string>

const std::map<unsigned long, std::string> function_strings = {

	{ 1, "GyBE1nnYYnn" },
	{ 102, "BE_Version" },
	{ 103, "BE_VersionAutoUpdate" },
	{ 104, "BE_ClipboardFormats" },
	{ 105, "BE_ClipboardGetText ( format )" },
	{ 106, "BE_ClipboardSetText ( text ; format )" },
	{ 107, "BE_FolderCreate ( path )" },
	{ 108, "BE_FileDelete ( path )" },
	{ 109, "BE_FileExists ( path )" },
	{ 110, "BE_FileReadText ( pathOrContainer {; start; to; eolChar } )" },
	{ 111, "BE_FileWriteText ( pathOrContainer ; text {; appendBoolean } )" },
	{ 112, "BE_FileSelectDialog ( prompt {; inFolderPath } )" },
	{ 113, "BE_FolderSelectDialog ( prompt {; inFolderPath } )" },
	{ 114, "BE_DialogDisplay ( title ; message ; defaultButton {; cancelButton ; alternateButton } )" },
	{ 115, "BE_XSLTApply ( xmlFilePath ; xsltText ; outputFilePath )" },
	{ 116, "BE_TextExtractWords ( text { ; wordPrefix } )" },
	{ 117, "BE_XMLStripInvalidCharacters ( path ; { resultFilePath } )" },
	{ 118, "BE_FileMove ( fromFilePath ; toFilePath )" },
	{ 119, "BE_FileCopy ( fromFilePath ; toFilePath )" },
	{ 121, "BE_FileListFolder ( path {; type ; includeSubdirBoolean ; useFullPathBoolean ; includeHiddenBoolean } )" },
	{ 124, "BE_OpenURL ( url )" },
	{ 125, "BE_FileOpen ( path )" },
	{ 126, "BE_FileSize ( path )" },
	{ 127, "BE_FileModificationTimestamp ( path )" },
	{ 128, "BaseElements" },
	{ 131, "GyBE1nnYYnn" },
	{ 132, "https://docs.baseelementsplugin.com/" },
	{ 134, "BE_DebugInformation" },
	{ 135, "BE_ClipboardGetFile ( format {; filename } )" },
	{ 136, "BE_ClipboardSetFile ( fileData ; format )" },
	{ 137, "BE_GetSystemDrive" },
	{ 140, "BE_WriteTextFileToContainer_Deprecated ( filename ; text {; append} )" },
	{ 150, "BE_ScriptExecute ( scriptName {; fileName ; parameter ; scriptControl } )" },
	{ 151, "BE_FileMakerSQL ( sqlStatement {; columnSeparator ; rowSeparator ; databaseName ; asText ; outputPath } )" },
	{ 152, "BE_HTTP_GET ( url {; filename ; username ; password } )" },
	{ 153, "BE_GetLastError" },
	{ 154, "BE_MessageDigest ( text {; algorithm ; encoding } )" },
	{ 155, "BE_GetLastDDLError" },
	{ 156, "BE_HTTP_GETFile ( url ; path {; username ; password } )" },
	{ 157, "BE_DialogProgress ( title ; description {; maximum } )" },
	{ 158, "BE_DialogProgressUpdate ( number {; description } )" },
	{ 159, "BE_Pause ( milliseconds )" },
	{ 160, "BE_PreferenceSet ( key ; value {; domain } )" },
	{ 161, "BE_PreferenceGet ( key {; domain } )" },
	{ 162, "BE_PreferenceDelete ( key {; domain } )" },
	{ 170, "BE_Unzip ( archiveFilePath {; outputFolderPath } )" },
	{ 171, "BE_Zip ( filePathList {; archiveFilePath } )" },
	{ 174, "BE_SetTextEncoding ( { encoding } )" },
	{ 176, "BE_ExportFieldContents ( field {; outputPath } )" },
	{ 177, "BE_FileImport ( filePath {; compressBoolean } )" },
	{ 178, "BE_FilePatternCount ( path ; searchText )" },
	{ 179, "BE_FileReplaceText ( pathOrContainer ; expression ; replaceString {; options } )" },
	{ 180, "BE_HTTP_POST ( url ; parameters {; username ; password ; filename } )" },
	{ 181, "BE_HTTP_ResponseCode" },
	{ 182, "BE_HTTP_ResponseHeaders ( { header } )" },
	{ 183, "BE_HTTP_SetCustomHeader ( { header ; value } )" },
	{ 184, "BE_HTTP_DELETE ( url {; username ; password } )" },
	{ 185, "BE_HTTP_PUTFile ( url ; path {; username ; password } )" },
	{ 186, "BE_HTTP_SetProxy ( proxy {; port ; username ; password } )" },
	{ 187, "BE_CurlSetOption ( { option ; value } )" },
	{ 188, "BE_HTTP_PUTData ( url ; data {; username ; password } )" },
	{ 189, "BE_FTP_Upload ( url ; data {; username ; password } )" },
	{ 190, "BE_CurlTrace" },
	{ 191, "BE_FTP_Delete ( url {; username ; password } )" },
	{ 192, "BE_HTTP_PATCH ( url ; parameters {; username ; password } )" },
	{ 193, "BE_FTP_UploadFile ( url ; pathToFile {; username ; password } )" },
	{ 200, "BE_XSLT_ApplyInMemory ( xmlText ; xsltText )" },
	{ 201, "BE_XPath ( xmlText ; xpathText {; namespaceListText ; asTextBoolean } )" },
	{ 202, "BE_XPathAll ( xmlText ; xpathText {; namespaceListText } )" },
	{ 210, "BE_XMLStripNodes ( inputFilePath ; outputFilePath ; nodeNames )" },
	{ 211, "BE_XMLParse ( pathOrXMLText )" },
	{ 212, "BE_SplitBEFileNodes ( path )" },
	{ 213, "BE_XMLValidate ( xmlFilePath ; schema )" },
	{ 214, "BE_XMLCanonical ( xmlText )" },
	{ 215, "BE_XMLTidy ( xml )" },
	{ 220, "BE_TimeCurrentMilliseconds" },
	{ 221, "BE_TimeUTCMilliseconds" },
	{ 222, "BE_TimeZoneOffset" },
	{ 230, "BE_FileSaveDialog ( prompt {; filename ; inFolder } )" },
	{ 240, "BE_RegularExpression ( text ; expression {; options ; replaceString } )" },
	{ 250, "BE_ContainerIsCompressed ( containerField )" },
	{ 251, "BE_ContainerCompress ( data {; filename } )" },
	{ 252, "BE_ContainerUncompress ( gzip_data {; filename } )" },
	{ 253, "BE_ContainerListTypes ( container )" },
	{ 254, "BE_ContainerGetType ( container ; type )" },
	{ 260, "BE_Gzip ( data {; filename } )" },
	{ 261, "BE_UnGzip ( gzip_data {; filename } )" },
	{ 271, "BE_JPEGRecompress ( jpeg {; compressionLevel ; scale } )" },
	{ 300, "BE_JSONPath_Deprecated ( json ; path )" },
	{ 301, "BE_JSON_Error_Description_Deprecated" },
	{ 302, "BE_JSON_ArraySize ( json {; path } )" },
	{ 350, "BE_ValuesUnique ( listOfValues {; caseSensitiveBoolean } )" },
	{ 351, "BE_ValuesFilterOut ( textToFilter ; filterValues {; caseSensitiveBoolean } )" },
	{ 352, "BE_ValuesContainsDuplicates ( listOfValues  {; caseSensitiveBoolean } )" },
	{ 353, "BE_ValuesSort ( listOfValues {; ascendingBoolean ; type } )" },
	{ 354, "BE_ValuesTimesDuplicated ( listOfValues ; numberOfTimes )" },
	{ 355, "BE_ValuesTrim ( listOfValues )" },
	{ 400, "BE_SMTPServer ( server { ; port ; username ; password } )" },
	{ 401, "BE_SMTPSend ( from ; to ; subject ; text { ; cc ; bcc ; replyTo ; html ; attachments } )" },
	{ 402, "BE_SMTPAddAttachment ( { attachment ; contentType } )" },
	{ 403, "BE_SMTPSetHeader ( { header ; value } )" },
	{ 470, "BE_VariableSet ( name ; { value } )" },
	{ 471, "BE_VariableGet ( name )" },
	{ 472, "BE_StackPush ( name ; value )" },
	{ 473, "BE_StackPop ( name )" },
	{ 474, "BE_StackCount ( name )" },
	{ 475, "BE_StackDelete ( name )" },
	{ 500, "BE_EncryptAES ( key ; text )" },
	{ 501, "BE_DecryptAES ( key ; text )" },
	{ 502, "BE_SignatureGenerateRSA ( data ; privateKey {; privateKeyPassword ; algorithm; fileNameWithExtension } )" },
	{ 503, "BE_SignatureVerifyRSA ( data ; publicKey ; signature {; algorithm } )" },
	{ 504, "BE_CipherEncrypt ( cipher ; data ; key ; iv {; paddingBoolean ; filenameWithExtension } )" },
	{ 505, "BE_CipherDecrypt ( cipher ; encryptedData ; key ; iv {; paddingBoolean ; filenameWithExtension } )" },
	{ 600, "BE_EvaluateJavaScript ( javaScript )" },
	{ 650, "BE_ArraySetFromValueList ( valueList {; retainEmptyBoolean } )" },
	{ 651, "BE_ArrayGetSize ( array )" },
	{ 652, "BE_ArrayGetValue ( array ; valueNumber )" },
	{ 653, "BE_ArrayDelete ( array )" },
	{ 654, "BE_ArrayFind ( array ; value )" },
	{ 655, "BE_ArrayChangeValue ( array ; valueNumber ; newValue )" },
	{ 700, "BE_ConvertContainer ( field {; type } )" },
	{ 750, "BE_PDFAppend ( pdfPathOrContainer ; appendPathOrContainer {; destinationPath } )" },
	{ 751, "BE_PDFPageCount ( pdfPathOrContainer )" },
	{ 752, "BE_PDFGetPages ( pdfPathOrContainer ; newPDFPath ; fromPageNum {; toPageNum } )" },
	{ 800, "BE_XOR ( text ; xorWith )" },
	{ 850, "BE_ScriptStepInstall ( name ; definitionXML ; id ; description ; calculation )" },
	{ 851, "BE_ScriptStepRemove ( scriptStepId )" },
	{ 852, "BE_ScriptStepPerform ( scriptStepId )" },
	{ 900, "BE_VectorDotProduct ( a ; b )" },
	{ 901, "BE_VectorEuclideanDistance ( a ; b )" },
	{ 950, "BE_GetMachineName" },
	{ 1001, "BE_ButtonOK" },
	{ 1002, "BE_ButtonCancel" },
	{ 1003, "BE_ButtonAlternate" },
	{ 1120, "BE_ExecuteSystemCommand ( command { ; timeout ; executeUsingShell } )" },
	{ 2001, "BE_MessageDigestAlgorithmMD5" },
	{ 2002, "BE_MessageDigestAlgorithmSHA256" },
	{ 2004, "BE_MessageDigestAlgorithmMDC2" },
	{ 2006, "BE_MessageDigestAlgorithmSHA" },
	{ 2007, "BE_MessageDigestAlgorithmSHA1" },
	{ 2008, "BE_MessageDigestAlgorithmSHA224" },
	{ 2009, "BE_MessageDigestAlgorithmSHA384" },
	{ 2010, "BE_MessageDigestAlgorithmSHA512" },
	{ 3000, "BE_FileTypeAll" },
	{ 3001, "BE_FileTypeFile" },
	{ 3002, "BE_FileTypeFolder" },
	{ 4001, "BE_EncodingHex" },
	{ 4002, "BE_EncodingBase64" },
	{ 65000, "Version: %@\n\nThis plug-in provides additional functionality for BaseElements from Goya." },

};

#endif // BELINUXFUNCTIONSTRINGS_H

