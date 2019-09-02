// SegmentDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Segment.h"
#include "SegmentDlg.h"
#include "fstream"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// CSegmentDlg 对话框


int OpenFileDlg(char* szFile , HWND hwnd)
{
	OPENFILENAME   ofn; //通用对话框结构
	
	//初始化结构体 
	
	ZeroMemory(&ofn, sizeof(ofn)); 
	ofn.lStructSize =   sizeof(ofn); 
	ofn.hwndOwner   =   hwnd; 
	ofn.lpstrFile   =   szFile; 
	
	//    将 lpstrFile设置为空字符串  
	ofn.lpstrFile[0]   =   ('\0'); 
	ofn.nMaxFile   =   256;//sizeof(szFile); 
	ofn.lpstrFilter   =("txt file(txt)\0*.txt\0All\0*.*\0");
	ofn.nFilterIndex   =   1; 
	ofn.lpstrFileTitle   =   NULL; 
	ofn.nMaxFileTitle   =   0; 
	ofn.lpstrInitialDir   =   NULL; 
	ofn.Flags   =   OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	//   显示打开文件对话框.   
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


// CSegmentDlg 消息处理程序
void CSegmentDlg::SetItemPos(UINT id, int top, int bottom, int left, int right)
{
	CWnd * p= (CWnd * )GetDlgItem(id); //获得控件指针赋给父类指针变量
	CRect  rc;

	p->GetWindowRect(rc);   //取得控件的屏幕坐标
	ScreenToClient(rc);//将控件的屏幕坐标转化为相对于对话框客户区的相对坐标
	rc.top  = top;//设置控件的新坐标
	rc.bottom = bottom;
	rc.left = left;
	rc.right = right;
	p->MoveWindow(rc);//调整控件位置

}
void CSegmentDlg::SetAllItemsPos()
{
	CRect  rcDlg;
	GetClientRect(rcDlg);//取得对话框的客户区坐标
	int dlgWidth = rcDlg.right - rcDlg.left ; //取得对话框的宽度
	int dlgHeight = rcDlg.bottom - rcDlg.top ;//取得对话框的高度

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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_show = true;
	ModifyStyle(0, WS_SIZEBOX);
	SetAllItemsPos(); //设置对话框中各个控件的位置

	char dict_name[300]={0};
	GetModuleFileName(NULL,dict_name,300); 
	int i=strlen(dict_name)-1;
	while (dict_name[i]!='\\')	i--;
	dict_name[i+1]=0;
	strcat_s(dict_name,"dict.txt");
	try{
		int n = m_ws.DictAppendTxt(dict_name);
		char info[100];
		sprintf_s(info,"词典加载成功，共导入%d个词，请选择分词的文件",n);
		SetDlgItemText(IDC_STATIC1, info);
	}
	catch(char * s)
	{
		string p = dict_name;
		p += " ";
		p += s;
		AfxMessageBox(p.c_str ());
	}	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSegmentDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	ifs.seekg(0,std::ios::end);	//文件指针指向文件末尾
	int size = ifs.tellg ();//得到文件大小
	ifs.seekg (0,std::ios::beg);	//文件指针指向文件头
	
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
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}


void CSegmentDlg::OnBnClickedButton2()
{
	char content[2001]={0};
	GetDlgItemText(IDC_EDIT1, content,2000);
	string s=content;
	
	SetDlgItemText(IDC_EDIT2, s.c_str ());
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
}
