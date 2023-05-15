#include "stack_reader.h"

#include <cstdlib>
#include <cstdio>
#include <regex>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include "common.h"

StackReader* StackReader::getInstance()
{
	static StackReader instance;
	return &instance;
}

void StackReader::setStackWalkToolPath(const std::string path)
{
	m_stackWalkToolPath = path;
}

void StackReader::setSymbolizerToolPath(const std::string path)
{
	m_symbolizerToolPath = path;
}

void StackReader::addLibPath(const std::string& libPath)
{
	std::string libName = libPath.substr(libPath.find_last_of("\\") + 1);

	// 查找m_libMap的key与libName相同的项目
	auto iter = m_libPaths.find(libPath);
	if (iter != m_libPaths.end())
	{
		m_libPaths[libName] = libPath;
	}
	else
	{
		m_libPaths.emplace(libName, libPath);
	}
}

void StackReader::removeLibPath(const std::string& libPath)
{
	std::string libName = libPath.substr(libPath.find_last_of("\\") + 1);
	auto iter = m_libPaths.find(libPath);
	if (iter != m_libPaths.end())
	{
		m_libPaths.erase(iter);
	}
}

void StackReader::clearLibPath()
{
	m_libPaths.clear();
}

void StackReader::loadDumpFilePath(const std::string& path)
{
	char buf[1024] = { 0 };

	std::string cmd = m_stackWalkToolPath;
	cmd += " ";
	cmd += path;
	cmd += " ";
	cmd += m_symbolizerToolPath;

	FILE* fp = NULL;

#if 1
	std::ofstream outTrace(path + ".trace", std::ios::out);
	if (!outTrace.is_open())
	{
		throw std::invalid_argument("[GiraKoo] Out file open failed!");
	}
#else
	std::string outTrace;
#endif

	// 清空m_addr2lineMap
	// m_libAddrs.empty();
	m_libFunctions.empty();

	if ((fp = _popen(cmd.c_str(), "r")) == NULL) {
		throw std::invalid_argument("[GiraKoo] Stack walk tool run failed!");
	}

	// 读取address
	while (fgets(buf, sizeof(buf), fp)) {
		std::string libName, addressName;
		if (isSupportedLine(buf, libName, addressName))
		{
			if (m_libPaths.find(libName) == m_libPaths.end())
			{
				continue;
			}

			if (m_libFunctions.find(libName) == m_libFunctions.end())
			{
				m_libFunctions.emplace(libName, std::vector<DetailInfo>());
			}
			m_libFunctions[libName].push_back(addressName);
		}
		outTrace << buf;
	}

	outTrace.close();

	_pclose(fp);

	// 将m_libAddrs转换数据保存到m_libFunctions
	addr2line();

	std::ifstream inTrace(path + ".trace", std::ios::in);
	std::ofstream outResult(path + ".txt", std::ios::out);

	if (!inTrace.is_open() || !outResult.is_open())
	{
		throw std::invalid_argument("[GiraKoo] Trace file open failed!");
	}

	while (inTrace.getline(buf, sizeof(buf)))
	{
		std::string libName, addressName, function;
		if (isSupportedLine(buf, libName, addressName))
		{
			if (m_libFunctions.find(libName) == m_libFunctions.end())
			{
				continue;
			}

			auto iter = std::find(m_libFunctions[libName].begin(), m_libFunctions[libName].end(), addressName);
			if (iter != m_libFunctions[libName].end())
			{
				function = iter->m_funcName;
			}
			outResult << buf << std::endl << function << std::endl;
		}
		else
		{
			outResult << buf << std::endl;
		}
	}

	inTrace.close();
	outResult.close();
}

#if 0
std::string StackReader::loadAddrs(const std::string& line, int cur)
{
	FILE* fp = NULL;
	char buf[1024] = { 0 };
	std::string result = "";

	int find_res = line.find_first_of('+');
	if (find_res != std::string::npos)
	{
		std::vector<int> spacePos;
		for (int i = 0; i < line.size(); ++i)
		{
			if (line[i] == ' ')
			{
				spacePos.push_back(i);
			}
		}

		if (spacePos.size() < 5)
		{
			return "[GiraKoo] Format not supported!";
		}

		std::string libName = line.substr(spacePos[2] + 1, spacePos[3] - spacePos[2] - 1);

		std::string funcAddr = line.substr(spacePos[4] + 1);
		funcAddr.erase(funcAddr.find_last_not_of(" \t\r\n") + 1);

		if (m_libPaths.find(libName) == m_libPaths.end())
		{
			return "[GiraKoo] Lib's symbol file has not been set!";
		}

		m_libAddrs[libName] += funcAddr + " ";
	}

	return result;
}
#endif

