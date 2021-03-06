#include "Model.h"
#include <YYZVis/Lib/Edge.h>
#include <YYZVis/Lib/YinYangGridSampling.h>
#include <YYZVis/Lib/ZhongGridSampling.h>
#include <kvs/ExternalFaces>
#include <kvs/SmartPointer>
#include <kvs/CellByCellMetropolisSampling>


namespace
{

typedef kvs::SharedPointer<kvs::UnstructuredVolumeObject> VolumePointer;

}

namespace local
{

Model::Model( const local::Input& input ):
    m_input( input )
{
    this->import_yin_volume();
    this->import_yang_volume();
    this->import_zhong_volume();
    this->update_min_max_values();
    this->update_min_max_coords();
}

kvs::LineObject* Model::newYinMeshes( const size_t dim_edge ) const
{
    return YYZVis::Edge::CreateLineMeshObject( &m_yin_volume, dim_edge );
}

kvs::LineObject* Model::newYangMeshes( const size_t dim_edge ) const
{
    return YYZVis::Edge::CreateLineMeshObject( &m_yang_volume, dim_edge );
}

kvs::LineObject* Model::newYinEdges() const
{
    return YYZVis::Edge::CreateLineEdgeObject( &m_yin_volume );
}

kvs::LineObject* Model::newYangEdges() const
{
    return YYZVis::Edge::CreateLineEdgeObject( &m_yang_volume );
}

kvs::LineObject* Model::newZhongEdges() const
{
    return YYZVis::Edge::CreateLineEdgeObject( &m_zhong_volume );
}

kvs::PolygonObject* Model::newYinFaces() const
{
    ::VolumePointer volume( YinVolume::ToUnstructuredVolumeObject( &m_yin_volume ) );
    return this->newFaces( volume.get() );
}

kvs::PolygonObject* Model::newYangFaces() const
{
    ::VolumePointer volume( YangVolume::ToUnstructuredVolumeObject( &m_yang_volume ) );
    return this->newFaces( volume.get() );
}

kvs::PolygonObject* Model::newZhongFaces() const
{
    ::VolumePointer volume( ZhongVolume::ToUnstructuredVolumeObject( &m_zhong_volume ) );
    return this->newFaces( volume.get() );
}

kvs::PolygonObject* Model::newFaces( const kvs::UnstructuredVolumeObject* volume ) const
{
    return new kvs::ExternalFaces( volume );
}

kvs::PointObject* Model::newYinParticles() const
{
    if ( m_input.previous_method )
    {
        ::VolumePointer volume( YinVolume::ToUnstructuredVolumeObject( &m_yin_volume ) );
        return this->newParticles( volume.get() );
    }
    return this->newYinParticles( &m_yin_volume );
}

kvs::PointObject* Model::newYangParticles() const
{
    if ( m_input.previous_method )
    {
        ::VolumePointer volume( YangVolume::ToUnstructuredVolumeObject( &m_yang_volume ) );
        return this->newParticles( volume.get() );
    }
    return this->newYangParticles( &m_yang_volume );
}

kvs::PointObject* Model::newZhongParticles() const
{
    if ( m_input.previous_method )
    {
        ::VolumePointer volume( ZhongVolume::ToUnstructuredVolumeObject( &m_zhong_volume ) );
        return this->newParticles( volume.get() );
    }
    return this->newZhongParticles( &m_zhong_volume );
}

kvs::PointObject* Model::newYinParticles( const YinVolume* volume ) const
{
    const size_t repeats = m_input.repeats;
    const size_t subpixels = 1; // fixed to '1'
    const size_t level = static_cast<size_t>( subpixels * std::sqrt( double( repeats ) ) );
    const float step = 0.1f;
    return new YYZVis::YinYangGridSampling( volume, level, step, m_input.tfunc );
}

kvs::PointObject* Model::newYangParticles( const YangVolume* volume ) const
{
    const size_t repeats = m_input.repeats;
    const size_t subpixels = 1; // fixed to '1'
    const size_t level = static_cast<size_t>( subpixels * std::sqrt( double( repeats ) ) );
    const float step = 0.1f;
    return new YYZVis::YinYangGridSampling( volume, level, step, m_input.tfunc );
}

kvs::PointObject* Model::newZhongParticles( const ZhongVolume* volume ) const
{
    const size_t repeats = m_input.repeats;
    const size_t subpixels = 1; // fixed to '1'
    const size_t level = static_cast<size_t>( subpixels * std::sqrt( double( repeats ) ) );
    const float step = 0.1f;
    return new YYZVis::ZhongGridSampling( volume, level, step, m_input.tfunc );
}

kvs::PointObject* Model::newParticles( const kvs::UnstructuredVolumeObject* volume ) const
{
    const size_t repeats = m_input.repeats;
    const size_t subpixels = 1; // fixed to '1'
    const size_t level = static_cast<size_t>( subpixels * std::sqrt( double( repeats ) ) );
    const float step = 0.1f;
    return new kvs::CellByCellMetropolisSampling( volume, level, step, m_input.tfunc );
}

void Model::import_yin_volume()
{
    const size_t dim_rad = m_input.dim_rad;
    const size_t dim_lat = m_input.dim_lat;
    const size_t dim_lon = m_input.dim_lon;
    const std::string& filename = m_input.filename_yin;

    m_yin_volume.setDimR( dim_rad );
    m_yin_volume.setDimTheta( dim_lat );
    m_yin_volume.setDimPhi( dim_lon );
    m_yin_volume.setVeclen( 1 );
    m_yin_volume.calculateCoords();
    m_yin_volume.readValues( filename );
    m_yin_volume.updateMinMaxCoords();
    m_yin_volume.updateMinMaxValues();
}

void Model::import_yang_volume()
{
    const size_t dim_rad = m_input.dim_rad;
    const size_t dim_lat = m_input.dim_lat;
    const size_t dim_lon = m_input.dim_lon;
    const std::string& filename = m_input.filename_yang;

    m_yang_volume.setDimR( dim_rad );
    m_yang_volume.setDimTheta( dim_lat );
    m_yang_volume.setDimPhi( dim_lon );
    m_yang_volume.setVeclen( 1 );
    m_yang_volume.calculateCoords();
    m_yang_volume.readValues( filename );
    m_yang_volume.updateMinMaxCoords();
    m_yang_volume.updateMinMaxValues();
}

void Model::import_zhong_volume()
{
    const size_t dim_rad = m_input.dim_rad;
    const size_t dim_zhong = m_input.dim_zhong;
    const std::string& filename = m_input.filename_zhong;

    m_zhong_volume.setDimR( dim_rad );
    m_zhong_volume.setDim( dim_zhong );
    m_zhong_volume.setVeclen( 1 );
    m_zhong_volume.calculateCoords();
    m_zhong_volume.readValues( filename );
    m_zhong_volume.updateMinMaxCoords();
    m_zhong_volume.updateMinMaxValues();
}

void Model::update_min_max_values()
{
    const kvs::Real32 min_value0 = m_yin_volume.minValue();
    const kvs::Real32 min_value1 = m_yang_volume.minValue();
    const kvs::Real32 min_value2 = m_zhong_volume.minValue();
    const kvs::Real32 max_value0 = m_yin_volume.maxValue();
    const kvs::Real32 max_value1 = m_yang_volume.maxValue();
    const kvs::Real32 max_value2 = m_zhong_volume.maxValue();
    const kvs::Real32 min_value = kvs::Math::Min( min_value0, min_value1, min_value2 );
    const kvs::Real32 max_value = kvs::Math::Max( max_value0, max_value1, max_value2 );
    m_yin_volume.setMinMaxValues( min_value, max_value );
    m_yang_volume.setMinMaxValues( min_value, max_value );
    m_zhong_volume.setMinMaxValues( min_value, max_value );
}

void Model::update_min_max_coords()
{
    const kvs::Vec3& min_coord0 = m_yin_volume.minObjectCoord();
    const kvs::Vec3& min_coord1 = m_yang_volume.minObjectCoord();
    const kvs::Vec3& min_coord2 = m_zhong_volume.minObjectCoord();
    const kvs::Vec3& max_coord0 = m_yin_volume.maxObjectCoord();
    const kvs::Vec3& max_coord1 = m_yang_volume.maxObjectCoord();
    const kvs::Vec3& max_coord2 = m_zhong_volume.maxObjectCoord();
    const kvs::Real32 min_x = kvs::Math::Min( min_coord0.x(), min_coord1.x(), min_coord2.x() );
    const kvs::Real32 min_y = kvs::Math::Min( min_coord0.y(), min_coord1.y(), min_coord2.y() );
    const kvs::Real32 min_z = kvs::Math::Min( min_coord0.z(), min_coord1.z(), min_coord2.z() );
    const kvs::Real32 max_x = kvs::Math::Min( max_coord0.x(), max_coord1.x(), max_coord2.x() );
    const kvs::Real32 max_y = kvs::Math::Min( max_coord0.y(), max_coord1.y(), max_coord2.y() );
    const kvs::Real32 max_z = kvs::Math::Min( max_coord0.z(), max_coord1.z(), max_coord2.z() );
    const kvs::Vec3 min_coord( min_x, min_y, min_z );
    const kvs::Vec3 max_coord( max_x, max_y, max_z );
    m_yin_volume.setMinMaxObjectCoords( min_coord, max_coord );
    m_yin_volume.setMinMaxExternalCoords( min_coord, max_coord );
    m_yang_volume.setMinMaxObjectCoords( min_coord, max_coord );
    m_yang_volume.setMinMaxExternalCoords( min_coord, max_coord );
    m_zhong_volume.setMinMaxObjectCoords( min_coord, max_coord );
    m_zhong_volume.setMinMaxExternalCoords( min_coord, max_coord );
}

} // end of namespace local
