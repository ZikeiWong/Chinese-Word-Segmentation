// SegmentDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Segment.h"
#include "SegmentDlg.h"
#include "fstream"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// CSegmentDlg �Ի���


int OpenFileDlg(char* szFile , HWND hwnd)
{
	OPENFILENAME   ofn; //ͨ�öԻ���ṹ
	
	//��ʼ���ṹ�� 
	
	ZeroMemory(&ofn, sizeof(ofn)); 
	ofn.lStructSize =   sizeof(ofn); 
	ofn.hwndOwner   =   hwnd; 
	ofn.lpstrFile   =   szFile; 
	
	//    �� lpstrFile����Ϊ���ַ���  
	ofn.lpstrFile[0]   =   ('\0'); 
	ofn.nMaxFile   =   256;//sizeof(szFile); 
	ofn.lpstrFilter   =("txt file(txt)\0*.txt\0All\0*.*\0");
	ofn.nFilterIndex   =   1; 
	ofn.lpstrFileTitle   =   NULL; 
	ofn.nMaxFileTitle   =   0; 
	ofn.lpstrInitialDir   =   NULL; 
	ofn.Flags   =   OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	//   ��ʾ���ļ��Ի���.   
	if   (GetOpenFileName(&ofn)==TRUE)   
		return 0;
	else
	{
		int err = CommDlgExtendedError();
		return -1;
	}
}


CSegmentDlg::CSegmentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSegmentDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_show = false;
}

void CSegmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSegmentDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CSegmentDlg::OnBnClickedFileChoice)
	ON_BN_CLICKED(IDOK, &CSegmentDlg::OnBnClickedOk)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDCANCEL, &CSegmentDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON2, &CSegmentDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSegmentDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CSegmentDlg ��Ϣ�������
void CSegmentDlg::SetItemPos(UINT id, int top, int bottom, int left, int right)
{
	CWnd * p= (CWnd * )GetDlgItem(id); //��ÿؼ�ָ�븳������ָ�����
	CRect  rc;

	p->GetWindowRect(rc);   //ȡ�ÿؼ�����Ļ����
	ScreenToClient(rc);//���ؼ�����Ļ����ת��Ϊ����ڶԻ���ͻ������������
	rc.top  = top;//���ÿؼ���������
	rc.bottom = bottom;
	rc.left = left;
	rc.right = right;
	p->MoveWindow(rc);//�����ؼ�λ��

}
void CSegmentDlg::SetAllItemsPos()
{
	CRect  rcDlg;
	GetClientRect(rcDlg);//ȡ�öԻ���Ŀͻ�������
	int dlgWidth = rcDlg.right - rcDlg.left ; //ȡ�öԻ���Ŀ��
	int dlgHeight = rcDlg.bottom - rcDlg.top ;//ȡ�öԻ���ĸ߶�

	SetItemPos(IDC_STATIC1,10,30,10,dlgWidth-70);
	SetItemPos(IDC_STATIC2,30,50,10,dlgWidth-70);
	SetItemPos(IDC_EDIT1,  50,dlgHeight/2-5, 10,dlgWidth-70);
	SetItemPos(IDC_EDIT2, dlgHeight/2+5,dlgHeight-10, 10,dlgWidth-70);

	SetItemPos(IDC_BUTTON1, 30, 60, dlgWidth-60, dlgWidth-10);
	SetItemPos(IDOK, dlgHeight/2-15,dlgHeight/2+15, dlgWidth-60, dlgWidth-10);
	SetItemPos(IDCANCEL, dlgHeight-40,dlgHeight-10, dlgWidth-60, dlgWidth-10);
    InvalidateRect(NULL,TRUE);
}

BOOL CSegmentDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_show = true;
	ModifyStyle(0, WS_SIZEBOX);
	SetAllItemsPos(); //���öԻ����и����ؼ���λ��

	char dict_name[300]={0};
	GetModuleFileName(NULL,dict_name,300); 
	int i=strlen(dict_name)-1;
	while (dict_name[i]!='\\')	i--;
	dict_name[i+1]=0;
	strcat_s(dict_name,"dict.txt");
	try{
		int n = m_ws.DictAppendTxt(dict_name);
		char info[100];
		sprintf_s(info,"�ʵ���سɹ���������%d���ʣ���ѡ��ִʵ��ļ�",n);
		SetDlgItemText(IDC_STATIC1, info);
	}
	catch(char * s)
	{
		string p = dict_name;
		p += " ";
		p += s;
		AfxMessageBox(p.c_str ());
	}	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSegmentDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSegmentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSegmentDlg::OnBnClickedFileChoice()
{
	std::locale::global(std::locale(""));
	char filename[256];
	if (OpenFileDlg(filename,NULL)) return;
	SetDlgItemText(IDC_STATIC2,filename);
	std::ifstream ifs(filename,std::ios::binary);
	if (ifs.bad()) throw "file open failed.";
	ifs.seekg(0,std::ios::end);	//�ļ�ָ��ָ���ļ�ĩβ
	int size = ifs.tellg ();//�õ��ļ���С
	ifs.seekg (0,std::ios::beg);	//�ļ�ָ��ָ���ļ�ͷ
	
	char * content = new char [size+1];
	content[size]=0;
	ifs.read (content,size);
	ifs.close ();
	SetDlgItemText(IDC_EDIT1, content);
	delete [] content;
}

void CSegmentDlg::OnBnClickedOk()
{
	char content[2001]={0};
	GetDlgItemText(IDC_EDIT1, content,2000);
	string s=content;
	string r;
	try{
		m_ws.SegBkWords(s,r);
	}
	catch(char * s)
	{
		AfxMessageBox(s);
	}	
	SetDlgItemText(IDC_EDIT2, r.c_str ());
}

void CSegmentDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (m_show){
		SetAllItemsPos();
	}
}

void CSegmentDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}


void CSegmentDlg::OnBnClickedButton2()
{
	char content[2001]={0};
	GetDlgItemText(IDC_EDIT1, content,2000);
	string s=content;
	
	SetDlgItemText(IDC_EDIT2, s.c_str ());
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CSegmentDlg::OnBnClickedButton3()
{
	char content[2001]={0};
	GetDlgItemText(IDC_EDIT1, content,2000);
	string s=content;
	string r;
	try{
		m_ws.SegBkWords1(s,r,6);
	}
	catch(char * s)
	{
		AfxMessageBox(s);
	}	
	SetDlgItemText(IDC_EDIT2, r.c_str ());
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
