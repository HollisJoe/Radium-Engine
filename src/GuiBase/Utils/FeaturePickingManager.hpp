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
            void setScale (Scalar scale);

            Engine::RenderObject* getSphereRo ();

        private:
            Ra::Core::TriangleMesh m_sphere;
            Engine::RenderObject* m_sphereRo;
        };


        struct FeatureData
        {
            FeatureData() : m_featureType(Engine::Renderer::RO), m_roIdx(-1)
            {}

            Engine::Renderer::PickingMode m_featureType;
            std::vector< int > m_data;
            uint m_roIdx;
        };

        class FeaturePickingManager
        {
        public:

        public:

            FeaturePickingManager();

            ~FeaturePickingManager();

            /// Registers the index of the first RenderObject from which feature picking is enabled.
            inline void setMinRenderObjectIndex(uint id)
            {
                m_firstRO = id;
            }

            void doPicking( int roIndex, const Engine::Renderer::PickingQuery& query, const Core::Ray& ray );

            inline const FeatureData& getFeatureData() const
            {
                return m_FeatureData;
            }

            inline FeatureData& getFeatureData()
            {
                return m_FeatureData;
            }

            void clearFeature();

        private:
            FeatureData m_FeatureData;

        public:

            /**/
            void setVertexIndex(int id);

            /**/
            bool isVertexSelected() const;

            /**/
            void setSpherePosition();

            /**/
            Ra::Core::Vector3 getFeaturePosition() const;

            /**/
            Ra::Core::Vector3 getFeatureVector() const;

        private:
            /**/
            Scalar getScaleFromFeature() const;

            int m_vertexIndex;

            Ra::Core::Ray m_ray;

            uint m_firstRO;

            Ra::Gui::SphereComponent* m_sphereComponent;
        };
    } // namespace Gui
} // namespace Ra

#endif // RADIUMENGINE_VERTEXPICKING_HPP
