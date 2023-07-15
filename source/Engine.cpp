#include "Engine.h"
#include "Debug.h"
#include <chrono>
#include <thread>

Engine *Engine::sEngine = nullptr;

Engine::Engine()
{
}

void Engine::Initialize()
{
    Debug::Initialize();
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        Debug::CoreError("Could not initialize SDL: {}", SDL_GetError());
    }
    else
    {
        int num = 0;
        num++;
        // Create window
        mWindow = SDL_CreateWindow(globalSettings.windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   globalSettings.windowWidth, globalSettings.windowHeight, SDL_WINDOW_SHOWN);
        if (mWindow == NULL)
        {
            Debug::CoreError("Window could not be created! SDL_Error: {}", SDL_GetError());
            return;
        }
        else
        {
            mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
            SDL_SetRenderDrawColor(mRenderer, 0x22, 0x22, 0x22, 0xFF);
        }

		//SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN);

        mTextRenderer = new TextRenderer(mRenderer, "res/fonts/OpenSans/OpenSans-Regular.ttf", 20);

    }

    Debug::CoreInfo("Engine successfully initialized!");
}

void Engine::Run()
{

    for (auto &startEvent : mStartEvents)
    {
        startEvent();
    }

    // Handle events on queue
    while (mRunning)
    {
        static float fpsTimer = 0.0f;

        if (fpsTimer >= 3.0f)
        {
            //Debug::CoreInfo("FPS: {0:.1f} DeltaTime: {1:.4f}", 1 / mDeltaTime, mDeltaTime);
            fpsTimer = 0.0f;
        }
        else
        {
            fpsTimer += mDeltaTime;
        }
		Engine::Inst()->mTextRenderer->RenderText(
				"FPS: "+std::to_string(1 / mDeltaTime) +" DeltaTime: " + std::to_string(mDeltaTime),
				{ 20, 00, 100, 100 });

        static auto lastTime = std::chrono::high_resolution_clock::now();

        auto newtime = std::chrono::high_resolution_clock::now();
        lastTime = std::chrono::high_resolution_clock::now();

        SDL_SetWindowTitle(mWindow, std::to_string(mDeltaTime).c_str());

        PollSdlEvents();

        for (auto &updateEvent : mUpdateEvents)
        {
            updateEvent(mDeltaTime);
        }

        // clear the screen
        SDL_RenderClear(mRenderer);

        for (auto &renderEvent : mRenderEvents)
        {
            renderEvent(mDeltaTime);
        }

		// GUI Pass
		mTextRenderer->RenderTextQueue();

        // Present to screen
        SDL_RenderPresent(mRenderer);

        // SDL_Delay(6.9444f - mDeltaTime);
        using namespace std::chrono_literals;
        std::chrono::milliseconds dura(5ms);
        std::chrono::microseconds dura1(std::chrono::microseconds(900));
        // std::this_thread::sleep_for(dura);
        // std::this_thread::sleep_for(std::chrono::milliseconds(4ms));

        newtime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> delta = newtime - lastTime;

        //std::this_thread::sleep_for(dura + dura1 - delta);
		SDL_Delay(4.9444f - mDeltaTime);

        newtime = std::chrono::high_resolution_clock::now();
        delta = newtime - lastTime;
        mDeltaTime = delta.count();

        newtime = lastTime;
    }
}

void Engine::PollSdlEvents()
{
    if (SDL_PollEvent(&mSdlEvent) != 0)
    {
        // User requests quit
        if (mSdlEvent.type == SDL_QUIT)
        {
            mRunning = false;
        }

        switch (mSdlEvent.type)
        {
        case SDL_QUIT:
            mRunning = false;
        case SDL_KEYDOWN:
            if (mSdlEvent.key.keysym.sym == SDLK_ESCAPE)
            {
                mRunning = false;
            }
        }

        for (int i = 0; i < mSdlEvents.size(); i++)
        {
            mSdlEvents[i](mSdlEvent);
        }
    }
}

void Engine::Finalize()
{
    SDL_DestroyWindow(mWindow);

	delete sEngine;

    SDL_Quit();
}

void Engine::AddEvent(std::function<void()> event, EventType eventType)
{
    switch (eventType)
    {
    case EventType::START:
        mStartEvents.push_back(event);
        break;
    case EventType::QUIT:
        mQuitEvents.push_back(event);
        break;
    default:
        Debug::CoreWarning("Can't add this event with this overload! Try other AddEvent Method!");
        break;
    }
}

void Engine::AddEvent(std::function<void(double)> event, EventType eventType)
{
    switch (eventType)
    {
    case EventType::UPDATE:
        mUpdateEvents.push_back(event);
        break;
    case EventType::RENDER:
        mRenderEvents.push_back(event);
        break;
    default:
        Debug::CoreWarning("Can't add this event with this overload! Try other AddEvent Method!");
        break;
    }
}

void Engine::AddEvent(std::function<void(SDL_Event &)> event, EventType eventType)
{
    switch (eventType)
    {
    case EventType::SDL:
        mSdlEvents.push_back(event);
        break;
    default:
        Debug::CoreWarning("Can't add this event with this overload! Try other AddEvent Method!");
        break;
    }
}
