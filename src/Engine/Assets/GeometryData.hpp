#ifndef RADIUMENGINE_GEOMETRY_DATA_HPP
#define RADIUMENGINE_GEOMETRY_DATA_HPP

#include <string>
#include <vector>

#include <Core/Math/LinearAlgebra.hpp>
#include <Core/Containers/VectorArray.hpp>

#include <Engine/Assets/AssetData.hpp>

namespace Ra {
namespace Asset {

struct MaterialData {
    MaterialData();
    MaterialData( const MaterialData& material ) = default;

    /// QUERY
    inline bool hasDiffuse() const;
    inline bool hasSpecular() const;
    inline bool hasShininess() const;
    inline bool hasOpacity() const;
    inline bool hasDiffuseTexture() const;
    inline bool hasSpecularTexture() const;
    inline bool hasShininessTexture() const;
    inline bool hasNormalTexture() const;
    inline bool hasOpacityTexture() const;

    /// VARIABLE
    Core::Color m_diffuse;
    Core::Color m_specular;
    Scalar      m_shininess;
    Scalar      m_opacity;
    std::string m_texDiffuse;
    std::string m_texSpecular;
    std::string m_texShininess;
    std::string m_texNormal;
    std::string m_texOpacity;
    bool        m_hasDiffuse;
    bool        m_hasSpecular;
    bool        m_hasShininess;
    bool        m_hasOpacity;
    bool        m_hasTexDiffuse;
    bool        m_hasTexSpecular;
    bool        m_hasTexShininess;
    bool        m_hasTexNormal;
    bool        m_hasTexOpacity;
};

class GeometryData : public AssetData {

    /// FRIEND
    friend class AssimpGeometryDataLoader;

public:
    using Vector3Array  = Core::VectorArray<Core::Vector3>  ;
    using Vector2iArray = Core::VectorArray<Core::Vector2i> ;
    using Vector2uArray = Core::VectorArray<Core::Vector2ui>;
    using VectorNiArray = Core::VectorArray<Core::VectorNi> ;
    using VectorNuArray = Core::VectorArray<Core::VectorNui>;
    using Vector4Array  = Core::VectorArray<Core::Vector4>  ;
    using ColorArray    = Core::VectorArray<Core::Color>    ;

    using Weight        = std::pair<Scalar, uint>;
    using VertexWeights = std::vector<Weight>;
    using WeightArray   = std::vector<VertexWeights>;

public:

    RA_CORE_ALIGNED_NEW

    /// ENUM
    enum GeometryType {
        UNKNOWN     = 1 << 0,
        POINT_CLOUD = 1 << 1,
        LINE_MESH   = 1 << 2,
        TRI_MESH    = 1 << 3,
        QUAD_MESH   = 1 << 4,
        POLY_MESH   = 1 << 5,
        TETRA_MESH  = 1 << 6,
        HEX_MESH    = 1 << 7
    };

    /// CONSTRUCTOR
    GeometryData( const std::string&  name = "",
                  const GeometryType& type = UNKNOWN );

    GeometryData( const GeometryData& data ) = default;

    /// DESTRUCTOR
    ~GeometryData();

    /// TYPE
    inline GeometryType getType() const;

    /// FRAME
    inline Core::Transform getFrame() const;

    /// DATA
    inline uint getVerticesSize()               const;
    inline const Vector3Array & getVertices()   const;
    inline const Vector2uArray& getEdges()      const;
    inline const VectorNuArray& getFaces()      const;
    inline const VectorNuArray& getPolyhedra()  const;
    inline const Vector3Array & getNormals()    const;
    inline const Vector3Array & getTangents()   const;
    inline const Vector3Array & getBiTangents() const;
    inline const Vector3Array & getTexCoords()  const;
    inline const ColorArray   & getColors()     const;
    inline const WeightArray  & getWeights()    const;
    inline const MaterialData & getMaterial()   const;

    inline const std::map< uint, uint >& getDuplicateTable() const;

    /// DUPLICATES
    inline void setLoadDuplicates( const bool status );

    /// QUERY
    inline bool isPointCloud() const;
    inline bool isLineMesh() const;
    inline bool isTriMesh() const;
    inline bool isQuadMesh() const;
    inline bool isPolyMesh() const;
    inline bool isTetraMesh() const;
    inline bool isHexMesh() const;
    inline bool hasVertices() const;
    inline bool hasEdges() const;
    inline bool hasFaces() const;
    inline bool hasPolyhedra() const;
    inline bool hasNormals() const;
    inline bool hasTangents() const;
    inline bool hasBiTangents() const;
    inline bool hasTextureCoordinates() const;
    inline bool hasColors() const;
    inline bool hasWeights() const;
    inline bool hasMaterial() const;
    inline bool isLoadingDuplicates() const;

    /// DEBUG
    inline void displayInfo() const;

protected:
    /// NAME
    inline void setName( const std::string& name );

    /// TYPE
    inline void setType( const GeometryType& type );

    /// FRAME
    inline void setFrame( const Core::Transform& frame );

    /// VERTEX
    inline void setVertices( const std::vector< Core::Vector3 >& vertexList );

    /// EDGE
    inline void setEdges( const std::vector< Core::Vector2ui >& edgeList );

    /// FACE
    inline void setFaces( const std::vector< Core::VectorNui >& faceList );

    /// POLYHEDRON
    inline void setPolyhedron( const std::vector< Core::VectorNui >& polyList );

    /// NORMAL
    inline void setNormals( const std::vector< Core::Vector3 >& normalList );

    /// TANGENT
    inline void setTangents( const std::vector< Core::Vector3 >& tangentList );

    /// BITANGENT
    inline void setBitangents( const std::vector< Core::Vector3 >& bitangentList );

    /// TEXTURE COORDINATE
    inline void setTextureCoordinates( const std::vector< Core::Vector3 >& texCoordList );

    /// COLOR
    inline void setColors( const std::vector< Core::Color >& colorList );

    /// WEIGHTS
    inline void setWeights(const WeightArray& weightList);

    /// MATERIAL
    inline void setMaterial( const MaterialData& material );

    /// DUPLICATE
    inline void setDuplicateTable( const std::map< uint, uint >& table );

protected:
    /// VARIABLE
    //std::string     m_name;
    Core::Transform m_frame;
    GeometryType    m_type;

    Vector3Array  m_vertex;
    Vector2uArray m_edge;
    VectorNuArray m_faces;
    VectorNuArray m_polyhedron;
    Vector3Array  m_normal;
    Vector3Array  m_tangent;
    Vector3Array  m_bitangent;
    Vector3Array  m_texCoord;
    ColorArray    m_color;
    WeightArray   m_weights;

    MaterialData m_material;
    bool         m_hasMaterial;

    std::map< uint, uint > m_duplicateTable;
    bool                   m_loadDuplicates;
};

} // namespace Asset
} // namespace Ra

#include <Engine/Assets/GeometryData.inl>

#endif // RADIUMENGINE_GEOMETRY_DATA_HPP
