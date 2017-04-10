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

            material->m_kd = Ra::Core::Color(0.5f, 0.0f, 0.8f, 1.0f);

            auto renderObject = Ra::Engine::RenderObject::createRenderObject("CubeRO", this,
                                                                             Ra::Engine::RenderObjectType::Fancy,
                                                                             display,
                                                                             Ra::Engine::ShaderConfigurationFactory::getConfiguration("BlinnPhong"),
                                                                             material);

            float randX, randY, randZ;
            randX = (1000000.f - rand() % 2000000) / 876359.f;
            randY = (1000000.f - rand() % 2000000) / 346798.f;
            randZ = (1000000.f - rand() % 2000000) / 641973.f;

            renderObject->setLocalTransform( renderObject->getLocalTransform() * Ra::Core::Translation( randX, randY, randZ ) * Ra::Core::Transform( Ra::Core::AngleAxis( randX, Ra::Core::Vector3::UnitY() ) ) );

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
                float random = rand() % 100 / 2000.f;
                Ra::Core::AngleAxis aa(random, Ra::Core::Vector3::UnitY());
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
        CORE_ASSERT(m_components.size() == 10, "System incorrectly initialized");

        for(size_t i = 0 ; i < m_components.size() ; ++i)
        {
            MinimalComponent *c = static_cast<MinimalComponent *>(m_components[i].second);
            q->registerTask(new Ra::Core::FunctionTask(std::bind(&MinimalComponent::spin, c), "spin"));
        }
    }