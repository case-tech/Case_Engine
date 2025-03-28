#include "Random.hpp"

namespace CE_Kernel
{
    namespace Aux
    {
        namespace Random
        {
            int Random::RandomInt(int min_a, int max_a)
            {
                return random_.RandomInt(min_a, max_a);
            }

            double Random::RandomDouble(double min_a, double max_a)
            {
                return random_.RandomDouble(min_a, max_a);
            }

            double Random::NormalDouble(double mean_a, double stddev_a)
            {
                return random_.NormalDouble(mean_a, stddev_a);
            }

            int Random::BinomialInt(int t_a, double p_a)
            {
                return random_.BinomialInt(t_a, p_a);
            }

            bool Random::BernoulliBool(double p_a)
            {
                return random_.BernoulliBool(p_a);
            }

            int Random::PoissonInt(double mean_a)
            {
                return random_.PoissonInt(mean_a);
            }
        }
    }
}
