// Copyright (c) 2025 Case Technologies

#pragma once
#include <FMOD/fmod.h>
#include <vector>

namespace CE_Kernel
{
    namespace Audio
    {
        namespace Geometry
        {
            struct Vector3D
            {
                float x_, y_, z_;
            };

            class Geometry
            {
            public:
                Geometry(FMOD_GEOMETRY* geom_a);
		~Geometry();

		void SetPosition(const Vector3D& position_a);
		void AddPolygon(
			float direct_occlusion_a, 
			float reverb_occlusion_a,
		       	const std::vector<Vector3D>& vertices_a);

	    private:
		FMOD_GEOMETRY* geometry_ = nullptr;
            };
        } // namespace Geometry
    }     // namespace Audio
} // namespace CE_Kernel
