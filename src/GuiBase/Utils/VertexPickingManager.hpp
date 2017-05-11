#ifndef RADIUMENGINE_VERTEXPICKING_HPP
#define RADIUMENGINE_VERTEXPICKING_HPP

#include <GuiBase/RaGuiBase.hpp>

#include <QObject>

#include <Engine/RaEngine.hpp>
#include <Engine/RadiumEngine.hpp>

//Class added by Axel

namespace Ra
{
    namespace Gui
    {
        class VertexPickingManager : public QObject
        {
            Q_OBJECT

        public:

            /*================================
             --- CONSTRUCTOR & DESTRUCTOR ---
            ================================*/

            VertexPickingManager();

            ~VertexPickingManager();


            /*=========================
             --- GETTERS & SETTERS ---
            =========================*/

            void setTrackedVertex(Ra::Core::Vector3* vertex);

            Ra::Core::Vector3& getTrackedVertex();

            int getOriginalNumRenderObjects();

            void setCurrentRenderObject(std::shared_ptr<Engine::RenderObject> renderObject);

            std::shared_ptr<Engine::RenderObject> getCurrentRenderObject();


            /*======================
             --- OTHER METHODS ---
            ======================*/

            bool vertexSelected();

            int getVertexIndex(std::shared_ptr<Engine::RenderObject> ro);



        public slots:

            void saveRay(Core::Ray r);


        private:

            void defineMinimumNumRenderObjects();

        private:

            /// Stores the address of a selected vertex.
            Ra::Core::Vector3* m_trackedVertex;

            Ra::Core::Ray m_ray;

            std::shared_ptr<Engine::RenderObject> m_currentRenderObject;

            uint m_originalNumRenderObjects;
        };
    } // namespace Gui
} // namespace Ra

#endif // RADIUMENGINE_VERTEXPICKING_HPP
