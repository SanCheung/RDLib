// js_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "js_macros.h"
#include "commonstuff.hpp"



namespace v8class
{
	
class CItem
{
public:
	CItem()	{}
	~CItem(){}

	string	_name;
	string	_title;
};

JS_METHOD(_ItemConstructor)
{
	ASSERT_CONSTRUCTOR;
	SAVE_PTR(0, new CItem());
	return args.This();
}


JS_METHOD(_ItemSetName)
{
	CItem *item = LOAD_PTR(0, CItem *);
	if( item!=NULL ){
		v8::String::AsciiValue xname(args[0]);
		item->_name = *xname;

		ATLTRACE( "CItem::setName name=%s\n", item->_name.c_str() );
	}
	return args.This();
}

JS_METHOD(_ItemGetName)
{
	CItem *item = LOAD_PTR(0, CItem *);
	if( item!=NULL ){
		//v8::String::AsciiValue xname(args[0]);
		//v8::Value item->_name = *xname;

		ATLTRACE( "CItem::getName %s\n", item->_name.c_str() );
		char *str = (char *)item->_name.c_str();
		return v8::String::New( str );

		//return v8::Undefined();
	}
	return v8::Undefined();
}






JS_METHOD(_ItemPrintName)
{
	Local<Object>	pT = args.This();

	CItem *item = LOAD_PTR(0, CItem *);
	if( item!=NULL ){
		ATLTRACE( "CItem::printName name=%s\n", item->_name.c_str() );
		//ATLTRACE( "\t***Call OnInfo\n" );
		//Local<Value>	value = pT->Get(String::New( "OnInfo" ));
		//if( !value.IsEmpty() && value->IsFunction() )
		//{
		//	Local<Function> fun = Local<Function>::Cast( value );

		//	Handle<Value> args[1];
		//	args[0] = Uint32::New( 8848 );
		//	fun->Call( pT, 1, args );
		//}
	}
	return args.This();
}


static Handle<Value> xxxGetter( Local<String> name, const AccessorInfo& info)
{ 
	CItem *item = reinterpret_cast<CItem*>(info.This()->GetPointerFromInternalField(0));
	if( item!=NULL )
	{
		ATLTRACE( "CItem::getTitle %s\n", item->_title.c_str() );
		char *str = (char *)item->_title.c_str();
		return v8::String::New( str );
	}
	return v8::Undefined();
} 

static void xxxSetter( Local<String> name, Local<Value> value, const AccessorInfo& info)
{ 
	CItem *item = reinterpret_cast<CItem*>(info.This()->GetPointerFromInternalField(0));
	if( item!=NULL ){
		v8::String::AsciiValue xname(value);
		item->_title = *xname;

		ATLTRACE( "CItem::setTitle %s\n", item->_title.c_str() );
	}
}



/*
/// 
/// @brief ��javascript�����й���һ��class TestObj
/// 
/// @confused v8���������ջ��ƻ�ûŪ����, �����TestObj������ʽ�ͷ�: obj.dispose();
///
Handle<v8::FunctionTemplate> pubishCppClass_TestObj()
{
	v8::HandleScope handle_scope;

	//ָ��"���캯��"
	v8::Handle<v8::FunctionTemplate> ft = v8::FunctionTemplate::New(_testObjConstructor);
	ft->SetClassName(JS_STR("TestObj"));

	//��javascript��class�ڷ�һ��C++ʵ��, *CTestObj
	//����Ҫ�趨InternalFieldCountΪ1
	v8::Handle<v8::ObjectTemplate> it = ft->InstanceTemplate();
	it->SetInternalFieldCount(1); //CTestObj

	//���"��Ա����"
	v8::Handle<v8::ObjectTemplate> pt = ft->PrototypeTemplate();
	pt->Set("printName", v8::FunctionTemplate::New(_testObjPrintName));
	pt->Set("setName", v8::FunctionTemplate::New(_testObjSetName));
	//�����ͷ��ڲ�����...
	pt->Set("dispose", v8::FunctionTemplate::New(_testObjDispose));

	return handle_scope.Close(ft);
}
//*/


Handle<v8::FunctionTemplate> publicCppClass_Item()
{
	v8::HandleScope handle_scope;

	//ָ��"���캯��"
	v8::Handle<v8::FunctionTemplate> ft = v8::FunctionTemplate::New(_ItemConstructor);
	//ft->SetClassName(JS_STR("x111"));

	//��javascript��class�ڷ�һ��C++ʵ��
	//����Ҫ�趨InternalFieldCountΪ1
	v8::Handle<v8::ObjectTemplate> it = ft->InstanceTemplate();
	it->SetInternalFieldCount(1); 

	//���"��Ա����"
	v8::Handle<v8::ObjectTemplate> pt = ft->PrototypeTemplate();
	pt->Set("printName", v8::FunctionTemplate::New(_ItemPrintName));
	pt->Set("setName", v8::FunctionTemplate::New(_ItemSetName));
	pt->Set("getName", v8::FunctionTemplate::New(_ItemGetName));
	pt->SetAccessor( String::New("title"), xxxGetter, xxxSetter);

	return handle_scope.Close(ft);
}


Local<ObjectTemplate> getClass()
{
	v8::HandleScope handle_scope;

	Local<ObjectTemplate> ot = ObjectTemplate::New();
	ot->SetInternalFieldCount(1);

	ot->Set(String::New("printName"), FunctionTemplate::New(_ItemPrintName));
	ot->Set(String::New("setName"), FunctionTemplate::New(_ItemSetName));
	ot->Set(String::New("getName"), FunctionTemplate::New(_ItemGetName));
	ot->SetAccessor( String::New("title"), xxxGetter, xxxSetter);

	return handle_scope.Close(ot);
}


}






