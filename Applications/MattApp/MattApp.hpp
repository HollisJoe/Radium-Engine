#pragma once

#include <Engine/RadiumEngine.hpp>
#include <Engine/System/System.hpp>
#include <Engine/Entity/Entity.hpp>

/// This is a simple tore using a BlinnPhong shader
struct SimpleTorus : public Ra::Engine::Component
{

    SimpleTorus();

    /// This function is called when the component is properly
    /// setup, i.e. it has an entity.
    void initialize() override;

    /// This function will spin our tore
    void spin();

    /// This function will allow us to switch between BlinnPhong and Cartoon shaders
    void switch_shader();

};

/// This system will be added to the engine. Every frame it will
/// add a task to be executed, calling the spin function of the component.
class MinimalSystem : public Ra::Engine::System
{

    public:
        virtual void generateTasks(Ra::Core::TaskQueue *q, const Ra::Engine::FrameInfo &info) override;

};
