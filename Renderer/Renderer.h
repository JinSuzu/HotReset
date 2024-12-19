#pragma once
#include "../Core/IModule.h"

#ifdef RENDERER_DLL_EXPORTS
#define RENDERER_API __declspec(dllexport)
#else
#define RENDERER_API __declspec(dllimport)
#endif // RENDERER_DLL_EXPORTS

class Renderer :public IModule
{
public:
	Renderer() = default;
	~Renderer() override = default;

	virtual void Initialize()override;
	virtual void Update()override;
	virtual void Shutdown()override;
};

extern "C" RENDERER_API IModule* CreateModule();
extern "C" RENDERER_API void DeleteModule(IModule* _module);