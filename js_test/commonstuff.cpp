/**
 * @file commonstuff.cpp
 * @brief ...
 *
 * @author pansunyou@gmail.com
 * @version 1.0
 * @date 2010-11-22
*/

//#include <cstdlib>
//#include <cstdio>
//#include <iostream>
//#include <string>
//#include <sstream>
#include "stdafx.h"

#include <iostream>
#include <string>
#include <tchar.h>
#include <sstream>

#include "commonstuff.hpp"
#ifdef WIN32
#include <Windows.h>
#endif

using namespace v8;
using namespace std;


//#ifdef WIN32
//void ConvertGBKToUtf8(string& strGBK) {
//	int len=MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK.c_str(), -1, NULL,0);
//	unsigned short * wszUtf8 = new unsigned short[len+1];
//	memset(wszUtf8, 0, len * 2 + 2);
//	MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK.c_str(), -1, (LPWSTR)wszUtf8, len);
//
//	len = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)wszUtf8, -1, NULL, 0, NULL, NULL); 
//	char *szUtf8=new char[len + 1];
//	memset(szUtf8, 0, len + 1);
//	WideCharToMultiByte (CP_UTF8, 0, (LPWSTR)wszUtf8, -1, szUtf8, len, NULL,NULL);
//
//	strGBK = szUtf8;
//	delete[] szUtf8;
//	delete[] wszUtf8;
//}
//
//
//void ConvertUtf8ToGBK(string& strUtf8) {
//	int len=MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8.c_str(), -1, NULL,0);
//	unsigned short * wszGBK = new unsigned short[len+1];
//	memset(wszGBK, 0, len * 2 + 2);
//	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8.c_str(), -1, (LPWSTR)wszGBK, len);
//
//	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL); 
//	char *szGBK=new char[len + 1];
//	memset(szGBK, 0, len + 1);
//	WideCharToMultiByte (CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL,NULL);
//
//	strUtf8 = szGBK;
//	delete[] szGBK;
//	delete[] wszGBK;
//}
//#endif

/// 
/// @brief 在javascript中打印字符
/// 
JS_METHOD(_printfForJS)
{
	for (int i = 0; i < args.Length(); i++) {
		String::Utf8Value str(args[i]);
		string utf = (*str)?*str:"(null)";
		//ConvertUtf8ToGBK(utf);
		//printf("%s", utf.c_str());
		ATLTRACE("%s", utf.c_str());
	}
	return Undefined();
}

/// 
/// @brief 把公用函数都塞到tk空间
///
namespace tk {

/// 
/// @brief 读取文件
/// 
string getFileContents(string filePath)
{
	string retString;
	FILE *fp = fopen(filePath.c_str(), "rb");
	if (!fp)
		return retString;
	
	fseek(fp, 0, SEEK_END);
	int fileSize = ftell(fp);

	if (fileSize<=0)
	{
		fclose(fp);
		return retString;
	}

	char *buf = (char*)malloc(fileSize+1);
	memset(buf, 0x00, fileSize+1);

	rewind(fp);
	fread(buf, 1, fileSize, fp);
	retString = buf;

	free(buf);
	fclose(fp);
	return retString;
}

/// 
/// @brief 获取错误消息
/// 
std::string formatException(v8::TryCatch* try_catch) {
	v8::HandleScope handle_scope;
	v8::String::Utf8Value exception(try_catch->Exception());
	v8::Handle<v8::Message> message = try_catch->Message();
	std::string msgstring = "";
	std::stringstream ss;

	if (message.IsEmpty()) {
		msgstring += *exception;
	} else {
		v8::String::Utf8Value filename(message->GetScriptResourceName());
		int linenum = message->GetLineNumber();
		msgstring += *exception;
		msgstring += " (";
		msgstring += *filename;
		msgstring += ":";
		//ss << linenum;
		//msgstring += ss.str();
		msgstring += ")";
		
		v8::Handle<v8::Value> stack = try_catch->StackTrace();
		if (!stack.IsEmpty()) {
			v8::String::Utf8Value sstack(stack);
			msgstring += "\n";
			msgstring += *sstack;
		}
	}
	return msgstring;
}


/// 
/// @brief 打印错误消息
/// 
void printError(TryCatch* try_catch)
{
	string s = formatException(try_catch);
	//printf("Error: %s\n", s.c_str());
	ATLTRACE("Error: %s\n", s.c_str());
}

}