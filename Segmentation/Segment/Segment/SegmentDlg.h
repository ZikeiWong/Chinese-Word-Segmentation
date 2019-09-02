// SegmentDlg.h : ͷ�ļ�
//

#pragma once

#include "WordSegment.h"

// CSegmentDlg �Ի���
class CSegmentDlg : public CDialog
{
// ����
public:
	CSegmentDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SEGMENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	void SetAllItemsPos();
	void SetItemPos(UINT id, int top, int bottom, int left, int right);
	CWordSegment m_ws;
	bool m_show;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFileChoice();
	afx_msg void OnBnClickedOk();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedCancel();
};
