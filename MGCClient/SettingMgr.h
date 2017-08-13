#pragma once
class CSettingMgr
{
public:
	CSettingMgr(void);
	virtual ~CSettingMgr(void);


	static CSettingMgr* instance( bool bReload = false );
	static void	release();

	static CSettingMgr* s_pSM;
	static CASYNC::CSectionCritical s_sc;

private:
	bool	load();

public:
	CStringW	_strWeb;

	// 全局信息
	CStringW	_strPhone;
};


#define		SetMgr		CSettingMgr::instance

