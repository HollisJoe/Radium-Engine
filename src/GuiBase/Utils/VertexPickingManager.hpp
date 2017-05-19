#ifndef RADIUMENGINE_VERTEXPICKING_HPP
#define RADIUMENGINE_VERTEXPICKING_HPP

#include <GuiBase/RaGuiBase.hpp>

#include <QObject>

#include <Engine/RaEngine.hpp>
#include <Engine/RadiumEngine.hpp>
#include <Core/Mesh/MeshPrimitives.hpp>
#include <Engine/Component/Component.hpp>
#include <Engine/Renderer/RenderTechnique/ShaderConfigFactory.hpp>
#include <Engine/Renderer/RenderObject/RenderObjectManager.hpp>

namespace Ra
{
    namespace Gui
    {

        //Sphere Component

        class SphereComponent : public Ra::Engine::Component
        {
        public :

            SphereComponent();

            /// This function is called when the component is properly
            /// setup, i.e. it has an entity.
            void initialize() override;

    //                Ra::Core::Vector3 position
            void setPosition (Ra::Core::Vector3 position);

            Engine::RenderObject* getSphereRo ();

        private:
            Ra::Core::TriangleMesh m_sphere;
            Engine::RenderObject* m_sphereRo;
        };


        //VertexPickingManager

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

            void displaySphere ();

            void setSpherePosition ();

            Ra::Core::Vector3 getVertexPosition() const;

            Ra::Core::Vector3 getVertexNormal() const;

        public slots:

            void saveRay(Core::Ray r);

        private:

            int m_vertexIndex;

            Ra::Core::Ray m_ray;

            uint m_originalNumRenderObjects;

            std::shared_ptr<Engine::RenderObject> m_currentRenderObject;

            Ra::Gui::SphereComponent* m_sphereComponent;
        };
    } // namespace Gui
} // namespace Ra

#endif // RADIUMENGINE_VERTEXPICKING_HPP
