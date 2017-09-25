#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;
namespace MGCDataDefine
{
	enum enum_http_request
	{
		enum_http_login = 0,
		enum_http_phone,
		enum_http_qrcode,
		enum_http_down_file,
		enum_http_game_family,
		enum_http_game,
	};

	//base
	struct mgcRequestResult
	{
		mgcRequestResult()
		{
			code = -1; //data exception
			msg = "";  //MultiByte
		}
		int code;
		string msg;
	};

	//login item
	struct mgcHttpLogin
	{
		string name;
		string password;
	};

	struct mgcHttpLoginResult:mgcRequestResult
	{
		mgcHttpLoginResult()
		{
			operationId = "";
			operationName = "";
			operationCode = "";
		}
		string operationId;
		string operationName;
		string operationCode;
	};

	//service phone number
	//input parameter none
	struct mgcHttpPhoneResult:mgcRequestResult
	{
		mgcHttpPhoneResult()
		{
			phone = "";
		}
		string phone;
	};


	//qr code
	struct mgcHttpQRCode
	{
		string clientId;
		string filePath;
	};

	struct mgcHttpQRCodeResult:mgcRequestResult
	{
	};

	//down file
	struct mgcHttpDownFile
	{
		string fileUrl;
		string filePath;
	};
	struct mgcHttpDownFileResult:mgcRequestResult
	{
	};

	//游戏类别
	struct mgcHttpGameFamily
	{
		string familyName;
		string familyNum;
		int pageNum;
	};


	typedef struct mgcGameFamilyInfo
	{
		string familyId;
		string familyNum;
		string familyName;
		string familyIcon;
	} mgcGfi;

	struct mgcHttpGameFamilyResult:mgcRequestResult
	{
		int pageSize;
		int totalResult;
		vector<mgcGameFamilyInfo> vecFamilyInfo;
		map<string,bool> mapFamilyId;  //判断是否已添加
	};

	//游戏信息
	struct mgcHttpGame
	{
		string gameName;
		string gameFamily;
		string operationMode;
		int pageNum;
	};

	struct mgcGameInfo
	{
		string gameId;
		string gameName;
		string gamePath;
		string gameFamily;
		string operationMode;
		string gameCover;
		string gameVideo;
	};

	struct mgcHttpGameResult:mgcRequestResult
	{
		int pageSize;
		int totalResult;
		vector<mgcGameInfo> vecGameInfo;
		map<string,bool> mapGameId;  //判断是否已添加
	};
}