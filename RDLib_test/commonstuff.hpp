/**
 * @file commonstuff.hpp
 * @brief ...
 *
 * @author pansunyou@gmail.com
 * @version 1.0
 * @date 2010-11-22
*/


#ifndef __commonstuff_hpp__
#define __commonstuff_hpp__

#include "stdafx.h"
#include "macros.h"

//#include <v8.h>
//#include <string>
//#include <iostream>
//#include "macros.h"
//
//using namespace v8;
//using namespace std;

/// 
/// @brief ��javascript�д�ӡ�ַ�
/// 
JS_METHOD(_printfForJS);


namespace tk {

/// 
/// @brief ��ȡ�ļ�
/// 
string getFileContents(string filePath);

/// 
/// @brief ��ȡ������Ϣ
/// 
string formatException(TryCatch* try_catch);

/// 
/// @brief ��ӡ������Ϣ
/// 
void printError(TryCatch* try_catch);


}


inline v8::Handle<v8::Array> JS_CHARARRAY(char * data, int count) {
	v8::Handle<v8::Array> arr = v8::Array::New(count);
	char ch;
	for (int i=0;i<count;i++) {
		ch = data[i];
		arr->Set(v8::Integer::New(i), v8::Integer::New((int)((unsigned char)data[i])));
	}
	return arr;
}

#endif
