#include "Debug.h"
#include "Engine.h"
#include "WorldGrid.h"
#include "Application.h"
#include "VirtualFileSystem.h"
#include <SDL_rect.h>
#include <SDL_render.h>
#include <functional>
#include <iostream>
#include <utility>
#include <vector>

int main()
{
    /*
    VirtualFileSystem vfs("test.dat");

    vfs.CreatePhysicalFile(true);
    vfs.WriteData({});


    std::vector<char> data;
    for (int i = 0; i < 255; i++)
    {
        data.push_back(0xff);
    }

    VirtualFileSystem::Entry e;
    e.offset = 10000;
    vfs.WriteEntry(e, data, 3);
    vfs.WriteEntry(e, data, 5);
    vfs.WriteEntry(e, data, 2);
    */
    Engine::Inst()->Initialize();
    // Engine::Inst()->Run();

	Application app = {};

    Engine::Inst()->AddEvent([&](SDL_Event &e) {
        if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.sym == SDLK_a)
            {
            }
        }
    }, EventType::SDL);

	static std::function<void()> appStart = std::bind(&Application::OnStart, &app);
	Engine::Inst()->AddEvent(appStart, EventType::START);
	
	static std::function<void()> appQuit = std::bind(&Application::OnQuit, &app);
	Engine::Inst()->AddEvent(appQuit, EventType::QUIT);

	static std::function<void(double)> appUpdate = std::bind(&Application::OnUpdate, &app, std::placeholders::_1);
	Engine::Inst()->AddEvent(appUpdate, EventType::UPDATE);

	static std::function<void(double)> appRender = std::bind(&Application::OnRender, &app, std::placeholders::_1);
	Engine::Inst()->AddEvent(appRender, EventType::RENDER);

	app.OnAwake();
	
	Engine::Inst()->Run();

    Engine::Inst()->Finalize();

    return 0;
}
