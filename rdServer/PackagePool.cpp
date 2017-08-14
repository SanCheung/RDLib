#include "StdAfx.h"
#include "PackagePool.h"
#include "TCPServer.h"

#include "Cryp.h"


CPackagePool::CPackagePool( CTCPServer* pServer )
	: m_pServer( pServer )
{
	m_MaxElapsed = 0;
}


CPackagePool::~CPackagePool(void)
{
}

void CPackagePool::Process()
{
	while (m_PackageInQueue.Size())
	{
		CAntPackagePtr package = m_PackageInQueue.Pop();
		if (package)
		{
			UINT sessionID = 0;
			UINT userID = 0;
			UINT meetingID = 0;
			bool find = package->GetProperty("Internal_SessionID", sessionID);
			find = package->GetProperty("Internal_UserID", userID);
			find = package->GetProperty("Internal_MeetingID", meetingID);

			bool result = OnProcessPackage(sessionID, userID, meetingID, package);

			package->StatEnd();

			long ref = package.use_count();

			m_LockStat.Lock();
			DWORD elapsed = package->ProcessElapsed();
			m_MaxElapsed = max(m_MaxElapsed, elapsed);
			m_LockStat.UnLock();

			ATLTRACE("Max:%u %u\n", m_MaxElapsed, elapsed);
		}
	}
}

bool CPackagePool::Idle()
{
	return m_PackageInQueue.Size() == 0;
}

void CPackagePool::AddInPackage( CAntPackagePtr& package )
{
	m_PackageInQueue.Push(package);
}

bool CPackagePool::WaitProcess( int ms )
{ 
	return m_PackageInQueue.WaitPop(ms) ;
}

bool CPackagePool::OnProcessPackage( UINT sessionID, UINT userID, UINT meetingID, CAntPackagePtr& package)
{
	bool result = false;

	if (package)
	{
		string& method = package->GetMethod();
		printf( "%s %d %d %d\n", method.c_str(), sessionID, userID, meetingID );


		if( method == "glc" )
		{
			UINT	code;
			package->GetProperty( "code", code );
			printf( "\tcode: %u\n", code );

			UINT v[2];
			v[0] = code;
			v[1] = 0x12345678;

			UINT k[4] = 
			{
				0x0ED967DE,
				0x52733A81,
				0x665203CF,
				0x1CC77472,
			};
			CCryp::tea_enc( k, v );
			UINT	code2 = v[0];

			CAntPackage responsePackage;
			responsePackage.SetMethod( "glc", 3 );
			responsePackage.AddParam( 0 );
			responsePackage.AddProperty( "code", code2 );

			CBuffer		buffer;
			responsePackage.ToProtocolBuffer( buffer );
			m_pServer->ResponseToSession( sessionID, buffer );
		}



		//CBuffer		buffer;
		//package->ToProtocolBuffer( buffer );
		//m_pServer->ResponseToSession( sessionID, buffer );
	}
	return result;
}
