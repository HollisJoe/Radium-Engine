#include <MainApplication/Viewer/Gizmo/Gizmo.hpp>

#include <Core/Math/LinearAlgebra.hpp>
#include <Core/Math/RayCast.hpp>
#include <Core/Math/ColorPresets.hpp>
#include <Core/Containers/VectorArray.hpp>
#include <Core/Mesh/MeshUtils.hpp>

#include <Engine/RadiumEngine.hpp>
#include <Engine/Renderer/Camera/Camera.hpp>
#include <Engine/Entity/Component.hpp>
#include <Engine/Renderer/RenderObject/RenderObject.hpp>
#include <Engine/Renderer/RenderTechnique/RenderTechnique.hpp>
#include <Engine/Renderer/Mesh/Mesh.hpp>

#include <Engine/DebugDisplay/DebugDisplay.hpp>

namespace Ra
{
    Gizmo::~Gizmo()
    {
        for (auto ro : m_renderObjects)
        {
            m_comp->removeRenderObject(ro);
        }

    }

    TranslateGizmo::TranslateGizmo(Engine::Component* c, const Core::Transform& t, Mode mode)
            : Gizmo(c, t, mode ), m_initialPix(Core::Vector2::Zero()), m_selectedAxis (-1)
    {
        constexpr Scalar arrowScale = 0.2f;
        constexpr Scalar axisWidth = 0.05f;
        constexpr Scalar arrowFrac = 0.1f;

        // For x,y,z
        for (uint i = 0; i < 3; ++i)
        {
            Core::Vector3 cylinderEnd = Core::Vector3::Zero();
            Core::Vector3 arrowEnd = Core::Vector3::Zero();
            cylinderEnd[i] = (1.f - arrowFrac);
            arrowEnd[i] = 1.f;

            Core::TriangleMesh cylinder = Core::MeshUtils::makeCylinder(
                    Core::Vector3::Zero(), arrowScale * cylinderEnd, arrowScale * axisWidth / 2.f);

            Core::TriangleMesh cone = Core::MeshUtils::makeCone(arrowScale * cylinderEnd, arrowScale * arrowEnd,
                                                                arrowScale * arrowFrac / 2.f);

            // Merge the cylinder and the cone to create the arrow shape.
            cylinder.append(cone);


            Core::Color arrowColor = Core::Color::Zero();
            arrowColor[i] = 1.f;
            Core::Vector4Array colors(cylinder.m_vertices.size(), arrowColor);

            Engine::Mesh* mesh = new Engine::Mesh("Gizmo Arrow");
            mesh->loadGeometry(cylinder);
            mesh->addData(Engine::Mesh::VERTEX_COLOR, colors);

            Engine::RenderObject* arrowDrawable = new Engine::RenderObject("Gizmo Arrow", m_comp, true);

            Engine::RenderTechnique* rt = new Engine::RenderTechnique;
            rt->shaderConfig = Ra::Engine::ShaderConfiguration("Plain", "../Shaders");
            rt->material = new Ra::Engine::Material("Default material");
            arrowDrawable->setRenderTechnique(rt);
            arrowDrawable->setType(Engine::RenderObject::Type::RO_UI);
            arrowDrawable->setMesh(mesh);

            updateTransform(m_transform);

            m_renderObjects.push_back(m_comp->addRenderObject(arrowDrawable));

        }
    }

    void TranslateGizmo::updateTransform(const Core::Transform& t)
    {
        m_transform = t;
        Core::Transform displayTransform = Core::Transform::Identity();
        if (m_mode == LOCAL )
        {
            displayTransform = m_transform;
        }
        else
        {
            displayTransform.translate(m_transform.translation());
        }

        for (auto roIdx : m_renderObjects)
        {
            Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(roIdx)->setLocalTransform(displayTransform);
        }
    }

    void TranslateGizmo::selectConstraint( int drawableIdx )
    {
        int oldAxis  = m_selectedAxis;
        m_selectedAxis = -1;
        if (drawableIdx >= 0)
        {
            auto found = std::find(m_renderObjects.cbegin(), m_renderObjects.cend(), Core::Index(drawableIdx));
            if (found != m_renderObjects.cend())
            {
                m_selectedAxis = int(found - m_renderObjects.begin());
            }
        }
        if (m_selectedAxis != oldAxis)
        {
            m_initialPix = Core::Vector2::Zero();
        }
    }

    Core::Transform TranslateGizmo::mouseMove(const Engine::Camera& cam, const Core::Vector2& nextXY)
    {
        if (m_selectedAxis >= 0)
        {
            // Taken from Rodolphe's View engine gizmos -- see slide_axis().

            const Core::Vector3 origin = m_transform.translation();
            const Core::Ray ray = cam.getRayFromScreen(nextXY + m_initialPix);

            Core::Vector3 translateDir = Core::Vector3::Unit(m_selectedAxis);
            if (m_mode == LOCAL)
            {
                translateDir = m_transform.rotation() * translateDir;
            }

            // Find a plane passing through axis_dir and as parrallel as possible to
            // the camera image plane
            auto ortho = cam.getDirection().cross(translateDir);
            const Core::Vector3 normal =  (ortho.squaredNorm() > 0) ?
                                          translateDir.cross(ortho) :
                               translateDir.cross(cam.getUpVector());

            std::vector<Scalar> hit;
            bool hasHit = Core::RayCast::vsPlane(ray, origin, normal, hit);

            if (hasHit)
            {
                auto endPoint = origin + (translateDir.dot(ray.at(hit[0]) - origin)) * translateDir;
                m_transform.translation() = endPoint;
            }

        }
        return m_transform;
    }

    void TranslateGizmo::setInitialState(const Engine::Camera& cam, const Core::Vector2& initialXY)
    {
        const Core::Vector3 origin = m_transform.translation();
        const Core::Vector2 orgScreen = cam.project(origin);
        m_initialPix = orgScreen - initialXY;
    }
}
