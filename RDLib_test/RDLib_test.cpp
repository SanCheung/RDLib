// RDLib_test.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "RDLib_test.h"
#include "MainDlg.h"
#include "DlgTestList.h"
#include "SwhMainLayout.h"
#include "DlgNetTest.h"


//#include "commonstuff.hpp"
//#include "JsEngine.h"
//
////#include "./cproxyv8/cproxyv8-class.h"
//
////using namespace v8;
////using namespace std;
////
////仅供测试
//
///*
//class CTestObj {
//public:
//	CTestObj(){
//		ATLTRACE("CTestObj::CTestObj\n");
//		buffer = (char*)malloc(1024*1024);
//	}
//	~CTestObj(){
//		ATLTRACE("CTestObj::~CTestObj\n");
//		if(buffer){
//			free(buffer);
//			buffer = NULL;
//		}
//	}
//
//	void printfName(){
//		ATLTRACE("CTestObj::printfName name=%s\n", name.c_str());
//	}
//
//	void setName(const char*xname){
//		name = xname;
//	}
//
//	string name;
//	char *buffer;
//};
//
////此函数用来做为TestObj的"构造函数"
////在 var obj = new TestObj; 的时候调用
//JS_METHOD(_testObjConstructor)
//{
//	ASSERT_CONSTRUCTOR;
//	SAVE_PTR(0, new CTestObj());
//	return args.This();
//}
//
////只是个包装
//JS_METHOD(_testObjPrintName)
//{
//	CTestObj *pObj = LOAD_PTR(0, CTestObj *);
//	if(pObj!=NULL){
//		pObj->printfName();
//	}
//	return args.This();
//}
//
////只是个包装
//JS_METHOD(_testObjSetName)
//{
//	CTestObj *pObj = LOAD_PTR(0, CTestObj *);
//	if(pObj!=NULL){
//		String::AsciiValue xname(args[0]);
//		pObj->setName(*xname);
//	}
//	return args.This();
//}
//
////用来显式地释放内部数据
////obj.dispose();
//JS_METHOD(_testObjDispose)
//{
//	CTestObj *pObj = LOAD_PTR(0, CTestObj *);
//	if(pObj!=NULL){
//		delete pObj;
//		pObj = NULL;
//		SAVE_PTR(0, pObj);
//	}
//	return args.This();
//}
////*/
//
//namespace v8class
//{
//	
//class CItem
//{
//public:
//	CItem()	{}
//	~CItem(){}
//
//	CStringW	_name;
//	CStringW	_title;
//};
//
//JS_METHOD(_ItemConstructor)
//{
//	ASSERT_CONSTRUCTOR;
//	SAVE_PTR(0, new CItem());
//	return args.This();
//}
//
//
//JS_METHOD(_ItemSetName)
//{
//	CItem *item = LOAD_PTR(0, CItem *);
//	if( item!=NULL ){
//		v8::String::AsciiValue xname(args[0]);
//		item->_name.Empty();
//		item->_name = *xname;
//
//		ATLTRACE( L"CItem setName name=%s\n", item->_name);
//	}
//	return args.This();
//}
//
//JS_METHOD(_ItemGetName)
//{
//	CItem *item = LOAD_PTR(0, CItem *);
//	if( item!=NULL ){
//		//v8::String::AsciiValue xname(args[0]);
//		//v8::Value item->_name = *xname;
//
//		ATLTRACE( L"CItem GetName %s\n", item->_name);
//
//		CStringA	strA = item->_name;
//		char *str = (char *)strA;
//
//		//int n = strlen(str);
//		//char *data = new char[n+1];
//		//strcpy( data, str );
//		return v8::String::New( str );
//
//		//return v8::Undefined();
//	}
//	return v8::Undefined();
//}
//
//
//
//
//
//
//JS_METHOD(_ItemPrintName)
//{
//	Local<Object>	pT = args.This();
//
//	CItem *item = LOAD_PTR(0, CItem *);
//	if( item!=NULL ){
//		ATLTRACE( L"CItem printName name=%s\n", item->_name);
//
//		Local<Value>	value = pT->Get(String::New( "OnInfo" ));
//		if( !value.IsEmpty() && value->IsFunction() )
//		{
//			Local<Function> fun = Local<Function>::Cast( value );
//
//			Handle<Value> args[1];
//			args[0] = Uint32::New( 111111111 );
//			fun->Call( pT, 1, args );
//		}
//	}
//	return args.This();
//}
//
//
//static Handle<Value> xxxGetter( Local<String> name, const AccessorInfo& info)
//{ 
//	CItem *item = reinterpret_cast<CItem*>(info.This()->GetPointerFromInternalField(0));
//	if( item!=NULL )
//	{
//		CStringA	strA = item->_title;
//		char *str = (char *)strA;
//		return v8::String::New( str );
//	}
//	return v8::Undefined();
//} 
//
//static void xxxSetter( Local<String> name, Local<Value> value, const AccessorInfo& info)
//{ 
//	CItem *item = reinterpret_cast<CItem*>(info.This()->GetPointerFromInternalField(0));
//	if( item!=NULL ){
//		v8::String::AsciiValue xname(value);
//		item->_title.Empty();
//		item->_title = *xname;
//
//		ATLTRACE( L"CItem setTitle title=%s\n", item->_title);
//	}
//}
//
//
//
///*
///// 
///// @brief 在javascript环境中构造一个class TestObj
///// 
///// @confused v8的垃圾回收机制还没弄明白, 这里的TestObj必须显式释放: obj.dispose();
/////
//Handle<v8::FunctionTemplate> pubishCppClass_TestObj()
//{
//	v8::HandleScope handle_scope;
//
//	//指定"构造函数"
//	v8::Handle<v8::FunctionTemplate> ft = v8::FunctionTemplate::New(_testObjConstructor);
//	ft->SetClassName(JS_STR("TestObj"));
//
//	//在javascript的class内放一个C++实例, *CTestObj
//	//所以要设定InternalFieldCount为1
//	v8::Handle<v8::ObjectTemplate> it = ft->InstanceTemplate();
//	it->SetInternalFieldCount(1); //CTestObj
//
//	//类的"成员函数"
//	v8::Handle<v8::ObjectTemplate> pt = ft->PrototypeTemplate();
//	pt->Set("printName", v8::FunctionTemplate::New(_testObjPrintName));
//	pt->Set("setName", v8::FunctionTemplate::New(_testObjSetName));
//	//用来释放内部数据...
//	pt->Set("dispose", v8::FunctionTemplate::New(_testObjDispose));
//
//	return handle_scope.Close(ft);
//}
////*/
//
//
//Handle<v8::FunctionTemplate> publicCppClass_Item()
//{
//	v8::HandleScope handle_scope;
//
//	//指定"构造函数"
//	v8::Handle<v8::FunctionTemplate> ft = v8::FunctionTemplate::New(_ItemConstructor);
//	//ft->SetClassName(JS_STR("x111"));
//
//	//在javascript的class内放一个C++实例
//	//所以要设定InternalFieldCount为1
//	v8::Handle<v8::ObjectTemplate> it = ft->InstanceTemplate();
//	it->SetInternalFieldCount(1); 
//
//	//类的"成员函数"
//	v8::Handle<v8::ObjectTemplate> pt = ft->PrototypeTemplate();
//	pt->Set("printName", v8::FunctionTemplate::New(_ItemPrintName));
//	pt->Set("setName", v8::FunctionTemplate::New(_ItemSetName));
//	pt->Set("getName", v8::FunctionTemplate::New(_ItemGetName));
//	pt->SetAccessor( String::New("title"), xxxGetter, xxxSetter);
//
//	return handle_scope.Close(ft);
//}
//
//
//}
//
//
//JsEngine		_je;
//
//void RunScript()
//{
//	HandleScope sp;
//
//	//HandleScope handleScope;
//	//Handle<Context> context = Context::New();
//	//Context::Scope contextScope(context);
//
//	string scriptFileText = tk::getFileContents("item.js");
//	if (scriptFileText.length()<=0) 
//	{
//		ATLTRACE("无法打开js文件!\n");
//		return;
//	}
//
//	//最后，环境准备好后，可以执行脚本了.
//	Handle<String> scriptSource = String::New(scriptFileText.c_str());
//
//	//"编译"脚本
//	TryCatch catcher;
//	Handle<Script> script = Script::Compile(scriptSource);
//	if (catcher.HasCaught())  {
//		tk::printError(&catcher);
//		return;
//	}
//
//	//执行脚本 
//	Handle<Value> result = script->Run();
//	if (catcher.HasCaught())  {
//		tk::printError(&catcher);
//		return;
//	}
//}
//
//
//int InitJS()
//{
//	HandleScope sp;
//	//HandleScope handleScope;
//	//Handle<Context> context = Context::New();
//	//Context::Scope contextScope(context);
//
//	//javascript中无打印函数
//	Handle<Object> globalObj = _je.global();//Context::GetCurrent()->Global();
//	globalObj->Set(String::New("printf"), FunctionTemplate::New(_printfForJS)->GetFunction());
//
//	//111
//	//string scriptFileText = tk::getFileContents("item.js");
//	//if (scriptFileText.length()<=0) 
//	//{
//	//	ATLTRACE("无法打开js文件!\n");
//	//	return 0;
//	//}
//
//	Handle<v8::FunctionTemplate> ccItem = v8class::publicCppClass_Item();
//
//	Local<Object>	oItem = ccItem->GetFunction()->NewInstance();
//	v8class::CItem *item = (v8class::CItem *)oItem->GetPointerFromInternalField(0);
//	if (item){
//		item->_name = L"343434";
//	}
//
//	globalObj->Set(String::New("GItem"), oItem );
//	return 0;
//}
//
//
//
//
//int TestV8()
//{
//	HandleScope handleScope;
//	Handle<Context> context = Context::New();
//	Context::Scope contextScope(context);
//
//	//javascript中无打印函数
//	Handle<Object> globalObj = Context::GetCurrent()->Global();
//	globalObj->Set(String::New("printf"), FunctionTemplate::New(_printfForJS)->GetFunction());
//
//
//
//	//string scriptFileText = tk::getFileContents(argv[1]);
//	//string scriptFileText = tk::getFileContents("fromJavaScriptToCpp.js");
//	string scriptFileText = tk::getFileContents("item.js");
//	if (scriptFileText.length()<=0) 
//	{
//		ATLTRACE("无法打开js文件!\n");
//		return 0;
//	}
//
//	////为脚本构造一个执行环境
//
//	////1. 创建一个字符串数组(当然也可以是其它类型)
//	//Handle<Array> _array = Array::New();
//	//char *strList[] = {"hello","the", "world"};
//	//for (int i=0; i<sizeof(strList)/sizeof(strList[0]); i++)
//	//	_array->Set(JS_INT(i), JS_STR(strList[i]));
//	//globalObj->Set(String::New("testArray"), _array);
//
//	////2. 创建一个Object testObject
//	//Handle<Object> _obj = Object::New();
//	//_obj->Set(JS_STR("strVal"), JS_STR("Hi,this is strVal"));
//	//globalObj->Set(String::New("testObject"), _obj);
//
//	//3. 创建一个可以在javascript中New的class...(怎么表述?)
//	//   像这样使用：
//	//    var obj = new Socket(Socket.PF_INET, Socket.SOCK_STREAM, Socket.IPPROTO_TCP);
//	//Handle<v8::FunctionTemplate> bb = pubishJavascriptClass_Socket();
//	//globalObj->Set(String::New("Socket"), bb->GetFunction());
//
//	////4. 包装一个含有真正C++类实体的javascript class
//	////   像这样使用： 
//	////    var obj = new TestObj;
//	////@confused v8的垃圾回收机制还没弄明白, 这里的TestObj必须显式释放: obj.dispose();
//	//Handle<v8::FunctionTemplate> cc = pubishCppClass_TestObj();
//	//globalObj->Set(String::New("TestObj"), cc->GetFunction());
//
//	Handle<v8::FunctionTemplate> ccItem = v8class::publicCppClass_Item();
//	
//	//globalObj->Set(String::New("ItemObj"), ccItem->GetFunction());
//
//
//
//	Local<Object>	oItem = ccItem->GetFunction()->NewInstance();
//	v8class::CItem *item = (v8class::CItem *)oItem->GetPointerFromInternalField(0);
//	if (item){
//		item->_name = L"343434";
//	}
//
//	globalObj->Set(String::New("GItem"), oItem );
//
//
//	////5. 直接实例化一个类，在javascript中访问
//	////   我们在脚本中操作此globaTestObj, 然后在结束的时候在c++中查看globaTestObj
//	////   中包含的数据
//	//Handle<Object> obj = cc->GetFunction()->NewInstance();
//	//CTestObj *pObj = (CTestObj *)obj->GetPointerFromInternalField(0);
//	//if (pObj){
//	//	pObj->printfName();
//	//	pObj->setName("pansunyou");
//	//	pObj->printfName();
//	//}
//	//globalObj->Set(String::New("globaTestObj"), obj);
//
//
//	//Handle<Object> oItem = ccItem->GetFunction()->NewInstance();
// //	CItem *item = (CItem *)oItem->GetPointerFromInternalField(0);
//	//if (item){
//	//	item->_name = L"11111111111";
//	//}
//	//globalObj->Set(String::New("globalItem"), oItem );
//
//
//
//
//	//还有其它一些东西,比如SetAccessor
//	//有空再研究
//	//globalObj->SetAccessor()
//
//	//最后，环境准备好后，可以执行脚本了.
//	Handle<String> scriptSource = String::New(scriptFileText.c_str());
//
//	//"编译"脚本
//	TryCatch catcher;
//	Handle<Script> script = Script::Compile(scriptSource);
//	if (catcher.HasCaught())  {
//		tk::printError(&catcher);
//		return 0;
//	}
//
//	//执行脚本 
//	Handle<Value> result = script->Run();
//	if (catcher.HasCaught())  {
//		tk::printError(&catcher);
//		return 0;
//	}
//
//	//// 5.1 看看脚本是否修改了我们的值
//	//pObj = (CTestObj *)obj->GetPointerFromInternalField(0);
//	//if (pObj){
//	//	pObj->printfName();
//
//	//	delete pObj;
//	//	obj->SetPointerInInternalField(0, NULL);
//	//}
//
//	/////@confused 这样清理有用么?
//	//v8::Handle<v8::Array> keys = JS_GLOBAL->GetPropertyNames();
//	//int length = keys->Length();
//	//for (int i=0;i<length;i++) {
//	//	v8::Handle<v8::String> key = keys->Get(JS_INT(i))->ToString();
//	//	JS_GLOBAL->ForceDelete(key);
//	//}
//
//	// 测试全局变量的函数
//
//	//Local<Object>	o = Local<Object>::Cast( globalObj->Get(String::New("GItem") ) );
//	//Local<Value>	value = o->Get(String::New( "JSFun" ));
//
//	Local<Object>	o = Local<Object>::Cast( globalObj->Get(String::New("GItem") ) );
//	Local<Value>	value = o->Get(String::New( "OnInfo" ));
//	if( !value.IsEmpty() && value->IsFunction() )
//	{
//		Local<Function> fun = Local<Function>::Cast( value );
//
//		Handle<Value> args[1];
//		args[0] = Uint32::New( 98765 );
//		fun->Call( o, 1, args );
//	}
//
//	return 0;
//}
//
//




