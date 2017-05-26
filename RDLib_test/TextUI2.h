#pragma once
class CTextUI2 : public CTextUI
{
public:
	CTextUI2(void);
	virtual ~CTextUI2(void);

	SIZE EstimateSize(SIZE szAvailable);
};

