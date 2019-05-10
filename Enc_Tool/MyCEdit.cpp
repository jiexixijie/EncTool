// MyCEdit.cpp: 实现文件
//

#include "pch.h"
#include "Enc_Tool.h"
#include "MyCEdit.h"
#include "EnumList.h"

// MyCEdit

IMPLEMENT_DYNAMIC(MyCEdit, CEdit)

MyCEdit::MyCEdit():FilePath(_T(""))
{
}

MyCEdit::~MyCEdit()
{
}


BEGIN_MESSAGE_MAP(MyCEdit, CEdit)
	ON_WM_DROPFILES()
	ON_CONTROL_REFLECT(EN_CHANGE, &MyCEdit::OnEnChange)
END_MESSAGE_MAP()



// MyCEdit 消息处理程序




void MyCEdit::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UpdateData(TRUE);
	UINT count;
	TCHAR filePath_w[MAX_PATH] = { 0 };
	count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);//从成功的拖放操作中检索文件的名称。并取代被拖拽文件的数目
	if (count == 1)//如果只拖拽一个文件夹
	{
		DragQueryFile(hDropInfo, 0, filePath_w, MAX_PATH);//获得拖拽的文件名
		DragFinish(hDropInfo);//拖放成功后，释放内存
		showFileHexData(filePath_w);
	}
	UpdateData(FALSE);
	CEdit::OnDropFiles(hDropInfo);
}

CString MyCEdit::data2Hex(BYTE* data, int len) {
	CString data_Hex(_T(""));
	CString temp;
	for (int i = 0; i < len; i++) {
		temp.Format(_T("%02X "), data[i]);
		if (i != 0 && (i+1) % 30 == 0) {
			temp.Format(_T("%s \r\n"), temp);
		}
		data_Hex += temp;
	}
	return data_Hex;
}

void MyCEdit::OnEnChange()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CEdit::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	//如果是导入文件后再次对内容进行修改则对filepath经行清空
	if (!FilePath.IsEmpty()) {
		FilePath.Empty();
		this->SetWindowTextW(_T(""));
	}
}

void MyCEdit::SetFilePath(CString path){
	FilePath = path;
}

void MyCEdit::showFileHexData(CString filePath) {
	//从文件中读取前1024个字节 如果超出则省略
	BYTE pBuf[10240];
	//记录加载的文件名
	this->FilePath = filePath;
	CFile file;
	if (file.Open(FilePath, CFile::modeRead) == 0) {
		MessageBox(_T("读取失败"));
		this->SetWindowTextW(_T(""));
		return;
	}
	ULONGLONG filesize = file.GetLength();
	if (filesize <= 10240) {
		file.Read(pBuf, (int)filesize);
		this->SetWindowTextW(data2Hex(pBuf, (int)filesize));
	}
	else {
		file.Read(pBuf, 10240);
		CString temp = data2Hex(pBuf, 10240);
		temp.Format(_T("%s \r\n...文件过长进行省略..."), temp);
		this->SetWindowTextW(temp);
	}
	file.Close();
}


void  MyCEdit::showFileData(CString filePath) {
	//从文件中读取前1024个字节 如果超出则省略
	char pBuf[10241];
	//记录加载的文件名
	this->FilePath = filePath;
	CFile file;
	if (file.Open(FilePath, CFile::modeRead) == 0) {
		MessageBox(_T("读取失败"));
		this->SetWindowTextW(_T(""));
		return;
	}
	ULONGLONG filesize = file.GetLength();
	if (filesize <= 10240) {
		file.Read(pBuf, (int)filesize);
		pBuf[filesize] = '\0';
		CString temp = TransData((CString)pBuf, 0);
		this->SetWindowTextW(temp);
	}
	else {
		file.Read(pBuf, 10240);
		pBuf[10240] = '\0';
		CString temp(pBuf);
		temp.Format(_T("%s \r\n...文件过长进行省略..."), temp);
		this->SetWindowTextW(temp);
	}
	file.Close();
}


void MyCEdit::SaveFileData(CString filePath) {
	//直接从windwosText中读取
	CString temp;
	this->GetWindowTextW(temp);
	temp=TransData(temp, 1);
	int flen = WideCharToMultiByte(CP_ACP, 0, temp, temp.GetLength(), NULL, 0, NULL, NULL);
	char* filedata = new char[flen  + 1];
	WideCharToMultiByte(CP_ACP, 0, temp, temp.GetLength(), filedata, flen, NULL, NULL);
	CFile file;
	if (!file.Open(filePath, CFile::modeCreate | CFile::modeReadWrite)) {
		MessageBox(_T("写入失败"));
		return;
	}
	else {
		file.Write((BYTE*)filedata, flen);
	}
	file.Close();
	delete[]filedata;
	MessageBox(_T("写入成功"));
}

bool MyCEdit::IsFile() {
	return !FilePath.IsEmpty();
}


void  MyCEdit::showHexData(char* Data, ULONGLONG datalen) {
	if (datalen <= 10240) {
		this->SetWindowTextW(data2Hex((BYTE*)Data, (int)datalen));
	}
	else {
		CString temp = data2Hex((BYTE*)Data, 10240);
		temp.Format(_T("%s \r\n...文件过长进行省略..."), temp);
		this->SetWindowTextW(temp);
	}
}