bool StackReader::isSupportedLine(const std::string& line, std::string& libName, std::string& addressName)
{
	int find_res = line.find_first_of('+');
	if (find_res != std::string::npos)
	{
		std::vector<int> spacePos;
		for (int i = 0; i < line.size(); ++i)
		{
			if (line[i] == ' ')
			{
				spacePos.push_back(i);
			}
		}

		if (spacePos.size() < 5)
		{
			return "[GiraKoo] Format not supported!";
		}

		libName = line.substr(spacePos[2] + 1, spacePos[3] - spacePos[2] - 1);

		addressName = line.substr(spacePos[4] + 1);
		addressName.erase(addressName.find_last_not_of(" \t\r\n") + 1);

		return true;
	}
	return false;
}

void StackReader::addr2line()
{
	for (auto iter = m_libFunctions.begin(); iter != m_libFunctions.end(); ++iter)
	{
		char buf[1024] = { 0 };

		std::string cmd = m_symbolizerToolPath;
		cmd += " --obj=";
		cmd += m_libPaths[iter->first];
		cmd += " ";

		std::string addrs;
		for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2)
		{
			addrs += iter2->m_addressName + " ";
		}

		cmd += addrs;

		FILE* fp = NULL;

		if ((fp = _popen(cmd.c_str(), "r")) == NULL) {
			throw std::invalid_argument("[GiraKoo] Symbolizer tool run failed!");
		}

		for (auto iter2 = m_libFunctions[iter->first].begin(); iter2 != m_libFunctions[iter->first].end(); ++iter2)
		{
			iter2->m_funcName = "======================================================\n";
			fgets(buf, sizeof(buf), fp);
			iter2->m_funcName += buf;
			fgets(buf, sizeof(buf), fp);
			iter2->m_funcName += buf;
			fgets(buf, sizeof(buf), fp);
			iter2->m_funcName += buf;
			iter2->m_funcName += "======================================================\n";
		}

		_pclose(fp);
	}
}

#if 0
std::string StackReader::loadFuncs(const std::string& line)
{
	FILE* fp = NULL;
	char buf[1024] = { 0 };
	std::string result = "";

	int find_res = line.find_first_of('+');
	if (find_res != std::string::npos)
	{
		std::vector<int> spacePos;
		for (int i = 0; i < line.size(); ++i)
		{
			if (line[i] == ' ')
			{
				spacePos.push_back(i);
			}
		}

		if (spacePos.size() < 5)
		{
			return "[GiraKoo] Format not supported!";
		}

		std::string libName = line.substr(spacePos[2] + 1, spacePos[3] - spacePos[2] - 1);
		std::string funcAddr = line.substr(spacePos[4] + 1);
		funcAddr.erase(funcAddr.find_last_not_of(" \t\r\n") + 1);

		// 查询func
	}

	return result;
}

std::string StackReader::addr2line(const std::string& lib, const std::string& addr)
{
	char buf[1024] = { 0 };
	std::string result = "";

	auto iter = m_libPaths.find(lib);
	if (iter == m_libPaths.end())
	{
		return "[GiraKoo] Lib's symbol file has not been set!";
	}

	std::string cmd = m_symbolizerToolPath;
	cmd += " --obj=";
	cmd += m_libPaths[lib];
	cmd += " ";
	cmd += addr;

	FILE* fp = NULL;

	if ((fp = _popen(cmd.c_str(), "r")) == NULL) {
		throw std::invalid_argument("[GiraKoo] Symbolizer tool run failed!");
	}

	result = "(";
	while (fgets(buf, sizeof(buf), fp)) {
		result += buf;
		result.erase(result.find_last_not_of(" \r\n\t") + 1);
	}
	result += ")\n";
	_pclose(fp);

	return result;
}
#endif