#pragma once

class ImageState
{
public:
	float	_x;  
	float	_y;
	float	_w;
	float	_h;
	float	_r;		// rotate 0~360
	float	_a;		// alpha  0~1.f
	float	_a2;

	ImageState()
	{
		_x = _y = _w = _h = _r = _a = _a2 = 0.f;
	}

	ImageState( float x, float y, float w, float h, float angle, float alpha, float alpha2 )
		:_x(x), _y(y), _w(w), _h(h), _r(angle), _a( alpha ), _a2(alpha2)
	{
	}

	void calcClip( ImageState *s1, ImageState *s2, int count )
	{
		_x = (s2->_x - s1->_x)/count;
		_y = (s2->_y - s1->_y)/count;
		_w = (s2->_w - s1->_w)/count;
		_h = (s2->_h - s1->_h)/count;
		_r = (s2->_r - s1->_r)/count;
		_a = (s2->_a - s1->_a)/count;
		_a2= (s2->_a2-s1->_a2)/count;
	}
};

class CAniTask
{
public:
	ImageState*	_s1;    // begin
	ImageState*	_s2;	// end
	int			_count;

	int			_count2;
	vector<double>	_r;

	//CAniTask( float x, float y, float w, float h, float r, float a,
	//		  float x2, float y2, float w2, float h2, float r2, float a,
	//		  int nCount )
	//	: _s1( x, y, w, h, r, a )
	//	, _s2( x2, y2, w2, h2, r2, a2 )

	CAniTask( ImageState *s1, ImageState *s2, int nFrameCount )
		: _s1( s1 )
		, _s2( s2 )
		, _count( nFrameCount )
		, _index( 0 )
	{
		// 线性
		//_sc.calcClip( _s1, _s2, _count );

		// 二次平滑
		_r.resize( _count );

		int n = 0;
		int i;
		for( i = 0; i < _count/2; i++ )
		{
			if( i < 5 )
				n += 2*i*i;
			else
				n += 32;
		}

		for( i = 0; i< _count/2; i++ )
		{
			if( i < 5 )
			{
				_r[i] = i*i*1.f/n;
				_r[_count-1-i] = 1.f-_r[i];
			}
			else
			{
				_r[i] = _r[i-1] + 16.f/n;
				_r[_count-1-i] = 1.f-_r[i];
			}
		}

		_sc.calcClip( _s1, _s2, n );
		_count2 = n;
	}


	ImageState*	get();
	void		rewind();

	// true:  continue
	// false: over
	bool		step();

private:
	ImageState	_sc;	// clip
	int			_index;

	ImageState  _scurrent;
};


class CAniJob
{
public:
	CAniJob(){}
	virtual ~CAniJob() { aj_clearAll(); }

	void aj_clearAll()
	{
		int n = (int)_tasks.size();
		for( int i = 0; i < n; i++ )
		{
			auto *p = _tasks[i];
			delete p;
			p = nullptr;
		}

		_tasks.clear();
	}

	void aj_addTask( CAniTask *p )
	{
		_tasks.push_back( p );
	}

	bool aj_step()
	{
		bool bContinue = true;
		int n = (int)_tasks.size();
		for( int i = 0; i < n; i++ )
		{
			bContinue = _tasks[i]->step();
		}

		return bContinue;
	}

	ImageState*	aj_getIs( int i )
	{
		return _tasks[i]->get();
	}

	bool aj_valid()
	{
		return !_tasks.empty();
	}


	vector< CAniTask* >		_tasks;
};
