// GetWord.cpp: implementation of the CGetWord class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GetWord.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGetWord::CGetWord(string filename)
{
	std::locale::global(std::locale(""));
	a.open (filename.c_str () );
}

CGetWord::~CGetWord()
{
	a.close();
}
bool CGetWord::GetNextWord( unsigned char * word)
{
	word[0]=0;
	unsigned char hz[50]={0};
	int offset = a.tellg();
	a.read ((char *)hz,50);
	if (hz[0]==',' && hz[1]==0xfd && hz[2]==0xb6 ){
		int debug=0;
	}
	if (!a.good()) return 0;
	int n = strlen((char *)hz);

	//3.1 get the first character byte
		int begin;
		int j=0;
		while(hz[j]<0x80 ){ //is not a chinese character 
			if(j<n)	
				j++;
			else 
				break;
		}
		if (j>=n) { //exception
			offset+=n-1;
			a.seekg(offset);
			return 1;
		}
		begin=j;
		//3.2 get the end character byte
 		int end;
		while(hz[j]>=0x80 ){ //is not a chinese character 
			if(j<n)	
				j+=2;
			else 
				break;
		}
		if (j>=n) { //exception
			offset+=1;
			a.seekg(offset);
			return 1;
		}
		end=j-1;
		//3.3 get the word length
		int wordlength=(end+1-begin)/2; //该词的长度
		if (wordlength<2||wordlength>100){
			offset+=end+1;
			a.seekg(offset);
			return 1;
		}
		
		//3.4 get the word content
		memcpy(word,hz+begin,wordlength*2);		
		word[wordlength * 2 ] =0;
		offset+=end+1;
		a.seekg(offset);
		return 1;
}

bool CGetWord::good()
{
	return a.good();
}