int TestV8()
{
	HandleScope handleScope;
	Handle<Context> context = Context::New();
	Context::Scope contextScope(context);

	//javascript���޴�ӡ����
	Handle<Object> globalObj = Context::GetCurrent()->Global();
	globalObj->Set(String::New("printf"), FunctionTemplate::New(_printfForJS)->GetFunction());



	//string scriptFileText = tk::getFileContents(argv[1]);
	//string scriptFileText = tk::getFileContents("fromJavaScriptToCpp.js");
	string scriptFileText = tk::getFileContents("item.js");
	if (scriptFileText.length()<=0) 
	{
		ATLTRACE("�޷���js�ļ�!\n");
		return 0;
	}

	////Ϊ�ű�����һ��ִ�л���

	////1. ����һ���ַ�������(��ȻҲ��������������)
	//Handle<Array> _array = Array::New();
	//char *strList[] = {"hello","the", "world"};
	//for (int i=0; i<sizeof(strList)/sizeof(strList[0]); i++)
	//	_array->Set(JS_INT(i), JS_STR(strList[i]));
	//globalObj->Set(String::New("testArray"), _array);

	////2. ����һ��Object testObject
	//Handle<Object> _obj = Object::New();
	//_obj->Set(JS_STR("strVal"), JS_STR("Hi,this is strVal"));
	//globalObj->Set(String::New("testObject"), _obj);

	//3. ����һ��������javascript��New��class...(��ô����?)
	//   ������ʹ�ã�
	//    var obj = new Socket(Socket.PF_INET, Socket.SOCK_STREAM, Socket.IPPROTO_TCP);
	//Handle<v8::FunctionTemplate> bb = pubishJavascriptClass_Socket();
	//globalObj->Set(String::New("Socket"), bb->GetFunction());

	////4. ��װһ����������C++��ʵ���javascript class
	////   ������ʹ�ã� 
	////    var obj = new TestObj;
	////@confused v8���������ջ��ƻ�ûŪ����, �����TestObj������ʽ�ͷ�: obj.dispose();
	//Handle<v8::FunctionTemplate> cc = pubishCppClass_TestObj();
	//globalObj->Set(String::New("TestObj"), cc->GetFunction());

	Handle<v8::FunctionTemplate> ccItem = v8class::publicCppClass_Item();

	//globalObj->Set(String::New("ItemObj"), ccItem->GetFunction());


	Local<Object>	oItem = ccItem->GetFunction()->NewInstance();
	v8class::CItem *item = (v8class::CItem *)oItem->GetPointerFromInternalField(0);
	if (item){
		item->_name = "343434";
	}

	globalObj->Set(String::New("GItem"), oItem );


	////5. ֱ��ʵ����һ���࣬��javascript�з���
	////   �����ڽű��в�����globaTestObj, Ȼ���ڽ�����ʱ����c++�в鿴globaTestObj
	////   �а���������
	//Handle<Object> obj = cc->GetFunction()->NewInstance();
	//CTestObj *pObj = (CTestObj *)obj->GetPointerFromInternalField(0);
	//if (pObj){
	//	pObj->printfName();
	//	pObj->setName("pansunyou");
	//	pObj->printfName();
	//}
	//globalObj->Set(String::New("globaTestObj"), obj);


	//Handle<Object> oItem = ccItem->GetFunction()->NewInstance();
	//	CItem *item = (CItem *)oItem->GetPointerFromInternalField(0);
	//if (item){
	//	item->_name = L"11111111111";
	//}
	//globalObj->Set(String::New("globalItem"), oItem );




	//��������һЩ����,����SetAccessor
	//�п����о�
	//globalObj->SetAccessor()

	//��󣬻���׼���ú󣬿���ִ�нű���.
	Handle<String> scriptSource = String::New(scriptFileText.c_str());

	//"����"�ű�
	TryCatch catcher;
	Handle<Script> script = Script::Compile(scriptSource);
	if (catcher.HasCaught())  {
		tk::printError(&catcher);
		return 0;
	}

	//ִ�нű� 
	Handle<Value> result = script->Run();
	if (catcher.HasCaught())  {
		tk::printError(&catcher);
		return 0;
	}

	//// 5.1 �����ű��Ƿ��޸������ǵ�ֵ
	//pObj = (CTestObj *)obj->GetPointerFromInternalField(0);
	//if (pObj){
	//	pObj->printfName();

	//	delete pObj;
	//	obj->SetPointerInInternalField(0, NULL);
	//}

	/////@confused ������������ô?
	//v8::Handle<v8::Array> keys = JS_GLOBAL->GetPropertyNames();
	//int length = keys->Length();
	//for (int i=0;i<length;i++) {
	//	v8::Handle<v8::String> key = keys->Get(JS_INT(i))->ToString();
	//	JS_GLOBAL->ForceDelete(key);
	//}

	// ����ȫ�ֱ����ĺ���

	//Local<Object>	o = Local<Object>::Cast( globalObj->Get(String::New("GItem") ) );
	//Local<Value>	value = o->Get(String::New( "JSFun" ));

	Local<Object>	o = Local<Object>::Cast( globalObj->Get(String::New("GItem") ) );
	Local<Value>	value = o->Get(String::New( "OnInfo" ));
	if( !value.IsEmpty() && value->IsFunction() )
	{
		Local<Function> fun = Local<Function>::Cast( value );

		Handle<Value> args[1];
		args[0] = Uint32::New( 98765 );
		fun->Call( o, 1, args );
	}

	return 0;
}


