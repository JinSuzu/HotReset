#include "pch.h"
#include "ModuleManager.h"
#include "IModule.h"
#include <fstream>

bool CopyFileUsingStreams(const std::string& sourcePath, const std::string& destinationPath) {
	// 入力ファイルを開く（バイナリモード）
	std::ifstream inputFile(sourcePath, std::ios::binary);
	if (!inputFile.is_open()) {
		std::cerr << "Failed to open source file: " << sourcePath << std::endl;
		return false;
	}

	// 出力ファイルを開く（バイナリモード）
	std::ofstream outputFile(destinationPath, std::ios::binary);
	if (!outputFile.is_open()) {
		std::cerr << "Failed to open destination file: " << destinationPath << std::endl;
		return false;
	}

	// ストリームを使ってデータをコピー
	outputFile << inputFile.rdbuf();

	// ストリームを閉じる
	inputFile.close();
	outputFile.close();

	// コピー成功
	return true;
}
bool ModuleManager::Load(const std::string& dllPath)
{
	IModuleSet* moduleSet = new IModuleSet();

	std::string tempDllPath = dllPath + "_copy.dll";
	if (!CopyFileUsingStreams(dllPath, tempDllPath)) {
		std::cerr << "Failed to load DLL: " << dllPath << std::endl;
		return false;
	}

	moduleSet->hModule = LoadLibraryA(tempDllPath.c_str());
	if (!moduleSet->hModule) {
		std::cerr << "Failed to copy DLL: " << dllPath << std::endl;
		return false;
	}

	CreateFnc createFunc = (CreateFnc)GetProcAddress(moduleSet->hModule, "CreateModule");
	if (!createFunc) {
		std::cerr << "Failed to get factory function: CreateModule" << std::endl;
		return false;
	}

	if ((moduleSet->pModule = createFunc())) {
		moduleSet->pModule->Initialize();
		m_moduleList.push_back(moduleSet);
		return true;
	}

	return false;
}
void ModuleManager::Release()
{
	for (auto modulePack : m_moduleList) {
		DeleteFnc deleteFunc = (DeleteFnc)GetProcAddress(modulePack->hModule, "DeleteModule");
		if (!deleteFunc) {
			std::cerr << "Failed to get factory function: CreateModule" << std::endl;
			return;
		}

		if (modulePack->pModule)
		{
			modulePack->pModule->Shutdown();
			deleteFunc(modulePack->pModule);
			modulePack->pModule = nullptr;
		}

		if (modulePack->hModule)
		{
			FreeLibrary(modulePack->hModule);
			modulePack->hModule = nullptr;
		}

		delete modulePack;
		modulePack = nullptr;
	}
	m_moduleList.clear();
}

void ModuleManager::Update() {
	for (auto& modulePack : m_moduleList) {
		modulePack->pModule->Update();
	}
}