#ifndef RADIUMENGINE_PHOTOSTUDIORENDERER_H
#define RADIUMENGINE_PHOTOSTUDIORENDERER_H

#include <Engine/RadiumEngine.hpp>
#include <Engine/Renderer/Renderer.hpp>
#include <Engine/Renderer/Light/DirLight.hpp>
#include <Engine/Renderer/Light/PointLight.hpp>

namespace Ra
{
    namespace Engine
    {
        class RA_ENGINE_API PhotoStudioRenderer : public Renderer
        {
            public:
                PhotoStudioRenderer( uint width, uint height );
                virtual ~PhotoStudioRenderer();

                virtual std::string getRendererName() const override { return "PhotoStudio Renderer"; }

            protected:
                virtual void initializeInternal() override;
                virtual void resizeInternal() override;

                virtual void updateStepInternal( const RenderData& renderData ) override;

                virtual void postProcessInternal( const RenderData& renderData ) override;
                virtual void renderInternal( const RenderData& renderData ) override;
                virtual void debugInternal( const RenderData& renderData ) override;
                virtual void uiInternal( const RenderData& renderData ) override;

            private:
                void initShaders();
                void initBuffers();
                void initLights();

                void updateShadowMaps();
                void configureLights();
                void compute3PointsLighting( const RenderData& renderData, const std::vector<RenderObjectPtr>& renderObjects );
                Core::Aabb getAabb();

            private:
                enum RendererTextures
                {
                    RendererTextures_Depth = 0,
                    RendererTextures_HDR,
                    RendererTextures_Normal,
                    RendererTextures_Diffuse,
                    RendererTextures_Specular,
                    RendererTextures_OITAccum,
                    RendererTextures_OITRevealage,
                    RendererTexture_Count
                };

                // Default renderer logic here, no need to be accessed by overriding renderers.
                std::unique_ptr<FBO> m_fbo;
                std::unique_ptr<FBO> m_postprocessFbo;
                std::unique_ptr<FBO> m_oitFbo;

                std::vector<RenderObjectPtr> m_transparentRenderObjects;
                uint m_fancyTransparentCount;

                uint m_pingPongSize;

                std::array<std::unique_ptr<Texture>, RendererTexture_Count> m_textures;

                static const int ShadowMapSize = 1024;
                std::vector<std::shared_ptr<Texture>> m_shadowMaps;
                std::vector<Core::Matrix4> m_lightMatrices;

                std::unique_ptr<PointLight> m_keyLight;
                std::unique_ptr<PointLight> m_fillLight;
                std::unique_ptr<PointLight> m_backLight;

        };

    } // namespace Engine
} // namespace Ra

#endif //RADIUMENGINE_PHOTOSTUDIORENDERER_H
