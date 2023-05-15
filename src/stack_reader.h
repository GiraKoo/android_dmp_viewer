#pragma once

#include <string>
#include <unordered_map>

class StackReader
{
public:

	/**
	 * @brief ����ģʽ
	 *
	 * @return ����
	*/
	static StackReader* getInstance();

	/**
	 * @brief �趨StackWalk·��
	 *
	 * @param path ·��
	*/
	void setStackWalkToolPath(const std::string path);

	/**
	 * @brief �趨Symbolizer·��
	 *
	 * @param path ·��
	*/
	void setSymbolizerToolPath(const std::string path);

	/**
	 * @brief �趨lib����Ϣ
	 *
	 * @param libPath lib����·��
	*/
	void addLibPath(const std::string& libPath);

	/**
	 * @brief �Ƴ�lib����Ϣ
	 *
	 * @param libPath lib����·��
	*/
	void removeLibPath(const std::string& libPath);

	/**
	 * @brief ɾ��ȫ��lib����Ϣ
	*/
	void clearLibPath();

	/**
	 * @brief ����dump�ļ�
	 *
	 * @param path ·��
	*/
	void loadDumpFilePath(const std::string& path);

private:

	/**
	 * @brief ����Nisdk
	 *
	 * @param line ����������
	 *
	 * @return ���غ�����Ϣ
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

