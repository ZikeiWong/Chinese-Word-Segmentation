#pragma once
#include "string"
#include "hash.h"


class CWordSegment
{
public:
	//构造函数，构建存储词的空词库散列表
	CWordSegment(void);
	//后向分词操作
	void SegBkWords(string &text, string &segresult, int maxword=6);
	//析构函数
	~CWordSegment(void);
	//向词库散列表添加新词，参数为文本词典文件名
	int DictAppendTxt(char * orig_txt_file); 
	void SegBkWords1(string & text, string & segresult, int maxword=6);

private:
	int SegBkDetectIt(unsigned char * s,int maxword);
	int SegBkDetectIt1(unsigned char * s,int maxword,int len);
	CHash m_hash;
};
