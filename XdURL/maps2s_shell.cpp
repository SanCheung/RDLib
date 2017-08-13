#include "stdAfx.h"
#include "XdURLLib.h"
#include "maps2s_shell.h"


string maps2s_shell::stringValue( char* key )
{
	if( isEmpty() )
		return "";

	if( _m->find( key ) == _m->end() )
		return "";

	string str = _m->at( key );
	return str;
}

int maps2s_shell::intValue( char* key )
{
	if( isEmpty() )
		return 0;

	if( _m->find( key ) == _m->end() )
		return 0;

	return atoi( _m->at( key ).c_str() );
}

maps2s_shell::operator maps2s() const
{
	return *_m;
}

bool maps2s_shell::isEmpty()
{
	if( _m == nullptr || _m->empty() )
		return true;

	return false;
}
