#include "PNodes.hpp"

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PNodes
        {
            std::shared_ptr<Node> NodeFactory::CreateConstant(double value_a)
            {
                return std::make_shared<ConstantNode>(value_a);
            }

            std::shared_ptr<Node> NodeFactory::CreateStringConstant(
                    const std::string& value_a)
            {
                return std::make_shared<StringConstantNode>(value_a);
            }

            std::shared_ptr<Node> NodeFactory::CreateVariable(double init_a)
            {
                return std::make_shared<VariableNode>(init_a);
            }

            std::shared_ptr<Node> NodeFactory::CreateStringVariable(
                    const std::string& init_a)
            {
                return std::make_shared<StringVariableNode>(init_a);
            }

            std::shared_ptr<Node> NodeFactory::CreateBinaryOp(
                    const std::string& op_type_a,
                    std::shared_ptr<Node> lhs_a,
                    std::shared_ptr<Node> rhs_a)
            {
                static const std::unordered_map<std::string,
                                                BinaryOpNode::OpFunc>
                        num_operations_ = {
                                {"+", [](double a_, double b_) { return a_ + b_; }},
                                {"-", [](double a_, double b_) { return a_ - b_; }},
                                {"*", [](double a_, double b_) { return a_ * b_; }},
                                {"/",
                                 [](double a_, double b_) {
                                     if (b_ == 0)
                                         throw std::runtime_error(
                                                 "Division by zero");
                                     return a_ / b_;
                                 }},
                                {"%",
                                 [](double a_, double b_) { return fmod(a_, b_); }},
                                {"^",
                                 [](double a_, double b_) { return pow(a_, b_); }},
                                {"max",
                                 [](double a_, double b_) {
                                     return a_ > b_ ? a_ : b_;
                                 }},
                                {"min",
                                 [](double a_, double b_) {
                                     return a_ < b_ ? a_ : b_;
                                 }},

                                {">",
                                 [](double a_, double b_) {
                                     return a_ > b_ ? 1.0 : 0.0;
                                 }},
                                {"<",
                                 [](double a_, double b_) {
                                     return a_ < b_ ? 1.0 : 0.0;
                                 }},
                                {">=",
                                 [](double a_, double b_) {
                                     return a_ >= b_ ? 1.0 : 0.0;
                                 }},
                                {"<=",
                                 [](double a_, double b_) {
                                     return a_ <= b_ ? 1.0 : 0.0;
                                 }},
                                {"==",
                                 [](double a_, double b_) {
                                     return a_ == b_ ? 1.0 : 0.0;
                                 }},
                                {"!=", [](double a_, double b_) {
                                     return a_ != b_ ? 1.0 : 0.0;
                                 }}};

                static const std::unordered_map<std::string,
                                                BinaryOpNode::StrOpFunc>
                        str_operations_ = {
                                {"+",
                                 [](const std::string& a_,
                                    const std::string& b_) { return a_ + b_; }},
                                {"==",
                                 [](const std::string& a_,
                                    const std::string& b_) {
                                     return a_ == b_ ? "true" : "false";
                                 }},
                                {"!=",
                                 [](const std::string& a_,
                                    const std::string& b_) {
                                     return a_ != b_ ? "true" : "false";
                                 }},
                                {"concat",
                                 [](const std::string& a_,
                                    const std::string& b_) { return a_ + b_; }}};

                auto num_it_ = num_operations_.find(op_type_a);
                if (num_it_ != num_operations_.end())
                {
                    auto node_ = std::make_shared<BinaryOpNode>(num_it_->second,
                                                                op_type_a);
                    if (lhs_a)
                        node_->ConnectInput(0, lhs_a);
                    if (rhs_a)
                        node_->ConnectInput(1, rhs_a);
                    return node_;
                }

                auto str_it_ = str_operations_.find(op_type_a);
                if (str_it_ != str_operations_.end())
                {
                    auto node_ = std::make_shared<BinaryOpNode>(str_it_->second,
                                                                op_type_a);
                    if (lhs_a)
                        node_->ConnectInput(0, lhs_a);
                    if (rhs_a)
                        node_->ConnectInput(1, rhs_a);
                    return node_;
                }

                throw std::runtime_error("Unknown binary operation: " + op_type_a);
            }

            std::shared_ptr<Node> NodeFactory::CreateUnaryOp(
                    const std::string& op_type_a,
                    std::shared_ptr<Node> input_a)
            {
                static const std::unordered_map<std::string,
                                                UnaryOpNode::OpFunc>
                        num_operations_ = {
                                {"abs", [](double a_) { return abs(a_); }},
                                {"floor", [](double a_) { return floor(a_); }},
                                {"ceil", [](double a_) { return ceil(a_); }},
                                {"round", [](double a_) { return round(a_); }},
                                {"sin", [](double a_) { return sin(a_); }},
                                {"cos", [](double a_) { return cos(a_); }},
                                {"tan", [](double a_) { return tan(a_); }},
                                {"log", [](double a_) { return log(a_); }},
                                {"log10", [](double a_) { return log10(a_); }},
                                {"exp", [](double a_) { return exp(a_); }},
                                {"sqrt", [](double a_) { return sqrt(a_); }},
                                {"neg", [](double a_) { return -a_; }},
                                {"!", [](double a_) {
                                     return a_ == 0.0 ? 1.0 : 0.0;
                                 }}};

                static const std::unordered_map<std::string,
                                                UnaryOpNode::StrOpFunc>
                        str_operations_ = {{"upper",
                                          [](const std::string& a_) {
                                              std::string result_ = a_;
                                              std::transform(result_.begin(),
                                                             result_.end(),
                                                             result_.begin(),
                                                             ::toupper);
                                              return result_;
                                          }},
                                         {"lower",
                                          [](const std::string& a_) {
                                              std::string result_ = a_;
                                              std::transform(result_.begin(),
                                                             result_.end(),
                                                             result_.begin(),
                                                             ::tolower);
                                              return result_;
                                          }},
                                         {"reverse",
                                          [](const std::string& a_) {
                                              return std::string(a_.rbegin(),
                                                                 a_.rend());
                                          }},
                                         {"length", [](const std::string& a_) {
                                              return std::to_string(a_.length());
                                          }}};

                auto num_it_ = num_operations_.find(op_type_a);
                if (num_it_ != num_operations_.end())
                {
                    auto node_ = std::make_shared<UnaryOpNode>(num_it_->second,
                                                               op_type_a);
                    if (input_a)
                        node_->ConnectInput(0, input_a);
                    return node_;
                }

                auto str_it_ = str_operations_.find(op_type_a);
                if (str_it_ != str_operations_.end())
                {
                    auto node_ = std::make_shared<UnaryOpNode>(str_it_->second,
                                                               op_type_a);
                    if (input_a)
                        node_->ConnectInput(0, input_a);
                    return node_;
                }

                throw std::runtime_error("Unknown unary operation: " + op_type_a);
            }

            std::shared_ptr<Node> NodeFactory::CreateConditional(
                    std::shared_ptr<Node> cond_a,
                    std::shared_ptr<Node> true_branch_a,
                    std::shared_ptr<Node> false_branch_a)
            {
                auto node_ = std::make_shared<ConditionalNode>();
                if (cond_a)
                    node_->ConnectInput(0, cond_a);
                if (true_branch_a)
                    node_->ConnectInput(1, true_branch_a);
                if (false_branch_a)
                    node_->ConnectInput(2, false_branch_a);
                return node_;
            }

            std::shared_ptr<Node> NodeFactory::CreateDelay(
                    std::shared_ptr<Node> input_a)
            {
                auto node_ = std::make_shared<DelayNode>();
                if (input_a)
                    node_->ConnectInput(0, input_a);
                return node_;
            }

            std::shared_ptr<Node> NodeFactory::CreateTAzer()
            {
                return std::make_shared<TimerNode>();
            }

            std::shared_ptr<Node> NodeFactory::CreateCounter(
                    std::shared_ptr<Node> increment_a)
            {
                auto node_ = std::make_shared<CounterNode>();
                if (increment_a)
                    node_->ConnectInput(0, increment_a);
                return node_;
            }

            std::shared_ptr<Node> NodeFactory::CreatePrinter(
                    std::shared_ptr<Node> input_a)
            {
                auto node_ = std::make_shared<PrintNode>();
                if (input_a)
                    node_->ConnectInput(0, input_a);
                return node_;
            }

            std::shared_ptr<Node> NodeFactory::CreateMathFunction(
                    const std::string& func_name_a,
                    std::shared_ptr<Node> input_a)
            {
                static const std::unordered_map<std::string,
                                                MathFunctionNode::MathFunc>
                        functions_ = {
                                {"sin", [](double a_) { return sin(a_); }},
                                {"cos", [](double a_) { return cos(a_); }},
                                {"tan", [](double a_) { return tan(a_); }},
                                {"asin", [](double a_) { return asin(a_); }},
                                {"acos", [](double a_) { return acos(a_); }},
                                {"atan", [](double a_) { return atan(a_); }},
                                {"exp", [](double a_) { return exp(a_); }},
                                {"log", [](double a_) { return log(a_); }},
                                {"log10", [](double a_) { return log10(a_); }},
                                {"sqrt", [](double a_) { return sqrt(a_); }},
                                {"abs", [](double a_) { return abs(a_); }},
                                {"floor", [](double a_) { return floor(a_); }},
                                {"ceil", [](double a_) { return ceil(a_); }},
                                {"round", [](double a_) { return round(a_); }}};

                auto it_ = functions_.find(func_name_a);
                if (it_ == functions_.end())
                    throw std::runtime_error("Unknown math function: "
                                             + func_name_a);

                auto node_ = std::make_shared<MathFunctionNode>(it_->second,
                                                                func_name_a);
                if (input_a)
                    node_->ConnectInput(0, input_a);
                return node_;
            }

            std::shared_ptr<Node> NodeFactory::CreateLogicalGate(
                    const std::string& gate_type_a,
                    std::shared_ptr<Node> lhs_a,
                    std::shared_ptr<Node> rhs_a)
            {
                static const std::unordered_map<std::string,
                                                LogicalGateNode::LogicFunc>
                        gates_ = {{"AND", [](bool a_, bool b_) { return a_ && b_; }},
                                 {"OR", [](bool a_, bool b_) { return a_ || b_; }},
                                 {"XOR", [](bool a_, bool b_) { return a_ != b_; }},
                                 {"NAND",
                                  [](bool a_, bool b_) { return !(a_ && b_); }},
                                 {"NOR",
                                  [](bool a_, bool b_) { return !(a_ || b_); }}};

                auto it_ = gates_.find(gate_type_a);
                if (it_ == gates_.end())
                    throw std::runtime_error("Unknown logic gate: " + gate_type_a);

                auto node_ = std::make_shared<LogicalGateNode>(it_->second,
                                                               gate_type_a);
                if (lhs_a)
                    node_->ConnectInput(0, lhs_a);
                if (rhs_a)
                    node_->ConnectInput(1, rhs_a);
                return node_;
            }

            std::shared_ptr<Node> NodeFactory::CreateForLoop(
                    std::shared_ptr<Node> start_a,
                    std::shared_ptr<Node> end_a,
                    std::shared_ptr<Node> step_a,
                    std::shared_ptr<Node> body_a)
            {
                auto node_ = std::make_shared<ForLoopNode>();
                if (start_a)
                    node_->ConnectInput(0, start_a);
                if (end_a)
                    node_->ConnectInput(1, end_a);
                if (step_a)
                    node_->ConnectInput(2, step_a);
                if (body_a)
                    node_->ConnectInput(3, body_a);
                return node_;
            }

            std::shared_ptr<Node> NodeFactory::CreateCustomFunction(
                    const std::function<double(const std::vector<double>&)>&
                            func_a)
            {
                return std::make_shared<CustomFunctionNode>(func_a);
            }
        } // namespace PNodes
    } // namespace Aid
} // namespace CE_Kernel