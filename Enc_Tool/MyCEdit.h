#pragma once

#include "Enc_Tool.h"
// MyCEdit

class MyCEdit : public CEdit
{
	DECLARE_DYNAMIC(MyCEdit)

public:
	MyCEdit();
	virtual ~MyCEdit();
	//16进制显示数据
	CString data2Hex(BYTE* data, int len);
	void SetFilePath(CString path);
	//将文件中前10240btye 16进制显示
	void showFileHexData(CString filePath);
	void showHexData(char *Data,ULONGLONG datalen);
	//正常读取内容到文件中
	void showFileData(CString filePath);
	//保存内容  直接保存到内容
	void SaveFileData(CString filePath);
	bool IsFile();
	void EmptyFile();
	CString FilePath;
protected:
	DECLARE_MESSAGE_MAP()
private:
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnEnChange();
};


