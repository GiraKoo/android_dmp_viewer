#pragma once

#include <string>

/**
 * @brief �ļ������
*/
class FileSystemHelper
{
public:

	/**
	 * @brief ��õ���
	 *
	 * @return ���ص���
	*/
	static FileSystemHelper* getInstance();

	/**
	 * @brief ѡ��һ���ļ�
	 *
	 * @param[in] title ���ڱ���
	 * @param[in] filter �ļ�ɸѡ
	 * @param[in] initialDirPath ��ʼ��Ŀ¼
	 * @param[out] selectedFilePath ����ֵΪtrueʱ�����ر�ѡ�е�Ŀ¼
	 * @return �Ƿ�ѡ���ļ�
	*/
	bool select(const std::wstring& title, const std::wstring& filter, const std::wstring& initialDirPath, std::wstring& selectedFilePath);

	/**
	 * @brief ѡ��һ���ļ�
	 *
	 * @param[in] title ���ڱ���
	 * @param[in] filter �ļ�ɸѡ
	 * @param[in] initialDirPath ��ʼ��Ŀ¼
	 * @param[out] selectedFilePath ����ֵΪtrueʱ�����ر�ѡ�е�Ŀ¼
	 * @return �Ƿ�ѡ���ļ�
	*/
	bool select(const std::string& title, const std::string& filter, const std::string& initialDirPath, std::string& selectedFilePath);

	/**
	 * @brief ��õ�ǰ��������Ŀ¼
	 *
	 * @return ��ǰ��������Ŀ¼
	*/
	std::string getApplicationDirectory();
};

