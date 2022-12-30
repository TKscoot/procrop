#pragma once
#include "Debug.h"
#include "GlobalSettings.h"
#include "Helpers.h"
#include "TextRenderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <functional>
#include <iostream>
#include <vector>
#include <chrono>

enum class EventType
{
	START,
	UPDATE,
	RENDER,
	QUIT,
	SDL
};

class Engine
{
  private:
    Engine();

  public:
    static Engine *Inst()
    {
        if (sEngine == nullptr)
        {
            sEngine = new Engine();
        }

        return sEngine;
    }

    void PollSdlEvents();

    void Initialize();
    void Run();
    void Finalize();

	void AddEvent(std::function<void()> event, EventType eventType);
	void AddEvent(std::function<void(double)> event, EventType eventType);
	void AddEvent(std::function<void(SDL_Event&)> event, EventType eventType);

  public:
    bool mRunning = true;
    SDL_Window *mWindow;
    SDL_Renderer *mRenderer;

	TextRenderer* mTextRenderer = nullptr;

	double mDeltaTime = 0.0;
	
  private:
    static Engine *sEngine;

    SDL_Event mSdlEvent;

    std::vector<std::function<void(SDL_Event &)>> mSdlEvents;
    std::vector<std::function<void()>> mStartEvents;
    std::vector<std::function<void(double)>> mUpdateEvents;
    std::vector<std::function<void(double)>> mRenderEvents;
    std::vector<std::function<void()>> mQuitEvents;

};
