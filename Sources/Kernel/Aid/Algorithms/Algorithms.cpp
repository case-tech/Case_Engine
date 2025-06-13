#include "Algorithms.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace Algorithms
        {
            // Interpolator
            template <typename T>
            T Interpolator<T>::Lerp(const T& a_a, const T& b_a, float t_a)
            {
                return a_a * (1 - t_a) + b_a * t_a;
            }

            template <typename T>
            T Interpolator<T>::EaseInOut(const T& a_a, const T& b_a, float t_a)
            {
                t_a = t_a * t_a * (3 - 2 * t_a);
                return Lerp(a_a, b_a, t_a);
            }

            template <typename T>
            T Interpolator<T>::EaseIn(const T& a_a, const T& b_a, float t_a)
            {
                t_a = t_a * t_a;
                return Lerp(a_a, b_a, t_a);
            }

            template <typename T>
            T Interpolator<T>::EaseOut(const T& a_a, const T& b_a, float t_a)
            {
                t_a = 1 - (1 - t_a) * (1 - t_a);
                return Lerp(a_a, b_a, t_a);
            }

            template <typename T>
            T Interpolator<T>::Cubic(const T& a_a, const T& b_a, float t_a)
            {
                t_a = t_a * t_a * t_a;
                return Lerp(a_a, b_a, t_a);
            }

            template <typename T>
            T Interpolator<T>::SmoothStep(const T& a_a, const T& b_a, float t_a)
            {
                t_a = t_a * t_a * (3 - 2 * t_a);
                return Lerp(a_a, b_a, t_a);
            }

            template <typename T>
            T Interpolator<T>::Exponential(const T& a_a, const T& b_a, float t_a)
            {
                t_a = t_a < 0.5f 
					? 2 * t_a * t_a 
					: 1 - static_cast<float>(std::pow(-2 * t_a + 2, 2)) / 2;
                return Lerp(a_a, b_a, t_a);
            }

            // FSM
            template <typename T>
            void StateMachine<T>::AddState(T state_a, std::function<void()> on_update_a)
            {
                states_[state_a] = on_update_a;
            }

            template <typename T>
            void StateMachine<T>::AddTransition(T from_a, T to_a, std::function<bool()> condition_a)
            {
                transitions_[from_a][to_a] = condition_a;
            }

            template <typename T>
            void StateMachine<T>::Update()
            {
                if (states_.count(current_state_)) {
                    states_[current_state_]();
                }
            }

            template <typename T>
            bool StateMachine<T>::TryTransition(T new_state_a, bool save_history_a)
            {
                if (transitions_[current_state_].count(new_state_a) && 
                    transitions_[current_state_][new_state_a]()) 
                {
                    if (save_history_a) 
                    {
                        state_history_.push(current_state_);
                    }
                    current_state_ = new_state_a;
                    return true;
                }
                return false;
            }

            template <typename T>
            bool StateMachine<T>::RevertToPreviousState()
            {
                if (!state_history_.empty()) 
                {
                    current_state_ = state_history_.top();
                    state_history_.pop();
                    return true;
                }
                return false;
            }

            template <typename T>
            void StateMachine<T>::ClearHistory()
            {
                while (!state_history_.empty()) 
                {
                    state_history_.pop();
                }
            }

            template <typename T>
            T StateMachine<T>::GetCurrentState() const
            {
                return current_state_;
            }

            // SAzplexNoise
            float SAzplexNoise::Generate(float x_a, float y_a, float scale_a, 
                    int octaves_a, float persistence_a)
            {
                float total_ = 0.0f;
                float frequency_ = scale_a;
                float amplitude_ = 1.0f;
                float max_value_ = 0.0f;

                for (int i_ = 0; i_ < octaves_a; ++i_) 
                {
                    total_ += Noise(x_a * frequency_, y_a * frequency_) * amplitude_;
                    max_value_ += amplitude_;
                    amplitude_ *= persistence_a;
                    frequency_ *= 2.0f;
                }

                return total_ / max_value_;
            }

            float SAzplexNoise::Noise(float x_a, float y_a)
            {
                const float f2_ = 0.5f * (std::sqrt(3.0f) - 1.0f);
                const float g2_ = (3.0f - std::sqrt(3.0f)) / 6.0f;

                float s_ = (x_a + y_a) * f2_;
                int i_ = FastFloor(x_a + s_);
                int j_ = FastFloor(y_a + s_);
                float t_ = (i_ + j_) * g2_;

                float x0_ = i_ - t_;
                float y0_ = j_ - t_;
                float x0_r_ = x_a - x0_;
                float y0_r_ = y_a - y0_;

                int i1_, j1_;
                if (x0_r_ > y0_r_) 
                {
                    i1_ = 1;
                    j1_ = 0;
                } 
                else 
                {
                    i1_ = 0;
                    j1_ = 1;
                }

                float x1_ = x0_ - i1_ + g2_;
                float y1_ = y0_ - j1_ + g2_;
                float x2_ = x0_ - 1.0f + 2.0f * g2_;
                float y2_ = y0_ - 1.0f + 2.0f * g2_;

                int ii_ = i_ & 255;
                int jj_ = j_ & 255;
                int gi0_ = perm_[ii_ + perm_[jj_]] % 8;
                int gi1_ = perm_[ii_ + i1_ + perm_[jj_ + j1_]] % 8;
                int gi2_ = perm_[ii_ + 1 + perm_[jj_ + 1]] % 8;

                float n0_ = CalculateContribution(grad2_[gi0_], x0_, y0_);
                float n1_ = CalculateContribution(grad2_[gi1_], x1_, y1_);
                float n2_ = CalculateContribution(grad2_[gi2_], x2_, y2_);

                return 70.0f * (n0_ + n1_ + n2_);
            }

            float SAzplexNoise::CalculateContribution(const std::array<float, 2>& grad_a, 
                    float x_a, float y_a)
            {
                float t_ = 0.5f - x_a * x_a - y_a * y_a;
                if (t_ < 0.0f) return 0.0f;
                t_ *= t_;
                return t_ * t_ * (grad_a[0] * x_a + grad_a[1] * y_a);
            }

            int SAzplexNoise::FastFloor(float x_a)
            {
                return (x_a >= 0) ? static_cast<int>(x_a) : static_cast<int>(x_a) - 1;
            }


            const std::array<int, 512> SAzplexNoise::perm_ = 
            {
                {
                 151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 
                 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 
                 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 
                 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 
                 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105,
                 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 
                 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 
                 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 
                 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 
                 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 
                 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178,
                 185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 
                 162, 241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 
                 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93,
                 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
                }
            };

            const std::array<std::array<float, 2>, 8> SAzplexNoise::grad2_ = 
            {
                        {{1.0f, 1.0f}, {-1.0f, 1.0f}, {1.0f, -1.0f}, {-1.0f, -1.0f},
                        {1.0f, 0.0f}, {-1.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, -1.0f}}
            };

            // AStar
            AStar::Node::Node(int x_a, int y_a, Node* parent_a)
            : x_(x_a), y_(y_a), g_(0), h_(0), f_(0), parent_(parent_a) {}

            void AStar::Node::CalculateH(int end_x_a, int end_y_a)
            {
                h_ = std::abs(x_ - end_x_a) + std::abs(y_ - end_y_a);
            }

            void AStar::Node::CalculateF()
            {
                f_ = g_ + h_;
            }

            bool AStar::CompareNode::operator()(const Node* a_a, const Node* b_a)
            {
                return a_a->f_ > b_a->f_;
            }

            std::vector<std::pair<int, int>> AStar::AStarSearch(std::vector<std::vector<int>> grid_a,
                                                                std::pair<int, int> start_a,
                                                                std::pair<int, int> end_a)
            {
                std::vector<std::pair<int, int>> path_;

                if (grid_a.empty() || grid_a[0].empty()) return path_;

                int rows_ = static_cast<int>(grid_a.size());
				int cols_ = static_cast<int>(grid_a[0].size());

                if (start_a.first < 0 || start_a.first >= rows_ ||
                    start_a.second < 0 || start_a.second >= cols_ ||
                    end_a.first < 0 || end_a.first >= rows_ ||
                    end_a.second < 0 || end_a.second >= cols_ ||
                    grid_a[start_a.first][start_a.second] == 1 ||
                    grid_a[end_a.first][end_a.second] == 1)
                {
                    return path_;
                }

                std::priority_queue<Node*, std::vector<Node*>, CompareNode> open_list_;
                std::vector<std::vector<bool>> closed_list_(rows_, std::vector<bool>(cols_, false));
                std::vector<std::vector<int>> g_values_(rows_, std::vector<int>(cols_, INT_MAX));

                Node* start_node_ = new Node(start_a.first, start_a.second);
                start_node_->CalculateH(end_a.first, end_a.second);
                start_node_->CalculateF();
                open_list_.push(start_node_);
                g_values_[start_a.first][start_a.second] = 0;

                std::vector<std::pair<int, int>> directions_ = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

                while (!open_list_.empty())
                {
                    Node* current_ = open_list_.top();
                    open_list_.pop();

                    if (current_->x_ == end_a.first && current_->y_ == end_a.second)
                    {
                        Node* temp_ = current_;
                        while (temp_ != nullptr)
                        {
                            path_.emplace_back(temp_->x_, temp_->y_);
                            temp_ = temp_->parent_;
                        }
                        std::reverse(path_.begin(), path_.end());
                        delete current_;
                        while (!open_list_.empty())
                        {
                            delete open_list_.top();
                            open_list_.pop();
                        }
                        return path_;
                    }

                    if (closed_list_[current_->x_][current_->y_])
                    {
                        delete current_;
                        continue;
                    }

                    closed_list_[current_->x_][current_->y_] = true;

                    for (auto& dir_ : directions_)
                    {
                        int new_x_ = current_->x_ + dir_.first;
                        int new_y_ = current_->y_ + dir_.second;

                        if (new_x_ < 0 || new_x_ >= rows_ || new_y_ < 0 ||
                            new_y_ >= cols_ || grid_a[new_x_][new_y_] == 1 ||
                            closed_list_[new_x_][new_y_])
                        {
                            continue;
                        }

                        int new_g_ = current_->g_ + 1;
                        if (new_g_ < g_values_[new_x_][new_y_])
                        {
                            Node* neighbor_ = new Node(new_x_, new_y_, current_);
                            neighbor_->g_ = new_g_;
                            neighbor_->CalculateH(end_a.first, end_a.second);
                            neighbor_->CalculateF();
                            open_list_.push(neighbor_);
                            g_values_[new_x_][new_y_] = new_g_;
                        }
                    }
                    delete current_;
                }
                return path_;
            }

            // Encryption
            Encryption::Encryption(CUtils::Alg algorithm_a, const std::string& key_a)
            : enc_(new CUtils::Encryption(algorithm_a, key_a)) {}

            std::string Encryption::Encrypt(const std::string& plain_text_a)
            {
                return enc_->Encrypt(plain_text_a);
            }

            std::string Encryption::Decrypt(const std::string& cipher_text_a)
            {
                return enc_->Decrypt(cipher_text_a);
            }

            // NoisePerlin
            NoisePerlin::NoisePerlin(unsigned int seed_a)
            : noise_perlin_(new CUtils::NoisePerlin(seed_a)) {}

            double NoisePerlin::Noise(double x_a, double y_a) const
            {
                return noise_perlin_->Noise(x_a, y_a);
            }

            template class Interpolator<float>;
            template class StateMachine<int>;
        } // namespace Algorithms
    } // namespace Aid
} // namespace CE_Kernel
