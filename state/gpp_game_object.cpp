#include "gpp_game_object.h"
#include "gpp_world.h"

void GPPGameObject::SetGPPWorld(GPPWorld& world)
{
    m_world = &world;
}

GPPWorld* GPPGameObject::GetGPPWorld()
{
    return m_world;
}
