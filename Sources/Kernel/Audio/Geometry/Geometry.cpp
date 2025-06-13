#include "Geometry.hpp"

namespace CE_Kernel
{
    namespace Audio
    {
        namespace Geometry
        {
            Geometry::Geometry(FMOD_GEOMETRY* geom_a) : geometry_(geom_a) {};

            Geometry::~Geometry()
            {
                if (geometry_)
                    FMOD_Geometry_Release(geometry_);
            }

            void Geometry::SetPosition(const Vector3D& position_a)
            {
                if (geometry_)
                {
                    FMOD_VECTOR pos_ {position_a.x_,
                                      position_a.y_,
                                      position_a.z_};

                    FMOD_Geometry_SetPosition(geometry_, &pos_);
                }
            }

            void Geometry::AddPolygon(float direct_occlusion_a,
                                      float reverb_occlusion_a,
                                      const std::vector<Vector3D>& vertices_a)
            {
                if (geometry_)
                {
                    std::vector<FMOD_VECTOR> fmod_verts_;
                    for (auto& v : vertices_a)
                    {
                        fmod_verts_.push_back({v.x_, v.y_, v.z_});
                    }

                    FMOD_Geometry_AddPolygon(geometry_,
                                             direct_occlusion_a,
                                             reverb_occlusion_a,
                                             0,
                                             static_cast<int>(
                                                     fmod_verts_.size()),
                                             fmod_verts_.data(),
                                             nullptr);
                }
            }

        } // namespace Geometry
    } // namespace Audio
} // namespace CE_Kernel
