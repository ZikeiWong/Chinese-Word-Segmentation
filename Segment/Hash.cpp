#include "StdAfx.h"
#include "Hash.h"
#include "GetWord.h"

#define ROOTNUM  1048576

int bitlen[]={7,7,6,1,7,7,5,2,7,7,4,3,7,7,3,4,7,7,2,5,7,7,1,6,7,7};
int wpos[]={0,0,0,6,0,0,0,5,0,0,0,4,0,0,0,3,0,0,0,2,0,0,0,1,0,0};

CHash::CHash(void)
{
	m_DictNum = 0;
	root=new Root [ROOTNUM]; //共计256K个单元,1.5MByte.
	if (!root){
		throw "内存不足，无法开辟为Root[262144]开辟空间！";
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
unsigned int CHash::GetWordAddr(unsigned char * word,int slen)//slen>=4，字节数
{ //按教材，20bit处理
	int address=0;
	int bytenumber=0;
	int n=slen / 2; //汉字个数
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
//在Root的next位置上插入新结点
void CHash::InsertNode(Root *  p ,unsigned char * hz, int n)
{
	Subnode *s = new Subnode;
	s->cchar = new unsigned char [n];
	memcpy(s->cchar, hz, n);
	s->number = n;
	s->next = p->next;
	p->next = s;
}

//在p的位置后插入新结点
void CHash::InsertNode(Subnode *  p ,unsigned char * hz, int n)
{
	Subnode *s = new Subnode;
	s->cchar = new unsigned char [n];
	memcpy(s->cchar, hz, n);
	s->number = n;
	s->next = p->next;
	p->next  = s;
}


//释放内存中的词库表，由析构函数调用。



//添加文本文件中的所有词到内存中的哈希表
int CHash::DictAppendTxt(char * orig_txt_file)
{
	if (!root){
		throw "原始词库未加载，不能追加词库！";
	}
	//1.打开文件
	CGetWord a(orig_txt_file);
	if (!a.good()) {
		throw "原始词典文件不能打开，可能是路径设置错误！";
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
		char ssss[]="强调";
		if (!memcmp(ssss,hz,4))
		{
			int debug=10;
		}
		unsigned int cursor=GetWordAddr(hz,wordbyte);
		if (cursor>=ROOTNUM){
			bad++;continue;}		 
		
		//考虑到长度短的词多，因此长度短的靠前，长度长的词靠后，进行中间插入操作
		Subnode *p,*q;
		p=root[cursor].next ;//此时p肯定不为空
		int found = 0;
		while (p){
			if (wordbyte > p->number || 
				(wordbyte==p->number && 
				memcmp(p->cchar , hz , wordbyte) < 0 )){//p小，p需要向后移动
					q=p, p=p->next ;						
			}
			else if (wordbyte == p->number && !memcmp(p->cchar , hz , wordbyte )){
				//有重复
				found = 1;
				break;
			}
			else {
				//其他情况，拉链中肯定没有该词
				break;
			}
		}
		if (!found) {//未找到重复词,需要添加该节点
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
				//找到
				return true;
		}
		else if (p->number < wordbyte ||
			(p->number  == wordbyte && 
			memcmp(txt,p->cchar,p->number )>0 )){
				//需要继续向后移动
				p=p->next;
		}
		else{//未找到
			return false;
		}
	}
	return false;
}