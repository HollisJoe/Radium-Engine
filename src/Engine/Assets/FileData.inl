
#include <Core/Log/Log.hpp>
#include <Engine/Assets/GeometryData.hpp>
#include <Engine/Assets/HandleData.hpp>
#include <Engine/Assets/AnimationData.hpp>

namespace Ra {
namespace Asset {


/// FILENAME
inline std::string FileData::getFileName() const {
    return m_filename;
}

inline void FileData::setFileName( const std::string& filename ) {
    m_filename = filename;
}

/// TIMING
inline Scalar FileData::getLoadingTime() const {
    return m_loadingTime;
}

/// DATA
inline std::vector< GeometryData* > FileData::getGeometryData() const {
    std::vector< GeometryData* > list;
    list.reserve( m_geometryData.size() );
    for( const auto& item : m_geometryData ) {
        list.push_back( item.get() );
    }
    return list;
}

inline std::vector< HandleData* > FileData::getHandleData() const {
    std::vector< HandleData* > list;
    list.reserve( m_handleData.size() );
    for( const auto& item : m_handleData ) {
        list.push_back( item.get() );
    }
    return list;
}

inline std::vector< AnimationData* > FileData::getAnimationData() const {
    std::vector< AnimationData* > list;
    list.reserve( m_animationData.size() );
    for( const auto& item : m_animationData ) {
        list.push_back( item.get() );
    }
    return list;
}

inline void FileData::setVerbose( const bool VERBOSE_MODE ) {
    m_verbose = VERBOSE_MODE;
}

/// QUERY
inline bool FileData::isInitialized() const {
    return ( ( m_filename != "" ) && !m_processed );
}

inline bool FileData::isProcessed() const {
    return m_processed;
}

inline bool FileData::hasGeometry() const {
    return ( !m_geometryData.empty() );
}

inline bool FileData::hasHandle() const {
    return ( !m_handleData.empty() );
}

inline bool FileData::hasAnimation() const {
    return ( !m_animationData.empty() );
}

inline bool FileData::isVerbose() const {
    return m_verbose;
}

/// RESET
inline void FileData::reset()
{
    m_filename = "";
    m_geometryData.clear();
    m_handleData.clear();
    m_animationData.clear();
    m_processed = false;
}

inline void FileData::displayInfo() const {
    uint64_t vtxCount = 0;
    for ( const auto& geom : m_geometryData )
    {
        vtxCount += geom->getVerticesSize();
    }
    LOG(logINFO) << "======== LOADING SUMMARY ========";
    LOG(logINFO) << "Mesh loaded        : " << m_geometryData.size();
    LOG(logINFO) << "Total vertex count : " << vtxCount;
    LOG(logINFO) << "Handle loaded      : " << m_handleData.size();
    LOG(logINFO) << "Animation loaded   : " << m_animationData.size();
    LOG(logINFO) << "Loading Time (sec) : " << m_loadingTime;
}


} // namespace Asset
} // namespace Ra

