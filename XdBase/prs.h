#pragma once

namespace PRS
{
	
// forward declarations
class CDUISize;
class CDUIPoint;
class CDUIRect;

///////////////////////////////////////////////////////////////////////////////
// CSize - Wrapper for Windows SIZE structure.

class XDBASE_API CDUISize : public SIZE
{
public:
	// Constructors
	CDUISize()
	{
		cx = 0;
		cy = 0;
	}

	CDUISize(int initCX, int initCY)
	{
		cx = initCX;
		cy = initCY;
	}

	CDUISize(SIZE initSize)
	{
		*(SIZE*)this = initSize;
	}

	CDUISize(POINT initPt)
	{
		*(POINT*)this = initPt;
	}

	CDUISize(DWORD dwSize)
	{
		cx = (short)LOWORD(dwSize);
		cy = (short)HIWORD(dwSize);
	}

	// Operations
	BOOL operator ==(SIZE size) const
	{
		return (cx == size.cx && cy == size.cy);
	}

	BOOL operator !=(SIZE size) const
	{
		return (cx != size.cx || cy != size.cy);
	}

	void operator +=(SIZE size)
	{
		cx += size.cx;
		cy += size.cy;
	}

	void operator -=(SIZE size)
	{
		cx -= size.cx;
		cy -= size.cy;
	}

	void SetSize(int CX, int CY)
	{
		cx = CX;
		cy = CY;
	}

	// Operators returning CDUISize values
	CDUISize operator +(SIZE size) const
	{
		return CDUISize(cx + size.cx, cy + size.cy);
	}

	CDUISize operator -(SIZE size) const
	{
		return CDUISize(cx - size.cx, cy - size.cy);
	}

	CDUISize operator -() const
	{
		return CDUISize(-cx, -cy);
	}

	// Operators returning CDUIPoint values
	CDUIPoint operator +(POINT point) const;
	CDUIPoint operator -(POINT point) const;

	// Operators returning CDUIRect values
	CDUIRect operator +(const RECT* lpRect) const;
	CDUIRect operator -(const RECT* lpRect) const;
};


///////////////////////////////////////////////////////////////////////////////
// CDUIPoint - Wrapper for Windows POINT structure.

class XDBASE_API CDUIPoint : public POINT
{
public:
	// Constructors
	CDUIPoint()
	{
		x = 0;
		y = 0;
	}

	CDUIPoint(int initX, int initY)
	{
		x = initX;
		y = initY;
	}

	CDUIPoint(POINT initPt)
	{
		*(POINT*)this = initPt;
	}

	CDUIPoint(SIZE initSize)
	{
		*(SIZE*)this = initSize;
	}

	CDUIPoint(DWORD dwPoint)
	{
		x = (short)LOWORD(dwPoint);
		y = (short)HIWORD(dwPoint);
	}

	// Operations
	void Offset(int xOffset, int yOffset)
	{
		x += xOffset;
		y += yOffset;
	}

	void Offset(POINT point)
	{
		x += point.x;
		y += point.y;
	}

	void Offset(SIZE size)
	{
		x += size.cx;
		y += size.cy;
	}

	BOOL operator ==(POINT point) const
	{
		return (x == point.x && y == point.y);
	}

	BOOL operator !=(POINT point) const
	{
		return (x != point.x || y != point.y);
	}

	void operator +=(SIZE size)
	{
		x += size.cx;
		y += size.cy;
	}

	void operator -=(SIZE size)
	{
		x -= size.cx;
		y -= size.cy;
	}

	void operator +=(POINT point)
	{
		x += point.x;
		y += point.y;
	}

	void operator -=(POINT point)
	{
		x -= point.x;
		y -= point.y;
	}

	void SetPoint(int X, int Y)
	{
		x = X;
		y = Y;
	}

	// Operators returning CDUIPoint values
	CDUIPoint operator +(SIZE size) const
	{
		return CDUIPoint(x + size.cx, y + size.cy);
	}

	CDUIPoint operator -(SIZE size) const
	{
		return CDUIPoint(x - size.cx, y - size.cy);
	}

	CDUIPoint operator -() const
	{
		return CDUIPoint(-x, -y);
	}

	CDUIPoint operator +(POINT point) const
	{
		return CDUIPoint(x + point.x, y + point.y);
	}

	// Operators returning CDUISize values
	CDUISize operator -(POINT point) const
	{
		return CDUISize(x - point.x, y - point.y);
	}

	// Operators returning CDUIRect values
	CDUIRect operator +(const RECT* lpRect) const;
	CDUIRect operator -(const RECT* lpRect) const;
};


///////////////////////////////////////////////////////////////////////////////
// CDUIRect - Wrapper for Windows RECT structure.

class XDBASE_API CDUIRect : public RECT
{
public:
	// Constructors
	CDUIRect()
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}

