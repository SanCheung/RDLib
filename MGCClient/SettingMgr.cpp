#include "StdAfx.h"
#include "SettingMgr.h"
#include "MainHelper.h"
#include "Helper.h"

CSettingMgr* CSettingMgr::s_pSM = nullptr;
CASYNC::CSectionCritical CSettingMgr::s_sc;

CSettingMgr::CSettingMgr(void)
{
}


CSettingMgr::~CSettingMgr(void)
{
}

CSettingMgr* CSettingMgr::instance( bool bReload/* = false*/ )
{
	CASYNC::CLockScoped lock( s_sc );

	if( s_pSM )
		return s_pSM;

	s_pSM = new CSettingMgr;
	if( s_pSM->load() )
		return s_pSM;

	DEL( s_pSM );
	return nullptr;
}

void CSettingMgr::release()
{
	if( s_pSM )
		DEL( s_pSM );
}

bool CSettingMgr::load()
{
	CStringW		strIniFile = CAppData::GetInstancePath() + CONFIG_INI;
	if( !CHelper::fileIsExist( strIniFile ) )
	{
		mgTrace( L"%s�����ڣ�", CONFIG_INI );
		return false;
	}

	fblSystem::CIni		ini( strIniFile, L"main" );
	_strWeb = ini.GetString( L"web" );
	return true;
}

////��ȡ���ӿ�·��
//CStringW CSettingMgr::GetInServer()
//{
//	return _strWebServer;
//}
//
////��ȡͨ�ŷ�������ַ
//CStringW CSettingMgr::GetMeetingServer()
//{
//	return _strMeetingServer;
//}