#pragma once

#include "DownloadListCtrl.h"
#include "FormatConvertDlg.h"

// CDownloadMgrDlg 对话框

class CDownloadMgrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDownloadMgrDlg)

public:
	CDownloadMgrDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDownloadMgrDlg();

	CDownloadListCtrl& GetDownListCtrl() { return m_down_list; }


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DOWN_MGR_DLG };
#endif

public:
	afx_msg void OnNMRClickPlaylistList(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnFormatConvert();
	afx_msg void OnItemExploreTrack();
	afx_msg void OnItemProperty();
	afx_msg void OnPlayItem();

	virtual BOOL OnInitDialog();

	CMenu m_list_popup_menu;		//播放列表右键菜单
	CFormatConvertDlg* m_pFormatConvertDlg{NULL};		//格式转换对话框（非模态对话框）
protected:
	CDownloadListCtrl m_down_list;
	int m_item_selected{};		//播放列表中鼠标选中的项目
	HWND m_hParent{};
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
