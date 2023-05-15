#pragma once

#include <string>
#include <unordered_map>

class StackReader
{
public:

	/**
	 * @brief 单例模式
	 *
	 * @return 单例
	*/
	static StackReader* getInstance();

	/**
	 * @brief 设定StackWalk路径
	 *
	 * @param path 路径
	*/
	void setStackWalkToolPath(const std::string path);

	/**
	 * @brief 设定Symbolizer路径
	 *
	 * @param path 路径
	*/
	void setSymbolizerToolPath(const std::string path);

	/**
	 * @brief 设定lib库信息
	 *
	 * @param libPath lib所在路径
	*/
	void addLibPath(const std::string& libPath);

	/**
	 * @brief 移除lib库信息
	 *
	 * @param libPath lib所在路径
	*/
	void removeLibPath(const std::string& libPath);

	/**
	 * @brief 删除全部lib库信息
	*/
	void clearLibPath();

	/**
	 * @brief 加载dump文件
	 *
	 * @param path 路径
	*/
	void loadDumpFilePath(const std::string& path);

private:

	/**
	 * @brief 加载Nisdk
	 *
	 * @param line 输入行内容
	 *
	 * @return 返回函数信息
	*/
	//std::string loadAddrs(const std::string& line);

	void addr2line();

	//std::string loadFuncs(const std::string& line, int cur);

	//std::string addr2line(const std::string& lib, const std::string& addr);

	bool isSupportedLine(const std::string& line, std::string& libName, std::string& addressName);

	std::unordered_map<std::string, std::string> m_libPaths;

	class DetailInfo
	{
	public:
		DetailInfo(const std::string& address) { m_addressName = address; }

		bool operator==(const DetailInfo& info) const
		{
			return m_addressName == info.m_addressName;
		}

		bool operator==(const std::string& address) const
		{
			return m_addressName == address;
		}

		std::string m_addressName;
		std::string m_funcName;
	};

	//std::unordered_map<std::string, std::string> m_libAddrs;
	std::unordered_map<std::string, std::vector<DetailInfo>> m_libFunctions;

	std::string m_stackWalkToolPath;
	std::string m_symbolizerToolPath;
};

