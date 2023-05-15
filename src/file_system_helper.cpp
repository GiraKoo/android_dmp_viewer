#include "file_system_helper.h"

#include <windows.h>
#include <shlobj.h>
#include <iostream>
#include <string>
#include <locale>

#include "common.h"

FileSystemHelper* FileSystemHelper::getInstance()
{
	static FileSystemHelper instance;
	return &instance;
}

static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED)
	{
		std::string tmp = (const char*)lpData;
		std::cout << "path: " << tmp << std::endl;
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	}

	return 0;
}

bool FileSystemHelper::select(const std::wstring& title, const std::wstring& filter, const std::wstring& initialDirPath, std::wstring& selectedFilePath)
{
	//用于接收文件名
	TCHAR strFilename[MAX_PATH] = { 0 };

	OPENFILENAME ofn = { 0 };

	//结构体大小
	ofn.lStructSize = sizeof(OPENFILENAME);

	// 拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄
	ofn.hwndOwner = NULL;

	//设置过滤
	ofn.lpstrFilter = filter.c_str();

	//过滤器索引
	ofn.nFilterIndex = 1;

	//接收返回的文件名，注意第一个字符需要为NULL
	ofn.lpstrFile = strFilename;

	//缓冲区长度
	ofn.nMaxFile = sizeof(strFilename);

	// 初始目录
	ofn.lpstrInitialDir = initialDirPath.c_str();

	// 标题
	ofn.lpstrTitle = title.c_str();

	// 文件、目录必须存在，隐藏只读选项
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

	// 显示对话框
	if (GetOpenFileName(&ofn))
	{
		selectedFilePath = strFilename;
		return true;
	}

	return false;
}

bool FileSystemHelper::select(const std::string& title, const std::string& filter, const std::string& initialDirPath, std::string& selectedFilePath)
{
	std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>>
		converter(new std::codecvt<wchar_t, char, std::mbstate_t>("CHS"));

	std::wstring wTitle = converter.from_bytes(title);
	std::wstring wFilter = converter.from_bytes(filter);
	std::wstring wInitialDirPath = converter.from_bytes(initialDirPath);

	std::wstring wSelectedFolder;
	bool ret = select(wTitle, wFilter, wInitialDirPath, wSelectedFolder);

	selectedFilePath = converter.to_bytes(wSelectedFolder);

	return ret;
}

std::string FileSystemHelper::getApplicationDirectory() {
	std::string res;
	char szModule[1024] = { 0 };
	GetModuleFileNameA(NULL, szModule, ELEMENT_OF(szModule));
	res = szModule;
	res.erase(res.find_last_of(L'\\'));
	return res;
}