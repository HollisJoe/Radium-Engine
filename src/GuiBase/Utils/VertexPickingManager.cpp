#include <GuiBase/Utils/VertexPickingManager.hpp>

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
            m_currentRenderObject(nullptr)
        {
        }


        Gui::VertexPickingManager::~VertexPickingManager()
        {
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


        //GET Vertex Information

        Core::Vector3 Gui::VertexPickingManager::getVertexPosition() const
        {
            return m_currentRenderObject -> getMesh() -> getGeometry().m_vertices[m_vertexIndex];
        }


        Core::Vector3 Gui::VertexPickingManager::getVertexNormal() const
        {
            return m_currentRenderObject -> getMesh() -> getGeometry().m_normals[m_vertexIndex];
        }
    }
}
