#include <GuiBase/Utils/VertexPickingManager.hpp>

namespace Ra
{
    namespace Gui
    {

        /*================================
         --- CONSTRUCTOR & DESTRUCTOR ---
        ================================*/

        Gui::VertexPickingManager::VertexPickingManager() :
            m_trackedVertex(nullptr),
            m_currentRenderObject(nullptr),
            m_originalNumRenderObjects(0)
        {
        }

        Gui::VertexPickingManager::~VertexPickingManager()
        {
        }


        /*=========================
         --- GETTERS & SETTERS ---
        =========================*/

        //TrackedVertex

        void Gui::VertexPickingManager::setTrackedVertex(Ra::Core::Vector3* vertex)
        {
            m_trackedVertex = vertex;
        }

        Core::Vector3& Gui::VertexPickingManager::getTrackedVertex()
        {
            return *m_trackedVertex;
        }

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


        /*======================
         --- OTHER METHODS ---
        ======================*/


        void Gui::VertexPickingManager::saveRay(Core::Ray r) //Mettre arg input en reference
        {
            m_ray = r;
        }


        void Gui::VertexPickingManager::defineMinimumNumRenderObjects()
        {
            Ra::Engine::RadiumEngine* engine = Ra::Engine::RadiumEngine::getInstance();
            m_originalNumRenderObjects = engine ->getRenderObjectManager() -> getNumRenderObject();
        }



        int Gui::VertexPickingManager::getVertexIndex(std::shared_ptr<Engine::RenderObject> ro)
        {
            const Ra::Core::Transform& t = ro->getLocalTransform();
            Core::Ray transformedRay = Ra::Core::transformRay(m_ray, t.inverse());
            auto result = Ra::Core::MeshUtils::castRay(ro -> getMesh() -> getGeometry(), transformedRay);

            const int& tidx = result.m_hitTriangle;

            if (tidx >= 0)
            {
                return result.m_nearestVertex;
            }

            return -1;
        }

        bool Gui::VertexPickingManager::vertexSelected()
        {
            return m_trackedVertex;
        }
    }
}
