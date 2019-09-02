#pragma once
using namespace std;
typedef struct snode{
	unsigned char number; //�ôʺ��е����ĺ��ָ�������ȡֵΪ��2,3,4,5,6�ȡ�
	unsigned char * cchar;//�ôʿ���N������
	struct snode * next; //�¸��ڵ��λ��
}Subnode; //8byte

typedef struct {
	unsigned short int number; //�õ�ַ�����дʵ���Ŀ
	Subnode * next; //��ͷ��ַ
}Root; //6 byte������ROOTNUM��Ԫ��

class CHash
{
public:
	CHash(void);
	~CHash(void);
	//��ʿ�ɢ�б�����´ʣ�����Ϊ�ı��ʵ��ļ���
	int DictAppendTxt(char * orig_txt_file); 
	bool find(unsigned char *txt,int wordbyte);
private:
	void CHash::putByte(int & group,int gpos,unsigned char word, int wpos, int bitlen);
	unsigned int CHash::GetWordAddr(unsigned char * word,int slen);
	void DictRelease(); //�ͷ��ڴ��еĴʿ���������������á�

	void InsertNode(Subnode *  p ,unsigned char * hz, int n);
	void InsertNode(Root *  p ,unsigned char * hz, int n);
	Root * root; //�ʿ�ɢ�б��ַ
	int m_DictNum;//�ʿ��дʵ�����
};