int v82()
{
	HandleScope handleScope;
	Handle<Context> context = Context::New();
	Context::Scope contextScope(context);

	//javascript���޴�ӡ����
	Handle<Object> globalObj = Context::GetCurrent()->Global();
	globalObj->Set(String::New("printf"), FunctionTemplate::New(_printfForJS)->GetFunction());


	Handle<v8::FunctionTemplate> ccItem = v8class::publicCppClass_Item();

	Local<Object>	oItem = ccItem->GetFunction()->NewInstance();
	v8class::CItem *item = (v8class::CItem *)oItem->GetPointerFromInternalField(0);
	if (item){
		item->_name = "343434";
	}

	globalObj->Set(String::New("GItem"), oItem );


	//Local<Object>	oItem2 = ccItem->GetFunction()->NewInstance();
	//v8class::CItem *item2 = (v8class::CItem *)oItem2->GetPointerFromInternalField(0);
	//if (item2){
	//	item2->_name = "2222222";
	//	item2->_title = "ttttttt";
	//}

	v8class::CItem *item2 = new v8class::CItem;
	if (item2){
		item2->_name = "2222222";
		item2->_title = "ttttttt";
	}


	//Local<ObjectTemplate> ot = ObjectTemplate::New();
	//ot->SetInternalFieldCount(1);

	//ot->Set(String::New("printName"), FunctionTemplate::New(v8class::_ItemPrintName));
	//ot->Set(String::New("setName"), FunctionTemplate::New(v8class::_ItemSetName));
	//ot->Set(String::New("getName"), FunctionTemplate::New(v8class::_ItemGetName));
	//ot->SetAccessor( String::New("title"), v8class::xxxGetter, v8class::xxxSetter);
	Local<ObjectTemplate> ot = v8class::getClass();

	Local<Object>	oItem2 = ot->NewInstance();
	oItem2->SetInternalField(0, External::New( item2));
	globalObj->Set(String::New("Item2"), oItem2 );

	
	v8class::CItem *item3 = new v8class::CItem;
	if (item3){
		item3->_name = "333333";
		item3->_title = "zzzzzzzz";
	}

	Local<ObjectTemplate> ot3 = v8class::getClass();
	Local<Object>	oItem3 = ot3->NewInstance();
	oItem3->SetInternalField(0, External::New( item3));
	globalObj->Set(String::New("Item3"), oItem3 );



	string scriptFileText = tk::getFileContents("item2.js");
	if (scriptFileText.length()<=0) 
	{
		ATLTRACE("�޷���js�ļ�!\n");
		return 0;
	}

	//��󣬻���׼���ú󣬿���ִ�нű���.
	Handle<String> scriptSource = String::New(scriptFileText.c_str());

	//"����"�ű�
	TryCatch catcher;
	Handle<Script> script = Script::Compile(scriptSource);
	if (catcher.HasCaught())  {
		tk::printError(&catcher);
		return 0;
	}

	//ִ�нű� 
	Handle<Value> result = script->Run();
	if (catcher.HasCaught())  {
		tk::printError(&catcher);
		return 0;
	}


	if (item2){
		item2->_name = "9999999999";
		item2->_title = "xxxxxxxxxxx";
	}


	// ִ����js֮���ٵ�����������
	Local<Object>	o = Local<Object>::Cast( globalObj->Get(String::New("Item2") ) );
	Local<Value>	value = o->Get(String::New( "OnInfo" ));
	if( !value.IsEmpty() && value->IsFunction() )
	{
		Local<Function> fun = Local<Function>::Cast( value );

		Handle<Value> args[1];
		args[0] = Uint32::New( 98765 );
		fun->Call( o, 1, args );
	}

	return 0;
}





int _tmain(int argc, _TCHAR* argv[])
{
	//TestV8();
	v82();
	return 0;
}

