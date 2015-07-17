#ifndef RADIUMENGINE_MESH_HPP
#define RADIUMENGINE_MESH_HPP

#include <vector>

#include <Core/CoreMacros.hpp>
#include <Core/Math/Vector.hpp>
#include <Core/Containers/VectorArray.hpp>
#include <Core/Mesh/TriangleMesh.hpp>
#include <Engine/Renderer/Drawable/Drawable.hpp>

namespace Ra { namespace Engine {

class Mesh : public Drawable
{
public:
    Mesh(const std::string& name);
    virtual ~Mesh();

    void loadGeometry(const Core::TriangleMesh &data, bool computeNormals = false);
    void loadGeometry(const Core::TriangleMesh &data,
                      const Core::VectorArray<Core::Vector3>& tangents,
                      const Core::VectorArray<Core::Vector3>& bitangents,
                      bool computeNormals = false);
    const Core::TriangleMesh& getMeshData() const { return m_data; }

    virtual void draw() override;

private:
    Mesh(const Mesh&) = delete;
    void operator=(const Mesh&) = delete;
    void initGL();
    void computeNormals();

private:
    Core::TriangleMesh m_data;
    Core::VectorArray<Core::Vector3> m_tangents;
    Core::VectorArray<Core::Vector3> m_bitangents;

    uint m_vao;
    uint m_vbo;
    uint m_nbo;
    uint m_tbo;
    uint m_bbo;

    uint m_ibo;

    bool m_initialized;
};

} // namespace Engine
} // namespace Ra

#endif // RADIUMENGINE_MESH_HPP
