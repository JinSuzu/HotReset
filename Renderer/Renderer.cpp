#include "pch.h"
#include <iostream>
#include "Renderer.h"


void Renderer::Initialize()
{
	std::cout << "Renderer : ������\n";
}
void Renderer::Update()
{
	std::cout << "Renderer : �X�V\n";
	// std::cout << "Renderer : �X�V(�z�b�g�����[�h�H)\n";
}
void Renderer::Shutdown()
{
	std::cout << "Renderer : �I��\n";
}

RENDERER_API IModule* CreateModule()
{
	return new Renderer();
}

RENDERER_API void DeleteModule(IModule* _module)
{
	delete _module;
}
