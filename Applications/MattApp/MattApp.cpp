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

/* This file contains a minimal radium/qt application which shows the
classic "Spinning Cube" demo. */

/// This is a very basic component which holds a spinning cube.

        MinimalComponent::MinimalComponent() : Ra::Engine::Component("Minimal Component") {

        }

        /// This function is called when the component is properly
        /// setup, i.e. it has an entity.
        void  MinimalComponent::initialize() {
            // Create a cube mesh render object.
            std::shared_ptr<Ra::Engine::Mesh> display(new Ra::Engine::Mesh("Torus"));
            display->loadGeometry(Ra::Core::MeshUtils::makeParametricTorus(0.25f, 0.1f));

            auto *material = new Ra::Engine::Material("MattMaterial");

            material->m_kd = Ra::Core::Color(0.0f, 0.8f, 0.2f, 1.0f);

            auto renderObject = Ra::Engine::RenderObject::createRenderObject("CubeRO", this,
                                                                             Ra::Engine::RenderObjectType::Fancy,
                                                                             display,
                                                                             Ra::Engine::ShaderConfigurationFactory::getConfiguration("Cartoon"),
                                                                             material);
            addRenderObject(renderObject);
        }

        /// This function will spin our cube
        void  MinimalComponent::spin() {
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
        }

/// This system will be added to the engine. Every frame it will
/// add a task to be executed, calling the spin function of the component.
     void MinimalSystem::generateTasks(Ra::Core::TaskQueue *q, const Ra::Engine::FrameInfo &info) {
        // We check that our component is here.
        CORE_ASSERT(m_components.size() == 1, "System incorrectly initialized");
        MinimalComponent *c = static_cast<MinimalComponent *>(m_components[0].second);

        // Create a new task which wil call c->spin() when executed.
        q->registerTask(new Ra::Core::FunctionTask(std::bind(&MinimalComponent::spin, c), "spin"));
    }