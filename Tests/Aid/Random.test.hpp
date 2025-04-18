#pragma once
#include "Aid/Random/Random.hpp"
#include <CUtils/UnitTests.hpp>
#include <vector>
#include <cmath>

using namespace CUtils;

UTEST(RandomTest, RandomIntRange)
{
    CE_Kernel::Aid::Random::Random rng_;
    const int min_ = 5, max_ = 10;
    for(int i = 0; i < 1000; ++i) 
    {
        int val_ = rng_.RandomInt(min_, max_);
        ASSERT_GE(val_, min_);
        ASSERT_LE(val_, max_);
    }
}

UTEST(RandomTest, RandomDoubleRange)
{
    CE_Kernel::Aid::Random::Random rng_;
    const double min_ = 2.5, max_ = 5.5;
    for(int i = 0; i < 1000; ++i) 
    {
        double val_ = rng_.RandomDouble(min_, max_);
        ASSERT_GE(val_, min_);
        ASSERT_LT(val_, max_);
    }
}

UTEST(RandomTest, NormalDistributionStats)
{
    CE_Kernel::Aid::Random::Random rng_;
    const double mean_ = 10.0, stddev_ = 2.0;
    std::vector<double> samples_;
    const int n_ = 10000;
    
    for(int i = 0; i < n_; ++i) 
    {
        samples_.push_back(rng_.NormalDouble(mean_, stddev_));
    }
    
    double sum_ = 0.0;
    for(double v : samples_) sum_ += v;
    double sample_mean_ = sum_ / n_;
    ASSERT_NEAR(sample_mean_, mean_, 0.2);
    
    double variance_ = 0.0;
    for(double v : samples_) 
        variance_ += (v - sample_mean_)*(v - sample_mean_);
    variance_ /= n_;
    ASSERT_NEAR(std::sqrt(variance_), stddev_, 0.3);
}

UTEST(RandomTest, BinomialBounds)
{
    CE_Kernel::Aid::Random::Random rng_;
    const int t_ = 10;
    const double p_ = 0.7;
    for(int i = 0; i < 1000; ++i) 
    {
        int val_ = rng_.BinomialInt(t_, p_);
        ASSERT_GE(val_, 0);
        ASSERT_LE(val_, t_);
    }
}

UTEST(RandomTest, BernoulliEdgeCases)
{
    CE_Kernel::Aid::Random::Random rng_;
    for(int i = 0; i < 100; ++i) 
    {
        ASSERT_TRUE(rng_.BernoulliBool(1.0));
    }
    
    for(int i = 0; i < 100; ++i) 
    {
        ASSERT_FALSE(rng_.BernoulliBool(0.0));
    }
}

UTEST(RandomTest, PoissonNonNegative)
{
    CE_Kernel::Aid::Random::Random rng_;
    const double mean_ = 4.5;
    for(int i = 0; i < 1000; ++i) 
    {
        int val_ = rng_.PoissonInt(mean_);
        ASSERT_GE(val_, 0);
    }
}

