#include "stdafx.h"
#include "comp_object_factory.h"
#include "input_component.h"
#include "physics_component.h"
#include "graphics_component.h"

CompObject* CompObjectFactory::CreateBjorn()
{
    CompObject* bjorn = new CompObject
    (
        new PlayerInputComponent(),
        new BjornPhysicsComponent(),
        new BjornGraphicsComponent()
    );
    SetDefaultValuesForBjorn(*bjorn);

    return bjorn;
}

CompObject* CompObjectFactory::CreateDemoBjorn()
{
    CompObject* bjorn = new CompObject
    (
        new DemoInputComponent(),
        new BjornPhysicsComponent(),
        new BjornGraphicsComponent()
    );
    SetDefaultValuesForBjorn(*bjorn);

    return bjorn;
}

void CompObjectFactory::SetDefaultValuesForBjorn(CompObject& object)
{
    object.SetPosition({ 0.0, 0.0, 0.0 });
    object.SetSize({ 0.8, 0.8 });
    object.SetMass({ 5.0 });
}

