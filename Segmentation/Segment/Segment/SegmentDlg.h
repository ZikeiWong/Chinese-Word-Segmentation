// SegmentDlg.h : 头文件
//

#pragma once

#include "WordSegment.h"

// CSegmentDlg 对话框
class CSegmentDlg : public CDialog
{
// 构造
public:
	CSegmentDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SEGMENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	void SetAllItemsPos();
	void SetItemPos(UINT id, int top, int bottom, int left, int right);
	CWordSegment m_ws;
	bool m_show;

	// 生成的消息映射函数
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
