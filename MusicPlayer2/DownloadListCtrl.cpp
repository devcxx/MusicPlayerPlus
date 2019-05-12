#include "stdafx.h"
#include "DownloadListCtrl.h"
#include "MusicPlayer2.h"
#include "downloader/Downloader.h"
#include "strcpcvt.h"
#include "utils.h"
// CPlayListCtrl

IMPLEMENT_DYNAMIC(CDownloadListCtrl, CListCtrlEx)

BEGIN_MESSAGE_MAP(CDownloadListCtrl, CListCtrlEx)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, CDownloadListCtrl::OnNMCustomdraw)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CListCtrlEx::OnNMCustomdraw)
END_MESSAGE_MAP()

CDownloadListCtrl::CDownloadListCtrl()
{
	m_toolTip.Create(this, TTS_ALWAYSTIP | TTS_NOPREFIX);

}


CDownloadListCtrl::~CDownloadListCtrl()
{
}

void CDownloadListCtrl::AdjustColumnWidth()
{
	vector<int> width;
	CalculateColumeWidth(width);

	for (size_t i{}; i < width.size(); i++)
		SetColumnWidth(i, width[i]);
}

void progress_callback(void *userdata, double downloadSpeed, double remainingTime, double progressPercentage)
{
	DownloadItem* item = static_cast<DownloadItem*>(userdata);
	CDownloadListCtrl* ctrl = static_cast<CDownloadListCtrl*>(item->ctrl);
	CString strPercent;
	strPercent.Format(_T("%d%%"), (int)progressPercentage * 100);
	ctrl->SetItemText(item->id, 3, strPercent);
}

void CDownloadListCtrl::Download(const SongInfo& si)
{
	std::wstring app_path = CPlayer::GetInstance().GetAppPath();

	CFilePathHelper path_helper(si.file_name);
	std::wstring ext = path_helper.GetFileExtension();
	std::wstring down_folder = app_path + L"songs//";
	std::wstring filename = si.title + L"." + ext;

	int newRow = GetItemCount();

	CString strID,strLength;
	strID.Format(_T("%u"), newRow+1);
	char fileSize[MAX_PATH] = { 0 };
	utils::bytesToSize(si.filesize, fileSize, MAX_PATH);
// 	strLength.Format(_T("%d"), si.filesize);
	InsertItem(newRow, strID);
	SetItemText(newRow, 1, (si.title).c_str());
	SetItemText(newRow, 2, cvt::s2ws(fileSize).c_str());

	DownItemPtr item( new DownloadItem);
	item->ctrl = this;
	item->id = newRow;

	m_downItems.push_back(item);

	Downloader::instance()->download(cvt::ws2s(si.file_name), cvt::ws2s(down_folder), cvt::ws2s(filename), item.get(), progress_callback);

	m_all_song_info.push_back(si);

// 	UpdateDownList();

}

void CDownloadListCtrl::UpdateDownList()
{
	CString str;
	DeleteAllItems();
	for (int i{}; i < m_all_song_info.size(); i++)
	{
		str.Format(_T("%u"), i + 1);
		InsertItem(i, str);
		SetItemText(i, 1, (m_all_song_info[i].title).c_str());
		SetItemText(i, 2, (m_all_song_info[i].file_name).c_str());
	}
}

void CDownloadListCtrl::CalculateColumeWidth(vector<int>& width)
{
	width.resize(3);

	width[0] = theApp.DPI(40);
	width[2] = theApp.DPI(50);
	CRect rect;
	GetWindowRect(rect);
	width[1] = rect.Width() - width[0] - width[2] - theApp.DPI(20) - 1;

}

void CDownloadListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//如果开启文本提示
	if (m_bEnableTips)
	{
		CString str_tip;
		LVHITTESTINFO lvhti;

		// 判断鼠标当前所在的位置(行, 列)
		lvhti.pt = point;
		SubItemHitTest(&lvhti);

		//如果鼠标移动到另一行, 则进行处理; 否则, 不做处理
		if (lvhti.iItem != m_nItem)
		{
			// 保存当前鼠标所在的行
			m_nItem = lvhti.iItem;

			// 如果鼠标移动到一个合法的行，则显示新的提示信息，否则不显示提示
			if (m_nItem >= 0 && m_nItem < static_cast<int>(m_all_song_info.size()))
			{
				CString dis_str = GetItemText(m_nItem, 1);
				int strWidth = GetStringWidth(dis_str) + theApp.DPI(10);		//获取要显示当前字符串的最小宽度
				int columnWidth = GetColumnWidth(1);	//获取鼠标指向列的宽度
				if (columnWidth < strWidth)		//当单元格内的的字符无法显示完全时在提示的第1行显示单元格内文本
				{
					str_tip += dis_str;
					str_tip += _T("\r\n");
				}
				int song_index;
				if (!m_searched)
				{
					song_index = m_nItem;
				}
				else
				{
					CString str = GetItemText(m_nItem, 0);
					song_index = _ttoi(str) - 1;
				}
				if (song_index < 0 || song_index >= static_cast<int>(m_all_song_info.size()))
					return;
				str_tip += CCommon::LoadText(IDS_FILE_NAME, _T(": "));
				str_tip += m_all_song_info[song_index].file_name.c_str();
				str_tip += _T("\r\n");

				str_tip += CCommon::LoadText(IDS_TITLE, _T(": "));
				str_tip += m_all_song_info[song_index].title.c_str();
				str_tip += _T("\r\n");

				str_tip += CCommon::LoadText(IDS_ARTIST, _T(": "));
				str_tip += m_all_song_info[song_index].artist.c_str();
				str_tip += _T("\r\n");

				str_tip += CCommon::LoadText(IDS_ALBUM, _T(": "));
				str_tip += m_all_song_info[song_index].album.c_str();
				str_tip += _T("\r\n");

				CString str_bitrate;
				str_bitrate.Format(CCommon::LoadText(IDS_BITRATE, _T(": %dkbps")), m_all_song_info[song_index].bitrate);
				str_tip += str_bitrate;

				m_toolTip.SetMaxTipWidth(theApp.DPI(400));		//设置提示信息的宽度，以支持提示换行

				m_toolTip.AddTool(this, str_tip);
				m_toolTip.Pop();			// 显示提示框
			}
			else
			{
				m_toolTip.AddTool(this, _T(""));
				m_toolTip.Pop();
			}
		}
	}
	CListCtrlEx::OnMouseMove(nFlags, point);
}


BOOL CDownloadListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_toolTip.GetSafeHwnd() && pMsg->message == WM_MOUSEMOVE)
	{
		m_toolTip.RelayEvent(pMsg);
	}

	//按Ctrl+A全选
	if ((GetKeyState(VK_CONTROL) & 0x80) && (pMsg->wParam == 'A'))
	{
		SelectAll();
		return TRUE;
	}

	return CListCtrlEx::PreTranslateMessage(pMsg);
}


void CDownloadListCtrl::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	//CWindowDC dc(this);
	//HDC hDC = dc.GetSafeHdc();
	//m_dpi = GetDeviceCaps(hDC, LOGPIXELSY);

	CListCtrlEx::PreSubclassWindow();

	//将提示信息设为置顶
	m_toolTip.SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	//初始化播放列表
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	vector<int> width;
	CalculateColumeWidth(width);
	InsertColumn(0, CCommon::LoadText(IDS_NUMBER), LVCFMT_LEFT, width[0]);		//插入第1列
	InsertColumn(1, CCommon::LoadText(IDS_TRACK), LVCFMT_LEFT, width[1]);		//插入第2列
	InsertColumn(2, _T("大小"), LVCFMT_LEFT, width[2]);		//插入第3列
	InsertColumn(3, _T("进度"), LVCFMT_LEFT, width[2]);		//插入第3列
	EnableTip();

	SetRowHeight(theApp.DPI(24));

}


void CDownloadListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = CDRF_DODEFAULT;
	LPNMLVCUSTOMDRAW lplvdr = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	NMCUSTOMDRAW& nmcd = lplvdr->nmcd;
	static bool this_item_select = false;
	switch (lplvdr->nmcd.dwDrawStage)	//判断状态   
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:			//如果为画ITEM之前就要进行颜色的改变
		if (IsWindowEnabled())
		{
			this_item_select = false;
			if (m_searched && m_search_result.size() == 0)		//如果播放列表处于搜索状态且没有搜索结果
			{
				if (GetItemState(nmcd.dwItemSpec, LVIS_SELECTED) == LVIS_SELECTED)	//不允许选中行
					SetItemState(nmcd.dwItemSpec, 0, LVIS_SELECTED);
				lplvdr->clrText = GRAY(140);
				lplvdr->clrTextBk = GRAY(255);
			}
			else
			{
				int hightlight_item;
				if (!m_searched || m_search_result.size() == m_all_song_info.size())	//当播放列表不处理搜索状态，或搜索结果数量等于播放列表中曲目数量时
				{
					hightlight_item = m_highlight_item;
				}
				else		//如果播放列表处于搜索状态，则高亮项目应该为搜索结果的索引
				{
					auto iter = std::find(m_search_result.begin(), m_search_result.end(), m_highlight_item);
					if (iter == m_search_result.end())
						hightlight_item = -1;
					else
						hightlight_item = iter - m_search_result.begin();
				}
				//当选中行又是高亮行时设置颜色
				if (GetItemState(nmcd.dwItemSpec, LVIS_SELECTED) == LVIS_SELECTED && nmcd.dwItemSpec == hightlight_item)
				{
					this_item_select = true;
					SetItemState(nmcd.dwItemSpec, 0, LVIS_SELECTED);
					lplvdr->clrText = m_theme_color.light3;
					lplvdr->clrTextBk = m_theme_color.dark1;
				}
				//设置选中行的颜色
				else if (GetItemState(nmcd.dwItemSpec, LVIS_SELECTED) == LVIS_SELECTED)
				{
					this_item_select = true;
					SetItemState(nmcd.dwItemSpec, 0, LVIS_SELECTED);
					lplvdr->clrText = m_theme_color.dark3;
					lplvdr->clrTextBk = m_theme_color.light2;
				}
				//设置高亮行的颜色
				else if (nmcd.dwItemSpec == hightlight_item)
				{
					lplvdr->clrText = m_theme_color.dark2;
					//lplvdr->clrText = 0;
					lplvdr->clrTextBk = m_theme_color.light3;
				}
				//设置偶数行的颜色
				else if (nmcd.dwItemSpec % 2 == 0)
				{
					lplvdr->clrText = CColorConvert::m_gray_color.dark3;
					lplvdr->clrTextBk = CColorConvert::m_gray_color.light3;
				}
				//设置奇数行的颜色
				else
				{
					lplvdr->clrText = CColorConvert::m_gray_color.dark3;
					lplvdr->clrTextBk = CColorConvert::m_gray_color.light4;
				}
			}

			//用背景色填充单元格，以去掉每行前面的空白
			CRect rect = nmcd.rc;
			CDC* pDC = CDC::FromHandle(nmcd.hdc);		//获取绘图DC
			pDC->FillSolidRect(rect, lplvdr->clrTextBk);
		}
		else		//当控件被禁用时，显示文本设为灰色
		{
			lplvdr->clrText = GRAY(140);
			lplvdr->clrTextBk = GRAY(240);
		}
		*pResult = CDRF_NOTIFYPOSTPAINT;
		break;
	case CDDS_ITEMPOSTPAINT:
		if (this_item_select)
			SetItemState(nmcd.dwItemSpec, 0xFF, LVIS_SELECTED);
		//*pResult = CDRF_DODEFAULT;
		break;
	}
}
