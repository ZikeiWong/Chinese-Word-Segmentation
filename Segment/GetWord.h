// GetWord.h: interface for the CGetWord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GETWORD_H__33638CB9_7A1E_4260_AFAC_B5A22FCE537E__INCLUDED_)
#define AFX_GETWORD_H__33638CB9_7A1E_4260_AFAC_B5A22FCE537E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "fstream"
using namespace std;
class CGetWord  
{
public:
	bool good();
	CGetWord(string filename);
	virtual ~CGetWord();
	bool GetNextWord(unsigned char * word);
private:
	ifstream a;
};

#endif // !defined(AFX_GETWORD_H__33638CB9_7A1E_4260_AFAC_B5A22FCE537E__INCLUDED_)
