#include "StdAfx.h"
#include "MainDlg.h"


CMainDlg::CMainDlg(void)
{
}


CMainDlg::~CMainDlg(void)
{
}

CControlUI* CMainDlg::CreateControl( LPCTSTR pstrClass )
{
	return NULL;
}

LRESULT CMainDlg::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	PostQuitMessage( 0 );
	return 0;
}

void CMainDlg::Init()
{
	//HandleScope sp;

	//Handle<Object> global = Context::GetCurrent()->Global();
	//Handle<v8::FunctionTemplate> ftButton = CButtonUI::getJSClass();


	//Local<ObjectTemplate> otButton = ObjectTemplate::New();
	//otButton->SetInternalFieldCount(1);

	//Local<Object>	objBn1 = otButton->NewInstance();

	//objBn1->SetInternalField( 0, External::New( FindCtrl<CButtonUI>(L"bn1") ) );
	//global->Set(String::New("bn1"), objBn1 );

	//Local<Object>	objBn2 = otButton->NewInstance();
	//objBn2->SetInternalField( 0, External::New( FindCtrl<CButtonUI>(L"bn2") ) );
	//global->Set(String::New("bn2"), objBn2 );

	//Local<Object>	objBn3 = otButton->NewInstance();
	//objBn3->SetInternalField( 0, External::New( FindCtrl<CButtonUI>(L"bn3") ) );
	//global->Set(String::New("bn3"), objBn3 );

}

void CMainDlg::Notify( TNotifyUI& msg )
{
	CStringW	strSenderName = msg.pSender->GetName();
	CStringW	strType = msg.sType;

	if( strType == L"click")
	{
		//CTabLayoutUI *pTab = FindCtrl<CTabLayoutUI>( L"tMain" );
		//if( strSenderName == L"bn1" )
		//	pTab->SelectItem( 0 );
		//else if( strSenderName == L"bn2" )
		//	pTab->SelectItem( 1 );
		//else if( strSenderName == L"bn3" )
		//	pTab->SelectItem( 2 );
		if( strSenderName == L"bn1" )
		{
			PostQuitMessage( 1 );
			//HandleScope sp;
			//string		strScript = "bn1.Text=\"111111111111\";";

			////最后，环境准备好后，可以执行脚本了.
			//Handle<String> scriptSource = String::New(strScript.c_str());

			////"编译"脚本
			//TryCatch catcher;
			//Handle<Script> script = Script::Compile(scriptSource);
			//if (catcher.HasCaught())  {
			//	return;
			//}

			////执行脚本 
			//Handle<Value> result = script->Run();
			//if (catcher.HasCaught())  {
			//	return;
			//}
		}

	}
}