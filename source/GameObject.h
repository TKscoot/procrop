#pragma once
#include "Types.h"

class GameObject
{
    friend class Application;

  public:
    GameObject();
	virtual ~GameObject();

    virtual void OnStart() = 0;

    virtual void OnUpdate(double deltaTime) = 0;

    virtual void OnRender(double deltaTime) = 0;

    virtual void OnQuit() = 0;

  protected:
  private:
    String mName = "";
};
