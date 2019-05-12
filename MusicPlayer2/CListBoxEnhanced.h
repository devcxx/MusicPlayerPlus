#pragma once
#include "ListCtrlEx.h"

#define WM_LISTBOX_SEL_CHANGED (WM_USER+116)

class CListBoxEnhanced :
	public CListCtrlEx
{
public:
	CListBoxEnhanced();
	~CListBoxEnhanced();

	void AddString(LPCTSTR str);

	virtual void PreSubclassWindow();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
};