	CDUIRect(int l, int t, int r, int b)
	{
		left = l;
		top = t;
		right = r;
		bottom = b;
	}

	CDUIRect(const RECT& srcRect)
	{
		::CopyRect(this, &srcRect);
	}

	CDUIRect(LPCRECT lpSrcRect)
	{
		::CopyRect(this, lpSrcRect);
	}

	CDUIRect(POINT point, SIZE size)
	{
		right = (left = point.x) + size.cx;
		bottom = (top = point.y) + size.cy;
	}

	CDUIRect(POINT topLeft, POINT bottomRight)
	{
		left = topLeft.x;
		top = topLeft.y;
		right = bottomRight.x;
		bottom = bottomRight.y;
	}

	// Attributes (in addition to RECT members)
	int Width() const
	{
		return right - left;
	}

	int Height() const
	{
		return bottom - top;
	}

	CDUISize Size() const
	{
		return CDUISize(right - left, bottom - top);
	}

	CDUIPoint& TopLeft()
	{
		return *((CDUIPoint*)this);
	}

	CDUIPoint& BottomRight()
	{
		return *((CDUIPoint*)this + 1);
	}

	const CDUIPoint& TopLeft() const
	{
		return *((CDUIPoint*)this);
	}

	const CDUIPoint& BottomRight() const
	{
		return *((CDUIPoint*)this + 1);
	}

	CDUIPoint CenterPoint() const
	{
		return CDUIPoint((left + right) / 2, (top + bottom) / 2);
	}

	// convert between CDUIRect and LPRECT/LPCRECT (no need for &)
	operator LPRECT()
	{
		return this;
	}

	operator LPCRECT() const
	{
		return this;
	}

	BOOL IsRectEmpty() const
	{
		return ::IsRectEmpty(this);
	}

	BOOL IsRectNull() const
	{
		return (left == 0 && right == 0 && top == 0 && bottom == 0);
	}

	BOOL PtInRect(POINT point) const
	{
		return ::PtInRect(this, point);
	}

	// Operations
	void SetRect(int x1, int y1, int x2, int y2)
	{
		::SetRect(this, x1, y1, x2, y2);
	}

