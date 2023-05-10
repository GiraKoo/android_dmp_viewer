#pragma once

#include <string>
#include <vector>
#include <cstdlib>
#include "common.h"

using namespace std;

class MainView
{
public:

	// 描画
	static void render();

	static void init();

	// 读取配置
	static void readConfig();

	// 写入配置
	static void writeConfig();

private:
	// 待解析文件路径
	static char m_dumpFilePath[MAX_FILE_PATH_LENGTH];

	static char m_symbolFilePath[MAX_FILE_PATH_LENGTH];

	static vector<string> m_symbolFileLocalHistory;

	static vector<string> m_symbolFilePublicHistory;

	// 工具路径
	static char m_stackWalkToolPath[MAX_FILE_PATH_LENGTH];
	static char m_symbolizerToolPath[MAX_FILE_PATH_LENGTH];

	// 用于下载符号文件的符号服务器地址
	static char m_publicSymbolServer[MAX_URL_LENGTH];

	// Android设备上Crash文件路径
	static char m_androidCrashDirectoryPath[MAX_FILE_PATH_LENGTH];
};