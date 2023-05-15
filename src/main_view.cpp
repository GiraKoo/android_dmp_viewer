#include "main_view.h"
#include "imgui.h"

#include <windows.h>
#include "xini_file.h"
#include "ImGuiFileDialog.h"
#include "file_system_helper.h"
#include "stack_reader.h"

// Our state
static bool show_demo_window = false;
static bool show_another_window = false;

char MainView::m_dmpFilePath[MAX_FILE_PATH_LENGTH] = { 0 };
char MainView::m_symbolFilePath[MAX_FILE_PATH_LENGTH] = { 0 };
char MainView::m_stackWalkToolPath[MAX_FILE_PATH_LENGTH] = { 0 };
char MainView::m_symbolizerToolPath[MAX_FILE_PATH_LENGTH] = { 0 };
char MainView::m_publicSymbolServer[MAX_URL_LENGTH] = { 0 };
char MainView::m_androidCrashDirectoryPath[MAX_FILE_PATH_LENGTH] = { 0 };

void MainView::init()
{
	readConfig();
}

void MainView::render()
{
	ImGuiIO& io = ImGui::GetIO();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Main View");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

		//ImGui::Checkbox("Another Window", &show_another_window);

		{
			ImGui::PushItemWidth(600);
			ImGui::InputText("Dmp file", m_dmpFilePath, ELEMENT_OF(m_dmpFilePath));
			ImGui::PopItemWidth();
			ImGui::SameLine();

			if (ImGui::Button("Browse##DmpFile"))
			{
				//ImGuiFileDialog::Instance()->OpenDialog("ChooseDmpFile", "Choose Dmp File", ".dmp", m_dumpFilePath);
				std::string selectedPath = "";
				std::string initialDir = m_dmpFilePath;
				initialDir.erase(initialDir.find_last_of(L'\\'));
				if (FileSystemHelper::getInstance()->select("Choose Dmp File", "*.dmp\0\0", initialDir, selectedPath))
				{
					strcpy_s(m_dmpFilePath, ELEMENT_OF(m_dmpFilePath), selectedPath.c_str());
					writeConfig();
				}
			}
		}

		{
			ImGui::PushItemWidth(600);
			ImGui::InputText("Symbol file", m_symbolFilePath, ELEMENT_OF(m_symbolFilePath));
			ImGui::PopItemWidth();
			ImGui::SameLine();

			if (ImGui::Button("Browse##SymbolFile"))
			{
				std::string selectedPath = "";
				std::string initialDir = m_symbolFilePath;
				initialDir.erase(initialDir.find_last_of(L'\\'));
				if (FileSystemHelper::getInstance()->select("Choose Dmp File", "*.so\0\0", initialDir, selectedPath))
				{
					strcpy_s(m_symbolFilePath, ELEMENT_OF(m_symbolFilePath), selectedPath.c_str());
					writeConfig();
				}
			}

			ImGui::SameLine();
			if (ImGui::Button("Local History"))
			{
				// TODO: Show Local History
			}
			ImGui::SameLine();
			if (ImGui::Button("Public"))
			{
				// TODO: Show public symbol
			}
		}

		{
			ImGui::PushItemWidth(600);
			ImGui::InputText("StackWalk tool", m_stackWalkToolPath, ELEMENT_OF(m_stackWalkToolPath));
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if (ImGui::Button("Browse"))
			{
			}
		}

		{
			ImGui::PushItemWidth(600);
			ImGui::InputText("Symbolizer tool", m_symbolizerToolPath, ELEMENT_OF(m_symbolizerToolPath));
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if (ImGui::Button("Browse"))
			{
			}
		}
		{
			ImGui::PushItemWidth(600);
			ImGui::InputText("Public symbol server", m_publicSymbolServer, ELEMENT_OF(m_publicSymbolServer));
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if (ImGui::Button("Browse"))
			{
			}
		}
		{
			ImGui::PushItemWidth(600);
			ImGui::InputText("Android crash directory", m_androidCrashDirectoryPath, ELEMENT_OF(m_androidCrashDirectoryPath));
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if (ImGui::Button("Browse"))
			{
			}
		}

		ImGui::NewLine();

		if (ImGui::Button("Reset"))
		{
			strcpy_s(m_dmpFilePath, ELEMENT_OF(m_dmpFilePath), "");
			strcpy_s(m_symbolFilePath, ELEMENT_OF(m_symbolFilePath), "");
			strcpy_s(m_stackWalkToolPath, ELEMENT_OF(m_stackWalkToolPath), "minidump_stackwalk.exe");
			strcpy_s(m_symbolizerToolPath, ELEMENT_OF(m_symbolizerToolPath), "llvm-symbolizer.exe");
			strcpy_s(m_publicSymbolServer, ELEMENT_OF(m_publicSymbolServer), "");
			strcpy_s(m_androidCrashDirectoryPath, ELEMENT_OF(m_androidCrashDirectoryPath), "");

			writeConfig();
		}

		ImGui::SameLine();

		if (ImGui::Button("Go"))
		{
			StackReader* reader = StackReader::getInstance();
			reader->setStackWalkToolPath(m_stackWalkToolPath);
			reader->setSymbolizerToolPath(m_symbolizerToolPath);

			reader->clearLibPath();
			reader->addLibPath(m_symbolFilePath);

			reader->loadDumpFilePath(m_dmpFilePath);
		}

		ImGui::End();
	}

	// display
	if (ImGuiFileDialog::Instance()->Display("ChooseDmpFile"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();

			// action
			strcpy_s(m_dmpFilePath, ELEMENT_OF(m_dmpFilePath), filePathName.c_str());
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}
	if (ImGuiFileDialog::Instance()->Display("ChooseDmpFile"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			strcpy_s(m_dmpFilePath, ELEMENT_OF(m_dmpFilePath), filePathName.c_str());
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}
	if (ImGuiFileDialog::Instance()->Display("ChooseSymbolFile"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();

			// action
			strcpy_s(m_symbolFilePath, ELEMENT_OF(m_symbolFilePath), filePathName.c_str());
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}

void MainView::readConfig()
{
	xini_file_t iniFile(FileSystemHelper::getInstance()->getApplicationDirectory() + "\\app.ini");

	strcpy_s(m_dmpFilePath, ELEMENT_OF(m_dmpFilePath), iniFile["DEFAULT"]["dump_file_path"]);
	strcpy_s(m_symbolFilePath, ELEMENT_OF(m_symbolFilePath), iniFile["DEFAULT"]["symbol_file_path"]);
	strcpy_s(m_stackWalkToolPath, ELEMENT_OF(m_stackWalkToolPath), iniFile["DEFAULT"]["stack_walk_tool_path"]("minidump_stackwalk.exe"));
	strcpy_s(m_symbolizerToolPath, ELEMENT_OF(m_symbolizerToolPath), iniFile["DEFAULT"]["symbolizer_tool_path"]("llvm-symbolizer.exe"));
	strcpy_s(m_publicSymbolServer, ELEMENT_OF(m_publicSymbolServer), iniFile["DEFAULT"]["public_symbol_server"]);
	strcpy_s(m_androidCrashDirectoryPath, ELEMENT_OF(m_androidCrashDirectoryPath), iniFile["DEFAULT"]["android_crash_directory_path"]);
}

void MainView::writeConfig()
{
	// 获得当前程序所在目录
	xini_file_t iniFile(FileSystemHelper::getInstance()->getApplicationDirectory() + "\\app.ini");
	iniFile["DEFAULT"]["dump_file_path"] = m_dmpFilePath;
	iniFile["DEFAULT"]["symbol_file_path"] = m_symbolFilePath;
	iniFile["DEFAULT"]["stack_walk_tool_path"] = m_stackWalkToolPath;
	iniFile["DEFAULT"]["symbolizer_tool_path"] = m_symbolizerToolPath;
	iniFile["DEFAULT"]["public_symbol_server"] = m_publicSymbolServer;
	iniFile["DEFAULT"]["android_crash_directory_path"] = m_androidCrashDirectoryPath;
	iniFile.dump("app.ini");
}