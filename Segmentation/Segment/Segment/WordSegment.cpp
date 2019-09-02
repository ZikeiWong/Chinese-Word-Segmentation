#include "StdAfx.h"
#include "WordSegment.h"


unsigned short int GetItem(unsigned char char1,unsigned char char2)
{
	return (char1-176)*94+char2-161;;
}


//构造函数，构建存储词的空词库散列表
CWordSegment::CWordSegment():m_hash()
{
}

CWordSegment::~CWordSegment()
{
}



//后向查找匹配
int CWordSegment::SegBkDetectIt(unsigned char * s,int maxword)
{
	//不是汉字，则查找到最后一个非汉字，将偏移量返回
	int kk=0;
	while (s[kk]<0x80) kk++;

	if (kk)	return kk;

	//是汉字标点符号,则查找到最后一个汉字标点符号，偏移量 返回
	kk=0;
	int pos= GetItem(s[kk],s[kk+1]);
	while (s[kk]>=0x80 && (pos<0 || pos >3754)) {
		kk+=2;
		pos = GetItem(s[kk],s[kk+1]);
	}
	if (kk) return kk;

	//是汉字，往后找maxword个，若遇非汉字停止
	//从最大maxword开始匹配，成功则返回 偏移量，否则返回2
	kk=0;
	int wordbyte=0;
	while (kk <= maxword * 2 && s[kk]>=0x80 && pos>=0 && pos <= 3754){
		kk+=2;
		pos= GetItem(s[kk],s[kk+1]);
	}
	if (kk>maxword *2 ) { //找到maxword个汉字
		wordbyte=maxword *2 ;
	}
	else{ //找到 kk/2 个汉字
		wordbyte=kk;	
	}

	unsigned char txt[100]; 
	memcpy(txt,s,wordbyte);
	while(wordbyte>2){
		if (m_hash.find(txt,wordbyte)){
			return wordbyte;
		}
		wordbyte-=2 ;
	}
	//未找到
	return 2;
}
//向词库散列表添加新词，参数为文本词典文件名
int CWordSegment::DictAppendTxt(char * orig_txt_file)
{
	return m_hash.DictAppendTxt(orig_txt_file);
}


//对正文后向分词
void CWordSegment::SegBkWords(string & origtext, string & segresult, int maxword)
{
	int i=0;
	int len=origtext.size ();
	const char * text = origtext.c_str();
	int offset;
	while (i<len){
		if ( offset=SegBkDetectIt((unsigned char *)text+i,maxword)) // if not a word, offset =1
		{
			char word[100]={0};
			strncpy_s(word,text+i,offset);
			segresult += word;
			segresult += "|";
		}
		else 
			break;
		i += offset;
	}
}