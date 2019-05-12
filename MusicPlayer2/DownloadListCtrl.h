#pragma once
#include "ListCtrlEx.h"
#include "AudioCommon.h"
#include "ColorConvert.h"
#include "ListCtrlEx.h"
#include "Common.h"
#include <boost/shared_ptr.hpp>

struct DownloadItem {
	void* ctrl;
	SongInfo* song;
	int id;
};

typedef boost::shared_ptr<DownloadItem> DownItemPtr;

class CDownloadListCtrl :
	public CListCtrlEx
{
	DECLARE_DYNAMIC(CDownloadListCtrl)

public:
	CDownloadListCtrl();
	~CDownloadListCtrl();

	void AdjustColumnWidth();

	void EnableTip(bool enable = true) { m_bEnableTips = enable; }		//设置是否开启提示

	void Download(const SongInfo& si);

	void UpdateDownList();

protected:

	CToolTipCtrl m_toolTip;		//文本提示类
	int m_nItem;				//存放行号
								//int m_nSubItem;			//存放列号
	bool m_bEnableTips{ false };	//是否开启文本提示

	vector<SongInfo> m_all_song_info;		//储存播放列表中所有歌曲的信息
	vector<int> m_search_result;					//储存快速搜索结果的歌曲序号
	bool m_searched{ false };
	vector <DownItemPtr> m_downItems;


	void CalculateColumeWidth(vector<int>& width);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);

};

