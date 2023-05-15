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
	//���ڽ����ļ���
	TCHAR strFilename[MAX_PATH] = { 0 };

	OPENFILENAME ofn = { 0 };

	//�ṹ���С
	ofn.lStructSize = sizeof(OPENFILENAME);

	// ӵ���Ŵ��ھ����ΪNULL��ʾ�Ի����Ƿ�ģ̬�ģ�ʵ��Ӧ����һ�㶼Ҫ��������
	ofn.hwndOwner = NULL;

	//���ù���
	ofn.lpstrFilter = filter.c_str();

	//����������
	ofn.nFilterIndex = 1;

	//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
	ofn.lpstrFile = strFilename;

	//����������
	ofn.nMaxFile = sizeof(strFilename);

	// ��ʼĿ¼
	ofn.lpstrInitialDir = initialDirPath.c_str();

	// ����
	ofn.lpstrTitle = title.c_str();

	// �ļ���Ŀ¼������ڣ�����ֻ��ѡ��
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

	// ��ʾ�Ի���
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