	void SetRect(POINT topLeft, POINT bottomRight)
	{
		::SetRect(this, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
	}

	void SetRectEmpty()
	{
		::SetRectEmpty(this);
	}

	void CopyRect(LPCRECT lpSrcRect)
	{
		::CopyRect(this, lpSrcRect);
	}

	BOOL EqualRect(LPCRECT lpRect) const
	{
		return ::EqualRect(this, lpRect);
	}

	void InflateRect(int x, int y)
	{
		::InflateRect(this, x, y);
	}

	void InflateRect(SIZE size)
	{
		::InflateRect(this, size.cx, size.cy);
	}

	void InflateRect(LPCRECT lpRect)
	{
		left -= lpRect->left;
		top -= lpRect->top;
		right += lpRect->right;
		bottom += lpRect->bottom;
	}

	void InflateRect(int l, int t, int r, int b)
	{
		left -= l;
		top -= t;
		right += r;
		bottom += b;
	}

	void DeflateRect(int x, int y)
	{
		::InflateRect(this, -x, -y);
	}

	void DeflateRect(SIZE size)
	{
		::InflateRect(this, -size.cx, -size.cy);
	}

	void DeflateRect(LPCRECT lpRect)
	{
		left += lpRect->left;
		top += lpRect->top;
		right -= lpRect->right;
		bottom -= lpRect->bottom;
	}

	void DeflateRect(int l, int t, int r, int b)
	{
		left += l;
		top += t;
		right -= r;
		bottom -= b;
	}

	void OffsetRect(int x, int y)
	{
		::OffsetRect(this, x, y);
	}
	void OffsetRect(SIZE size)
	{
		::OffsetRect(this, size.cx, size.cy);
	}

	void OffsetRect(POINT point)
	{
		::OffsetRect(this, point.x, point.y);
	}

	void NormalizeRect()
	{
		int nTemp;
		if (left > right)
		{
			nTemp = left;
			left = right;
			right = nTemp;
		}
		if (top > bottom)
		{
			nTemp = top;
			top = bottom;
			bottom = nTemp;
		}
	}

	// absolute position of rectangle
	void MoveToY(int y)
	{
		bottom = Height() + y;
		top = y;
	}

	void MoveToX(int x)
	{
		right = Width() + x;
		left = x;
	}

	void MoveToXY(int x, int y)
	{
		MoveToX(x);
		MoveToY(y);
	}

	void MoveToXY(POINT pt)
	{
		MoveToX(pt.x);
		MoveToY(pt.y);
	}

	// operations that fill '*this' with result
	BOOL IntersectRect(LPCRECT lpRect1, LPCRECT lpRect2)
	{
		return ::IntersectRect(this, lpRect1, lpRect2);
	}

	BOOL UnionRect(LPCRECT lpRect1, LPCRECT lpRect2)
	{
		return ::UnionRect(this, lpRect1, lpRect2);
	}

	BOOL SubtractRect(LPCRECT lpRectSrc1, LPCRECT lpRectSrc2)
	{
		return ::SubtractRect(this, lpRectSrc1, lpRectSrc2);
	}

	// Additional Operations
	void operator =(const RECT& srcRect)
	{
		::CopyRect(this, &srcRect);
	}

	BOOL operator ==(const RECT& rect) const
	{
		return ::EqualRect(this, &rect);
	}

	BOOL operator !=(const RECT& rect) const
	{
		return !::EqualRect(this, &rect);
	}

	void operator +=(POINT point)
	{
		::OffsetRect(this, point.x, point.y);
	}

	void operator +=(SIZE size)
	{
		::OffsetRect(this, size.cx, size.cy);
	}

	void operator +=(LPCRECT lpRect)
	{
		InflateRect(lpRect);
	}

	void operator -=(POINT point)
	{
		::OffsetRect(this, -point.x, -point.y);
	}

	void operator -=(SIZE size)
	{
		::OffsetRect(this, -size.cx, -size.cy);
	}

	void operator -=(LPCRECT lpRect)
	{
		DeflateRect(lpRect);
	}

	void operator &=(const RECT& rect)
	{
		::IntersectRect(this, this, &rect);
	}

	void operator |=(const RECT& rect)
	{
		::UnionRect(this, this, &rect);
	}

	// Operators returning CRect values
	CDUIRect operator +(POINT pt) const
	{
		CDUIRect rect(*this);
		::OffsetRect(&rect, pt.x, pt.y);
		return rect;
	}

	CDUIRect operator -(POINT pt) const
	{
		CDUIRect rect(*this);
		::OffsetRect(&rect, -pt.x, -pt.y);
		return rect;
	}

	CDUIRect operator +(LPCRECT lpRect) const
	{
		CDUIRect rect(this);
		rect.InflateRect(lpRect);
		return rect;
	}

	CDUIRect operator +(SIZE size) const
	{
		CDUIRect rect(*this);
		::OffsetRect(&rect, size.cx, size.cy);
		return rect;
	}

	CDUIRect operator -(SIZE size) const
	{
		CDUIRect rect(*this);
		::OffsetRect(&rect, -size.cx, -size.cy);
		return rect;
	}

	CDUIRect operator -(LPCRECT lpRect) const
	{
		CDUIRect rect(this);
		rect.DeflateRect(lpRect);
		return rect;
	}

	CDUIRect operator &(const RECT& rect2) const
	{
		CDUIRect rect;
		::IntersectRect(&rect, this, &rect2);
		return rect;
	}

	CDUIRect operator |(const RECT& rect2) const
	{
		CDUIRect rect;
		::UnionRect(&rect, this, &rect2);
		return rect;
	}

	CDUIRect MulDiv(int nMultiplier, int nDivisor) const
	{
		return CDUIRect(
			::MulDiv(left, nMultiplier, nDivisor),
			::MulDiv(top, nMultiplier, nDivisor),
			::MulDiv(right, nMultiplier, nDivisor),
			::MulDiv(bottom, nMultiplier, nDivisor));
	}

	// for Old Method;
	int GetWidth() const
	{
		return right - left;
	}

	int GetHeight() const
	{
		return bottom - top;
	}

	void Join(const RECT& rc)
	{
		if( rc.left < left ) left = rc.left;
		if( rc.top < top ) top = rc.top;
		if( rc.right > right ) right = rc.right;
		if( rc.bottom > bottom ) bottom = rc.bottom;
	}

	void Offset(int x, int y)
	{
		OffsetRect( x, y );
	}

	void ResetOffset()
	{
		::OffsetRect(this, -left, -top);
	}

	void Empty()
	{
		left = top = right = bottom = 0;
	}

};


// CDUISize implementation

inline CDUIPoint CDUISize::operator +(POINT point) const
{ return CDUIPoint(cx + point.x, cy + point.y); }

inline CDUIPoint CDUISize::operator -(POINT point) const
{ return CDUIPoint(cx - point.x, cy - point.y); }

inline CDUIRect CDUISize::operator +(const RECT* lpRect) const
{ return CDUIRect(lpRect) + *this; }

inline CDUIRect CDUISize::operator -(const RECT* lpRect) const
{ return CDUIRect(lpRect) - *this; }


// CDUIPoint implementation

inline CDUIRect CDUIPoint::operator +(const RECT* lpRect) const
{ return CDUIRect(lpRect) + *this; }

inline CDUIRect CDUIPoint::operator -(const RECT* lpRect) const
{ return CDUIRect(lpRect) - *this; }


}