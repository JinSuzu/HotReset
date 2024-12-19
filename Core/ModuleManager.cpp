#include "pch.h"
#include "ModuleManager.h"
#include "IModule.h"
#include <fstream>

bool CopyFileUsingStreams(const std::string& sourcePath, const std::string& destinationPath) {
	// ���̓t�@�C�����J���i�o�C�i�����[�h�j
	std::ifstream inputFile(sourcePath, std::ios::binary);
	if (!inputFile.is_open()) {
		std::cerr << "Failed to open source file: " << sourcePath << std::endl;
		return false;
	}

	// �o�̓t�@�C�����J���i�o�C�i�����[�h�j
	std::ofstream outputFile(destinationPath, std::ios::binary);
	if (!outputFile.is_open()) {
		std::cerr << "Failed to open destination file: " << destinationPath << std::endl;
		return false;
	}

	// �X�g���[�����g���ăf�[�^���R�s�[
	outputFile << inputFile.rdbuf();

	// �X�g���[�������
	inputFile.close();
	outputFile.close();

	// �R�s�[����
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