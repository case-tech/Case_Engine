// Copyright (c) 2025 Case Technologies

#pragma once
#include <CUtils/Encryption.hpp>
#include <CUtils/NoisePerlin.hpp>
#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <functional>
#include <iostream>
#include <queue>
#include <stack>
#include <unordered_map>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace Algorithms
        {
            template <typename T>
            class Interpolator
            {
            public:
                static T Lerp(const T& a_a, const T& b_a, float t_a);
                static T EaseInOut(const T& a_a, const T& b_a, float t_a);
                static T EaseIn(const T& a_a, const T& b_a, float t_a); 
                static T EaseOut(const T& a_a, const T& b_a, float t_a); 
                static T Cubic(const T& a_a, const T& b_a, float t_a); 
                static T SmoothStep(const T& a_a, const T& b_a, float t_a); 
                static T Exponential(const T& a_a, const T& b_a, float t_a);
            };

            template <typename T>
            class StateMachine
            {
            public:
                StateMachine() = default;
                void AddState(T state_a, std::function<void()> on_update_a);
                void AddTransition(T from_a, T to_a, std::function<bool()> condition_a); 
                void Update(); 
                bool TryTransition(T new_state_a, bool save_history_a = false); 
                bool RevertToPreviousState();
                void ClearHistory();
                T GetCurrentState() const;

            private:
                T current_state_;
                std::stack<T> state_history_;
                                                                                                  
                std::unordered_map<T, std::function<void()>> states_;
                std::unordered_map<T, std::unordered_map<T, std::function<bool()>>> transitions_;
            };

            class SAzplexNoise
            {
            public:
                static float Generate(float x_a, float y_a, 
                                      float scale_a = 0.1f, int octaves_a = 4, 
                                      float persistence_a = 0.5f);

            private: 
                static const std::array<int, 512> perm_;
                static const std::array<std::array<float, 2>, 8> grad2_;

            private:
                static float Noise(float x_a, float y_a); 
                static float CalculateContribution(const std::array<float, 2>& grad_a, float x_a, float y_a); 
                static int FastFloor(float x_a);
            };

            class AStar
            {
            public:
                struct Node
                {
                    int x_, y_;
                    int g_, h_, f_;
                    Node* parent_;

                    Node(int x_a, int y_a, Node* parent_a = nullptr); 
                    void CalculateH(int end_x_a, int end_y_a); 
                    void CalculateF();
                };

                struct CompareNode
                {
                    bool operator()(const Node* a_a, const Node* b_a);
                };

                std::vector<std::pair<int, int>> AStarSearch(std::vector<std::vector<int>> grid_a,
                                                             std::pair<int, int> start_a,
                                                             std::pair<int, int> end_a);
            };

            class Encryption
            {
            public:
                Encryption(CUtils::Alg algorithm_a, const std::string& key_a = "");

                std::string Encrypt(const std::string& plain_text_a);
                std::string Decrypt(const std::string& cipher_text_a);

            private:
                CUtils::Encryption* enc_;
            };

            class NoisePerlin
            {
            public:
                NoisePerlin(unsigned int seed_a = 0);
                double Noise(double x_a, double y_a) const;
            
            private:
                CUtils::NoisePerlin* noise_perlin_; 
            };

            extern template class Interpolator<float>;
            extern template class StateMachine<int>;
        } // namespace Algorithms
    } // namespace Aid
} // namespace CE_Kernel
