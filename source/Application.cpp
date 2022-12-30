#include "Application.h"

void Application::OnStart()
{
    mWorldGrid->CreateGrid();

}

void Application::OnUpdate(double deltaTime)
{
}

void Application::OnRender(double deltaTime)
{
    mWorldGrid->DrawGrid();
}

void Application::OnQuit()
{
}
