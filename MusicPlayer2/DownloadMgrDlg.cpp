// DownloadMgrDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "DownloadMgrDlg.h"
#include "afxdialogex.h"


// CDownloadMgrDlg 对话框

IMPLEMENT_DYNAMIC(CDownloadMgrDlg, CDialogEx)

CDownloadMgrDlg::CDownloadMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DOWN_MGR_DLG, pParent)
{

}

CDownloadMgrDlg::~CDownloadMgrDlg()
{
}

void CDownloadMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DOWN_LIST, m_down_list);

}


BEGIN_MESSAGE_MAP(CDownloadMgrDlg, CDialogEx)
END_MESSAGE_MAP()


// CDownloadMgrDlg 消息处理程序
