#pragma once
#include <string>
#include <list>
#include <iostream>
#include <Windows.h>
#include <libloaderapi.h>

class IModule;
typedef IModule* (*CreateFnc)();
typedef void (*DeleteFnc)(IModule*);

class ModuleManager
{
public:
	struct IModuleSet
	{
		HMODULE hModule = nullptr;
		IModule* pModule = nullptr;
	};

	bool Load(const std::string& dllPath);
	void Release();
	
	void Update();
private:
	std::list<IModuleSet*> m_moduleList;
};

