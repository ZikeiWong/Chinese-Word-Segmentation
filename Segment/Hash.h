#pragma once
using namespace std;
typedef struct snode{
	unsigned char number; //该词含有的中文汉字个数，可取值为：2,3,4,5,6等。
	unsigned char * cchar;//该词可有N个汉字
	struct snode * next; //下个节点的位置
}Subnode; //8byte

typedef struct {
	unsigned short int number; //该地址下所有词的数目
	Subnode * next; //表头地址
}Root; //6 byte，共计ROOTNUM个元素

class CHash
{
public:
	CHash(void);
	~CHash(void);
	//向词库散列表添加新词，参数为文本词典文件名
	int DictAppendTxt(char * orig_txt_file); 
	bool find(unsigned char *txt,int wordbyte);
private:
	void CHash::putByte(int & group,int gpos,unsigned char word, int wpos, int bitlen);
	unsigned int CHash::GetWordAddr(unsigned char * word,int slen);
	void DictRelease(); //释放内存中的词库表，由析构函数调用。

	void InsertNode(Subnode *  p ,unsigned char * hz, int n);
	void InsertNode(Root *  p ,unsigned char * hz, int n);
	Root * root; //词库散列表地址
	int m_DictNum;//词库中词的数量
};
