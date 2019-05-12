#pragma once

#include "DownloadListCtrl.h"
// CDownloadMgrDlg 对话框

class CDownloadMgrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDownloadMgrDlg)

public:
	CDownloadMgrDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDownloadMgrDlg();

	CDownloadListCtrl& GetDownList() { return m_down_list; }

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DOWN_MGR_DLG };
#endif

public:


protected:
	CDownloadListCtrl m_down_list;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
