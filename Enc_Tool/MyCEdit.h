#pragma once

#include "Enc_Tool.h"
// MyCEdit

class MyCEdit : public CEdit
{
	DECLARE_DYNAMIC(MyCEdit)

public:
	MyCEdit();
	virtual ~MyCEdit();
	//16������ʾ����
	CString data2Hex(BYTE* data, int len);
	void SetFilePath(CString path);
	//���ļ���ǰ10240btye 16������ʾ
	void showFileHexData(CString filePath);
	void showHexData(char *Data,ULONGLONG datalen);
	//������ȡ���ݵ��ļ���
	void showFileData(CString filePath);
	//��������  ֱ�ӱ��浽����
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


