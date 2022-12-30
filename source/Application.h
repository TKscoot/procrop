#pragma once
#include "Debug.h"
#include "GameObject.h"
#include "Image.h"
#include "Random.h"
#include "WorldGrid.h"
#include "Player.h"
#include <vector>

class Application
{
  public:
    Application()
    {
    }


	void OnAwake(){
		CreateGameObject<Player>("Player");
        mWorldGrid = new WorldGrid(dynamic_cast<Player*>(mGameObjects[0]), {60, 60});
	}
	
    void OnStart();

    void OnUpdate(double deltaTime);
    void OnRender(double deltaTime);

    void OnQuit();

	template<typename T>
    T *CreateGameObject(const String& name)
    {
		//static_assert(std::is_base_of<GameObject, T>::value, "T must derive from GameObject");

        T *go = new T();
        go->mName = name;

        static std::function<void()> goStart = std::bind(&GameObject::OnStart, go);
        Engine::Inst()->AddEvent(goStart, EventType::START);

        static std::function<void()> goQuit = std::bind(&GameObject::OnQuit, go);
        Engine::Inst()->AddEvent(goQuit, EventType::QUIT);

        static std::function<void(double)> goUpdate = std::bind(&GameObject::OnUpdate, go, std::placeholders::_1);
        Engine::Inst()->AddEvent(goUpdate, EventType::UPDATE);

        static std::function<void(double)> goRender = std::bind(&GameObject::OnRender, go, std::placeholders::_1);
        Engine::Inst()->AddEvent(goRender, EventType::RENDER);

        mGameObjects.push_back(go);

        //return dynamic_cast<T*>(go);
		return go;
    }

  private:
    Vector<GameObject *> mGameObjects;

    WorldGrid *mWorldGrid = nullptr;
};
