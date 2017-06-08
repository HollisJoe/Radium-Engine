#ifndef RADIUMENGINE_VERTEXPICKING_HPP
#define RADIUMENGINE_VERTEXPICKING_HPP

#include <GuiBase/RaGuiBase.hpp>

#include <Engine/RaEngine.hpp>
#include <Engine/RadiumEngine.hpp>
#include <Core/Mesh/MeshPrimitives.hpp>
#include <Engine/Component/Component.hpp>
#include <Engine/Renderer/Renderer.hpp>
#include <Engine/Renderer/RenderTechnique/ShaderConfigFactory.hpp>
#include <Engine/Renderer/RenderObject/RenderObjectManager.hpp>

#include <QObject>

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

            void setPosition (Ra::Core::Vector3 position);

            Engine::RenderObject* getSphereRo ();

        private:
            Ra::Core::TriangleMesh m_sphere;
            Engine::RenderObject* m_sphereRo;
        };


        //VertexPickingManager

        class FeaturePickingManager
        {
        public:

            struct FeatureData
            {
                Engine::Renderer::PickingMode m_featureType;
                std::vector< int > m_data;
            };

        public:

            FeaturePickingManager();

            ~FeaturePickingManager();

            void defineMinimumNumRenderObjects();

            void doPicking( Engine::Renderer::PickingQuery, int roIndex );

            inline const FeatureData& getFeatureData() const
            {
                return m_FeatureData;
            }

            inline FeatureData& getFeatureData()
            {
                return m_FeatureData;
            }

            void clearPicking();

        private:
            FeatureData m_FeatureData;

        public:

            /*=========================
             --- GETTERS & SETTERS ---
            =========================*/

            /**/
            int getOriginalNumRenderObjects();

            /**/
            void setCurrentRenderObject(std::shared_ptr<Engine::RenderObject> renderObject);

            /**/
            std::shared_ptr<Engine::RenderObject> getCurrentRenderObject();

            /**/
            int getVertexIndex() const;

            /**/
            void setVertexIndex (int index);

            /*======================
             --- OTHER METHODS ---
            ======================*/

            /**/
            bool isVertexSelected() const;

            /**/
            bool isVertexIndexValid() const;

            /**/
            void computeVertexIndex(std::shared_ptr<Engine::RenderObject> ro);

            /**/
            void displaySphere ();

            /**/
            void setSpherePosition ();

            /**/
            Ra::Core::Vector3 getVertexPosition() const;

            /**/
            Ra::Core::Vector3 getVertexNormal() const;

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
