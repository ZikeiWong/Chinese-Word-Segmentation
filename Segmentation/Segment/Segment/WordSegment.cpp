#include "StdAfx.h"
#include "WordSegment.h"


unsigned short int GetItem(unsigned char char1,unsigned char char2)
{
	return (char1-176)*94+char2-161;;
}


//���캯���������洢�ʵĿմʿ�ɢ�б�
CWordSegment::CWordSegment():m_hash()
{
}

CWordSegment::~CWordSegment()
{
}



//�������ƥ��
int CWordSegment::SegBkDetectIt(unsigned char * s,int maxword)
{
	//���Ǻ��֣�����ҵ����һ���Ǻ��֣���ƫ��������
	int kk=0;
	while (s[kk]<0x80) kk++;

	if (kk)	return kk;

	//�Ǻ��ֱ�����,����ҵ����һ�����ֱ����ţ�ƫ���� ����
	kk=0;
	int pos= GetItem(s[kk],s[kk+1]);
	while (s[kk]>=0x80 && (pos<0 || pos >3754)) {
		kk+=2;
		pos = GetItem(s[kk],s[kk+1]);
	}
	if (kk) return kk;

	//�Ǻ��֣�������maxword���������Ǻ���ֹͣ
	//�����maxword��ʼƥ�䣬�ɹ��򷵻� ƫ���������򷵻�2
	kk=0;
	int wordbyte=0;
	while (kk <= maxword * 2 && s[kk]>=0x80 && pos>=0 && pos <= 3754){
		kk+=2;
		pos= GetItem(s[kk],s[kk+1]);
	}
	if (kk>maxword *2 ) { //�ҵ�maxword������
		wordbyte=maxword *2 ;
	}
	else{ //�ҵ� kk/2 ������
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
	//δ�ҵ�
	return 2;
}
//��ʿ�ɢ�б�����´ʣ�����Ϊ�ı��ʵ��ļ���
int CWordSegment::DictAppendTxt(char * orig_txt_file)
{
	return m_hash.DictAppendTxt(orig_txt_file);
}


//�����ĺ���ִ�
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