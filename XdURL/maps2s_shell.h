#pragma once

class XDURL_API maps2s_shell
{
public:
	maps2s_shell( maps2s *m )		{ _m = m; }
	maps2s_shell( const maps2s *m )	{ _m = (maps2s *)m; }
	virtual ~maps2s_shell(void)		{}

	void	setM( maps2s *m )		{ _m = m; }
	operator maps2s() const;

	maps2s	*_m;

	bool		isEmpty();
	string		stringValue( char* key );
	int			intValue( char* key );
};

