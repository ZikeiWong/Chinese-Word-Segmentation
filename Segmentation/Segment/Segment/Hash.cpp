#include "StdAfx.h"
#include "Hash.h"
#include "GetWord.h"

#define ROOTNUM  1048576

int bitlen[]={7,7,6,1,7,7,5,2,7,7,4,3,7,7,3,4,7,7,2,5,7,7,1,6,7,7};
int wpos[]={0,0,0,6,0,0,0,5,0,0,0,4,0,0,0,3,0,0,0,2,0,0,0,1,0,0};

CHash::CHash(void)
{
	m_DictNum = 0;
	root=new Root [ROOTNUM]; //����256K����Ԫ,1.5MByte.
	if (!root){
		throw "�ڴ治�㣬�޷�����ΪRoot[262144]���ٿռ䣡";
	}
	memset (root,0,ROOTNUM*sizeof (Root));
}

CHash::~CHash(void)
{
	DictRelease();
}

void CHash::putByte(int & group,int gpos,unsigned char word, int wpos, int bitlen)
{
			int temp = (word & (0x7f>> wpos)) >> (7-wpos-bitlen);
			temp <<= 20-gpos-bitlen;
			group |= temp;	
}
unsigned int CHash::GetWordAddr(unsigned char * word,int slen)//slen>=4���ֽ���
{ //���̲ģ�20bit����
	int address=0;
	int bytenumber=0;
	int n=slen / 2; //���ָ���
	if (n>10) n=10;
	int calnumber = 14 * n;
	int p = bytenumber;
	int gpos=0;
	for (int i=0;i<calnumber;i+=20){
		int group = 0;
		gpos=0;
		for (int j=0;j<3;j++){
			putByte(group,gpos,word[bytenumber], wpos[p], bitlen[p]);
			gpos += bitlen[p];
			bytenumber++;
			if (bytenumber>=slen) 
				return address^group;
			p++;
		}
		if (bitlen[p-1]==7){
			putByte(group,gpos,word[bytenumber], wpos[p], bitlen[p]);
			gpos+=bitlen[p];
			p++;
		}
		else
			bytenumber--;
		address ^= group;
	}
	return address;
}
void CHash::DictRelease()
{
	if (root){
		for (int i=0;i<ROOTNUM;i++){
			Subnode * p = root[i].next;
			while (p){
				root[i].next = p->next;
				delete [] p->cchar ;
				delete p;
				p = root[i].next;
			}
		}
		delete [] root;
	}
}
//��Root��nextλ���ϲ����½��
void CHash::InsertNode(Root *  p ,unsigned char * hz, int n)
{
	Subnode *s = new Subnode;
	s->cchar = new unsigned char [n];
	memcpy(s->cchar, hz, n);
	s->number = n;
	s->next = p->next;
	p->next = s;
}

//��p��λ�ú�����½��
void CHash::InsertNode(Subnode *  p ,unsigned char * hz, int n)
{
	Subnode *s = new Subnode;
	s->cchar = new unsigned char [n];
	memcpy(s->cchar, hz, n);
	s->number = n;
	s->next = p->next;
	p->next  = s;
}


//�ͷ��ڴ��еĴʿ���������������á�



//����ı��ļ��е����дʵ��ڴ��еĹ�ϣ��
int CHash::DictAppendTxt(char * orig_txt_file)
{
	if (!root){
		throw "ԭʼ�ʿ�δ���أ�����׷�Ӵʿ⣡";
	}
	//1.���ļ�
	CGetWord a(orig_txt_file);
	if (!a.good()) {
		throw "ԭʼ�ʵ��ļ����ܴ򿪣�������·�����ô���";
	}

	//3.get every word to the hash list
	unsigned char hz[20]={0};
	int i=0; //word number
	int offset = 0;
	int num=0;
	int bad=0;
	while (a.GetNextWord (hz)) {
		i++;
		int wordbyte =  strlen((char *) hz) ;
		if (wordbyte< 4) 	{
			bad++;continue;}

		//3.1 get the address
		char ssss[]="ǿ��";
		if (!memcmp(ssss,hz,4))
		{
			int debug=10;
		}
		unsigned int cursor=GetWordAddr(hz,wordbyte);
		if (cursor>=ROOTNUM){
			bad++;continue;}		 
		
		//���ǵ����ȶ̵Ĵʶ࣬��˳��ȶ̵Ŀ�ǰ�����ȳ��Ĵʿ��󣬽����м�������
		Subnode *p,*q;
		p=root[cursor].next ;//��ʱp�϶���Ϊ��
		int found = 0;
		while (p){
			if (wordbyte > p->number || 
				(wordbyte==p->number && 
				memcmp(p->cchar , hz , wordbyte) < 0 )){//pС��p��Ҫ����ƶ�
					q=p, p=p->next ;						
			}
			else if (wordbyte == p->number && !memcmp(p->cchar , hz , wordbyte )){
				//���ظ�
				found = 1;
				break;
			}
			else {
				//��������������п϶�û�иô�
				break;
			}
		}
		if (!found) {//δ�ҵ��ظ���,��Ҫ��Ӹýڵ�
			if (p==root[cursor].next)
				InsertNode(&root[cursor] , hz, wordbyte);
			else
				InsertNode(q , hz, wordbyte);

			root[cursor].number ++;
			num++;
		}
	}	
	return num;
}

bool CHash::find(unsigned char *txt,int wordbyte)
{
	unsigned int cursor=GetWordAddr(txt,wordbyte);
	Subnode * p = root[cursor].next;
	while (p){
		if (p->number == wordbyte && 
			!memcmp(txt,p->cchar,p->number)) {
				//�ҵ�
				return true;
		}
		else if (p->number < wordbyte ||
			(p->number  == wordbyte && 
			memcmp(txt,p->cchar,p->number )>0 )){
				//��Ҫ��������ƶ�
				p=p->next;
		}
		else{//δ�ҵ�
			return false;
		}
	}
	return false;
}