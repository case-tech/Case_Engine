// Copyright (c) 2025 Case Technologies

#pragma once
#include <CUtils/Random.hpp>

namespace CE_Kernel
{
    namespace Aux
    {
        namespace Random
        {
            class Random
            {
            public:
                int RandomInt(int min_a, int max_a);
                double RandomDouble(double min_a, double max_a);
                double NormalDouble(double mean_a, double stddev_a);
                int BinomialInt(int t_a, double p_a);
                bool BernoulliBool(double p_a);
                int PoissonInt(double men_a);

            private:
                CUtils::Random random_;
            };
        }
    }
}
