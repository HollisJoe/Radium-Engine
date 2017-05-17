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

            int getOriginalNumRenderObjects();

            void setCurrentRenderObject(std::shared_ptr<Engine::RenderObject> renderObject);

            std::shared_ptr<Engine::RenderObject> getCurrentRenderObject();

            int getVertexIndex() const;

            void setVertexIndex (int index);

            /*======================
             --- OTHER METHODS ---
            ======================*/

            bool isVertexSelected() const;

            bool isVertexIndexValid() const;

            void computeVertexIndex(std::shared_ptr<Engine::RenderObject> ro);

            void defineMinimumNumRenderObjects();

            Ra::Core::Vector3 getVertexPosition() const;

            Ra::Core::Vector3 getVertexNormal() const;

        public slots:

            void saveRay(Core::Ray r);

        private:

            int m_vertexIndex;

            Ra::Core::Ray m_ray;

            uint m_originalNumRenderObjects;

            std::shared_ptr<Engine::RenderObject> m_currentRenderObject;
        };
    } // namespace Gui
} // namespace Ra

#endif // RADIUMENGINE_VERTEXPICKING_HPP
