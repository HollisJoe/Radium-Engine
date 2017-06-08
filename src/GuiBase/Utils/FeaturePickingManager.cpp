#include <GuiBase/Utils/FeaturePickingManager.hpp>
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

        FeaturePickingManager::FeaturePickingManager() :
            m_vertexIndex(-1),
            m_originalNumRenderObjects(0),
            m_currentRenderObject(nullptr),
            m_sphereComponent(nullptr)
        {
        }


        FeaturePickingManager::~FeaturePickingManager()
        {
            delete m_sphereComponent;
        }


        /*=========================
         --- GETTERS & SETTERS ---
        =========================*/


        //OriginalNumrenderObject

        int FeaturePickingManager::getOriginalNumRenderObjects()
        {
            return int(m_originalNumRenderObjects);
        }


        //CurrentRenderObject

        void FeaturePickingManager::setCurrentRenderObject(std::shared_ptr<Engine::RenderObject> renderObject)
        {
            m_currentRenderObject = renderObject;
        }

        std::shared_ptr<Engine::RenderObject> FeaturePickingManager::getCurrentRenderObject()
        {
            return m_currentRenderObject;
        }


        //VertexIndex

        int FeaturePickingManager::getVertexIndex() const
        {
            return m_vertexIndex;
        }

        void FeaturePickingManager::setVertexIndex (int index)
        {
            m_vertexIndex = index;
        }

        /*======================
         --- OTHER METHODS ---
        ======================*/

        //Boolean Controlers

        bool FeaturePickingManager::isVertexSelected() const
        {
            return m_currentRenderObject != nullptr;
        }


        bool FeaturePickingManager::isVertexIndexValid() const
        {
            return m_vertexIndex > -1;
        }


        void FeaturePickingManager::saveRay(Core::Ray r) //Mettre arg input en reference
        {
            m_ray = r;
        }


        // Here to avoid selecting features on static grid or static frame or gizmos
        void FeaturePickingManager::defineMinimumNumRenderObjects()
        {
            Ra::Engine::RadiumEngine* engine = Ra::Engine::RadiumEngine::getInstance();
            m_originalNumRenderObjects = engine -> getRenderObjectManager() -> getNumRenderObjects();
        }


        void FeaturePickingManager::computeVertexIndex(std::shared_ptr<Engine::RenderObject> ro)
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

        void FeaturePickingManager::displaySphere ()
        {
            Ra::Engine::RadiumEngine* engine = Ra::Engine::RadiumEngine::getInstance();
            Ra::Engine::Entity* e = engine->getEntityManager()->createEntity("Sphere");
            m_sphereComponent = new SphereComponent;
            e->addComponent(m_sphereComponent);
            m_sphereComponent -> initialize();
        }

        void FeaturePickingManager::setSpherePosition ()
        {
            auto sphereRoName = m_sphereComponent -> getSphereRo() -> getName();
            auto currentRoName = m_currentRenderObject -> getName();

            if( m_sphereComponent && (sphereRoName != currentRoName ) )
            {
                m_sphereComponent -> setPosition(getVertexPosition());
            }
        }

        //GET Vertex Information

        Core::Vector3 FeaturePickingManager::getVertexPosition() const
        {
            return m_currentRenderObject -> getMesh() -> getGeometry().m_vertices[m_vertexIndex];
        }


        Core::Vector3 FeaturePickingManager::getVertexNormal() const
        {
            return m_currentRenderObject -> getMesh() -> getGeometry().m_normals[m_vertexIndex];
        }



        SphereComponent::SphereComponent()
                : Ra::Engine::Component("SphereC Component"), m_sphereRo (nullptr)
        {
            m_sphere = Ra::Core::MeshUtils::makeParametricSphere(0.1);
        }


        void SphereComponent::initialize()
        {
            // Create a cube mesh render object.
            std::shared_ptr<Ra::Engine::Mesh> display(new Ra::Engine::Mesh("FeaturePickingManagerSphere"));
            display->loadGeometry(m_sphere);
            std::shared_ptr<Ra::Engine::Material> material;
            material.reset( new Ra::Engine::Material("VertexPickingManageSphereMaterial") );
            material-> m_kd = Ra::Core::Color(1.f,0.f,0.f,1.f);
            m_sphereRo = Ra::Engine::RenderObject::createRenderObject("FeaturePickingManagerSphereRO",
                                                                      this,
                                                                      Ra::Engine::RenderObjectType::Fancy,
                                                                      display,
                                                                      Ra::Engine::ShaderConfigurationFactory::getConfiguration("BlinnPhong"),
                                                                      material);
            addRenderObject(m_sphereRo);
        }


        void SphereComponent::setPosition (Ra::Core::Vector3 position)
        {
            if (m_sphereRo)
            {
                Ra::Core::Translation aa(position);
                Ra::Core::Transform rot(aa);
                m_sphereRo -> setLocalTransform( rot );
            }
        }

        Engine::RenderObject* SphereComponent::getSphereRo ()
        {
            return m_sphereRo;
        }
    }
}