int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	//// Test;
	//int *an = (int *)malloc( 10*4 );
	//for( int i = 0; i < 10; i++ )
	//	an[i] = i+1;

	//realloc( (void*)an, 20*4 );

	//free( an );

	//wchar_t		buffer[1024] = {0};
	//GetCurrentDirectory( 1024, buffer );
	//MessageBox( NULL, buffer, L"", MB_OK );

	//TestV8();

	//HandleScope handleScope;
	//Handle<Context> context = Context::New();
	//Context::Scope contextScope(context);

	//HandleScope handleScope;
	//Persistent<Context>		_ctx = Context::New();
	//Context::Scope contextScope( _ctx );
	//_ctx->Enter();




	//InitJS();
	//RunScript();

	//{
	//	HandleScope sp;

	//	Handle<Object> globalObj = Context::GetCurrent()->Global();
	//	Local<Object>	o = Local<Object>::Cast( globalObj->Get(String::New("GItem") ) );
	//	Local<Value>	value = o->Get(String::New( "OnInfo" ));
	//	if( !value.IsEmpty() && value->IsFunction() )
	//	{
	//		Local<Function> fun = Local<Function>::Cast( value );

	//		Handle<Value> args[1];
	//		args[0] = Uint32::New( 98765 );
	//		fun->Call( o, 1, args );
	//	}
	//}

	////_ctx->Exit();
	////_ctx.Dispose();


	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	ULONG_PTR token;
	GdiplusStartupInput gsi;
	::GdiplusStartup(&token, &gsi, NULL);

	CAppData::SetInstance(hInstance);
	CAppData::SetResourcePath( CAppData::GetInstancePath() + L"skin/" );


	//int nRet = CDlgNetTest::PopNetTestDlg();
	//if( nRet == 2 )
	//{
	//	PostQuitMessage( 0 );
	//}
	//else
	//{
		CSwhMainLayout	*pMainWnd = new CSwhMainLayout;
		//CMainDlg		*pMainWnd = new CMainDlg;
		//CDlgTestList	*pMainWnd = new CDlgTestList;
		//pMainWnd->Create( NULL, L"Title", UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE );
		pMainWnd->Create( NULL, L"Title", WS_VISIBLE|WS_POPUPWINDOW, 0 );
		pMainWnd->CenterWindow();
		CAppData::MessageLoop();
		DEL( pMainWnd );
	//}

	::GdiplusShutdown(token);
	CoUninitialize();
}
