#pragma once

#include <string>

/**
 * @brief 文件浏览器
*/
class FileSystemHelper
{
public:

	/**
	 * @brief 获得单例
	 *
	 * @return 返回单例
	*/
	static FileSystemHelper* getInstance();

	/**
	 * @brief 选择一个文件
	 *
	 * @param[in] title 窗口标题
	 * @param[in] filter 文件筛选
	 * @param[in] initialDirPath 初始化目录
	 * @param[out] selectedFilePath 返回值为true时，返回被选中的目录
	 * @return 是否选中文件
	*/
	bool select(const std::wstring& title, const std::wstring& filter, const std::wstring& initialDirPath, std::wstring& selectedFilePath);

	/**
	 * @brief 选择一个文件
	 *
	 * @param[in] title 窗口标题
	 * @param[in] filter 文件筛选
	 * @param[in] initialDirPath 初始化目录
	 * @param[out] selectedFilePath 返回值为true时，返回被选中的目录
	 * @return 是否选中文件
	*/
	bool select(const std::string& title, const std::string& filter, const std::string& initialDirPath, std::string& selectedFilePath);

	/**
	 * @brief 获得当前程序所在目录
	 *
	 * @return 当前程序所在目录
	*/
	std::string getApplicationDirectory();
};

