#pragma once
#include "string"
#include "hash.h"


class CWordSegment
{
public:
	//���캯���������洢�ʵĿմʿ�ɢ�б�
	CWordSegment(void);
	//����ִʲ���
	void SegBkWords(string &text, string &segresult, int maxword=6);
	//��������
	~CWordSegment(void);
	//��ʿ�ɢ�б�����´ʣ�����Ϊ�ı��ʵ��ļ���
	int DictAppendTxt(char * orig_txt_file); 

private:
	int SegBkDetectIt(unsigned char * s,int maxword);	
	CHash m_hash;
};
