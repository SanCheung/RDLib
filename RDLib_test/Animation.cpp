#include "stdAfx.h"
#include "Animation.h"

#include <math.h>

ImageState* CAniTask::get()
{
	//int i = _index+1;
	float i = (float)_r[_index]*_count2;

	_scurrent._x = _sc._x*i + _s1->_x;
	_scurrent._y = _sc._y*i + _s1->_y;
	_scurrent._w = _sc._w*i + _s1->_w;
	_scurrent._h = _sc._h*i + _s1->_h;
	_scurrent._r = _sc._r*i + _s1->_r;
	_scurrent._a = _sc._a*i + _s1->_a;

	//if( _sc._a == 0 )
	//	_scurrent._a = 0;

	return &_scurrent;
}

// return continue
bool CAniTask::step()
{
	_index++;
	if( _index == _count )
	{
		_index = 0;
		return false;
	}

	return true;
}

void CAniTask::rewind()
{
	_index = 0;
}

