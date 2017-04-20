#include <MattApp.hpp>

#include <QtCore/Qt>

#include <Core/Time/Timer.hpp>
#include <Core/Tasks/Task.hpp>
#include <Core/Tasks/TaskQueue.hpp>
#include <Core/Mesh/MeshPrimitives.hpp>

#include <Engine/Renderer/Mesh/Mesh.hpp>

#include <Engine/Renderer/RenderObject/RenderObjectManager.hpp>
#include <Engine/Renderer/RenderObject/RenderObject.hpp>
#include <Engine/Renderer/RenderTechnique/Material.hpp>
#include <Engine/Renderer/RenderTechnique/RenderTechnique.hpp>

#include <GuiBase/Utils/Keyboard.hpp>
#include <GuiBase/Utils/KeyMappingManager.hpp>

/* This file contains a minimal radium/qt application which shows the
classic "Spinning Cube" demo. */

/// This is a simple tore using a BlinnPhong shader

SimpleTorus::SimpleTorus() : Ra::Engine::Component("Simple Torus")
{

}

/// This function is called when the component is properly
/// setup, i.e. it has an entity.
void SimpleTorus::initialize()
{
    // Create a cube mesh render object.

    std::shared_ptr<Ra::Engine::Mesh> display(new Ra::Engine::Mesh("SimpleTorus"));
    display->loadGeometry(Ra::Core::MeshUtils::makeParametricTorus<100, 40>(0.25, 0.1));

    auto *material = new Ra::Engine::Material("MattMaterial");

    material->m_kd = Ra::Core::Color(0.3f, 0.3f, 0.7f, 1.0f);

    auto renderObject = Ra::Engine::RenderObject::createRenderObject("SimpleTorusRO", this,
                                                                     Ra::Engine::RenderObjectType::Fancy,
                                                                     display,
                                                                     Ra::Engine::ShaderConfigurationFactory::getConfiguration("BlinnPhong"),
                                                                     material);
    addRenderObject(renderObject);
}

/// This function will spin our cube
void SimpleTorus::spin()
{
    static bool doSpin = false;

    if(Ra::Gui::isKeyPressed(Qt::Key_F1))
    {
        Ra::Gui::keyReleased(Qt::Key_F1);
        doSpin = !doSpin;
    }

    if(doSpin)
    {
        Ra::Core::AngleAxis aa(0.01f, Ra::Core::Vector3::UnitY());
        Ra::Core::Transform rot(aa);

        auto ro = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(
                m_renderObjects[0]);
        Ra::Core::Transform t = ro->getLocalTransform();
        ro->setLocalTransform(rot * t);
    }

    if(Ra::Gui::isKeyPressed(Qt::Key_F3))
    {
        Ra::Gui::keyReleased(Qt::Key_F3);
        Ra::Gui::KeyMappingManager::getInstance()->reloadConfiguration();
    }
}

void SimpleTorus::switch_shader()
{
    static bool cartoonShader = false;

    if(Ra::Gui::isKeyPressed( Qt::Key_F2 ))
    {
        Ra::Gui::keyReleased( Qt::Key_F2 );

        auto ro = Ra::Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(
                m_renderObjects[0]);
        auto rt = ro->getRenderTechnique();

        if(!cartoonShader)
        {
            rt->changeShader(Ra::Engine::ShaderConfigurationFactory::getConfiguration("Cartoon"));
        }
        else
        {
            rt->changeShader(Ra::Engine::ShaderConfigurationFactory::getConfiguration("BlinnPhong"));
        }

        cartoonShader = !cartoonShader;
    }
}

/// This system will be added to the engine. Every frame it will
/// add a task to be executed, calling the spin function of the component.
void MinimalSystem::generateTasks(Ra::Core::TaskQueue *q, const Ra::Engine::FrameInfo &info)
{
    SimpleTorus * c_simpleTorus = static_cast<SimpleTorus *>(m_components[0].second);
    q->registerTask(new Ra::Core::FunctionTask(std::bind(&SimpleTorus::spin, c_simpleTorus), "spin"));
    q->registerTask(new Ra::Core::FunctionTask(std::bind(&SimpleTorus::switch_shader, c_simpleTorus), "switch_shader"));
}