#include <GuiBase/Utils/VertexPickingManager.hpp>
#include <Engine/Entity/Entity.hpp>
#include <Core/Containers/MakeShared.hpp>

#include <Engine/Renderer/RenderObject/RenderObject.hpp>
#include <Engine/Managers/EntityManager/EntityManager.hpp>
#include <Engine/Renderer/Mesh/Mesh.hpp>
#include <Engine/Renderer/RenderTechnique/Material.hpp>

namespace Ra
{
    namespace Gui
    {
        /*================================
         --- CONSTRUCTOR & DESTRUCTOR ---
        ================================*/

        Gui::VertexPickingManager::VertexPickingManager() :
            m_vertexIndex(-1),
            m_originalNumRenderObjects(0),
            m_currentRenderObject(nullptr),
            m_sphereComponent(nullptr)
        {
        }


        Gui::VertexPickingManager::~VertexPickingManager()
        {
            delete m_sphereComponent;
        }


        /*=========================
         --- GETTERS & SETTERS ---
        =========================*/


        //OriginalNumrenderObject

        int Gui::VertexPickingManager::getOriginalNumRenderObjects()
        {
            return int(m_originalNumRenderObjects);
        }


        //CurrentRenderObject

        void Gui::VertexPickingManager::setCurrentRenderObject(std::shared_ptr<Engine::RenderObject> renderObject)
        {
            m_currentRenderObject = renderObject;
        }

        std::shared_ptr<Engine::RenderObject> Gui::VertexPickingManager::getCurrentRenderObject()
        {
            return m_currentRenderObject;
        }


        //VertexIndex

        int Gui::VertexPickingManager::getVertexIndex() const
        {
            return m_vertexIndex;
        }

        void Gui::VertexPickingManager::setVertexIndex (int index)
        {
            m_vertexIndex = index;
        }

        /*======================
         --- OTHER METHODS ---
        ======================*/

        //Boolean Controlers

        bool Gui::VertexPickingManager::isVertexSelected() const
        {
            return m_currentRenderObject != nullptr;
        }


        bool Gui::VertexPickingManager::isVertexIndexValid() const
        {
            return m_vertexIndex > -1;
        }


        void Gui::VertexPickingManager::saveRay(Core::Ray r) //Mettre arg input en reference
        {
            m_ray = r;
        }


        void Gui::VertexPickingManager::defineMinimumNumRenderObjects()
        {
            Ra::Engine::RadiumEngine* engine = Ra::Engine::RadiumEngine::getInstance();
            m_originalNumRenderObjects = engine -> getRenderObjectManager() -> getNumRenderObjects();
        }


        void Gui::VertexPickingManager::computeVertexIndex(std::shared_ptr<Engine::RenderObject> ro)
        {
            const Ra::Core::Transform& t = ro->getLocalTransform();
            Core::Ray transformedRay = Ra::Core::transformRay(m_ray, t.inverse());
            auto result = Ra::Core::MeshUtils::castRay(ro -> getMesh() -> getGeometry(), transformedRay);

            const int& tidx = result.m_hitTriangle;

            if (tidx >= 0)
            {
                m_vertexIndex = result.m_nearestVertex;
            }
            else
            {
                m_vertexIndex = -1;
            }
        }

        void Gui::VertexPickingManager::displaySphere ()
        {
            Ra::Engine::RadiumEngine* engine = Ra::Engine::RadiumEngine::getInstance();
            Ra::Engine::Entity* e = engine->getEntityManager()->createEntity("Sphere");
            m_sphereComponent = new SphereComponent;
            e->addComponent(m_sphereComponent);
            m_sphereComponent -> initialize();
        }

        void Gui::VertexPickingManager::setSpherePosition ()
        {
            auto sphereRoName = m_sphereComponent -> getSphereRo() -> getName();
            auto currentRoName = m_currentRenderObject -> getName();

            if( m_sphereComponent && (sphereRoName != currentRoName ) )
            {
                m_sphereComponent -> setPosition(getVertexPosition());
            }
        }

        //GET Vertex Information

        Core::Vector3 Gui::VertexPickingManager::getVertexPosition() const
        {
            return m_currentRenderObject -> getMesh() -> getGeometry().m_vertices[m_vertexIndex];
        }


        Core::Vector3 Gui::VertexPickingManager::getVertexNormal() const
        {
            return m_currentRenderObject -> getMesh() -> getGeometry().m_normals[m_vertexIndex];
        }



        SphereComponent::SphereComponent()
                : Ra::Engine::Component("SphereC Component"), m_sphereRo (nullptr)
        {
            m_sphere = Ra::Core::MeshUtils::makeParametricSphere(0.1);
        }


        void Gui::SphereComponent::initialize()
        {
            // Create a cube mesh render object.
            std::shared_ptr<Ra::Engine::Mesh> display(new Ra::Engine::Mesh("VertexpickingManagerSphere"));
            display->loadGeometry(m_sphere);
            std::shared_ptr<Ra::Engine::Material> material;
            material.reset( new Ra::Engine::Material("VertexPickingManageSphereMaterial") );
            material-> m_kd = Ra::Core::Color(1.f,0.f,0.f,1.f);
            m_sphereRo = Ra::Engine::RenderObject::createRenderObject("VertexpickingManagerSphereRO",
                                                                      this,
                                                                      Ra::Engine::RenderObjectType::Fancy,
                                                                      display,
                                                                      Ra::Engine::ShaderConfigurationFactory::getConfiguration("BlinnPhong"),
                                                                      material);
            addRenderObject(m_sphereRo);
        }


        void Gui::SphereComponent::setPosition (Ra::Core::Vector3 position)
        {
            if (m_sphereRo)
            {
                Ra::Core::Translation aa(position);
                Ra::Core::Transform rot(aa);
                m_sphereRo -> setLocalTransform( rot );
            }
        }

        Engine::RenderObject* Gui::SphereComponent::getSphereRo ()
        {
            return m_sphereRo;
        }
    }
}
