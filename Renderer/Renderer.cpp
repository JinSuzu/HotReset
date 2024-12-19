#include "pch.h"
#include <iostream>
#include "Renderer.h"


void Renderer::Initialize()
{
	std::cout << "Renderer : 初期化\n";
}
void Renderer::Update()
{
	std::cout << "Renderer : 更新\n";
	// std::cout << "Renderer : 更新(ホットリロード？)\n";
}
void Renderer::Shutdown()
{
	std::cout << "Renderer : 終了\n";
}

RENDERER_API IModule* CreateModule()
{
	return new Renderer();
}

RENDERER_API void DeleteModule(IModule* _module)
{
	delete _module;
}
