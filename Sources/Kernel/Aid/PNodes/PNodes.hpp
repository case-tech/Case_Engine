// Copyright (c) 2025 Case Technologies

#pragma once
#include <algorithm>
#include <atomic>
#include <cctype>
#include <chrono>
#include <cmath>
#include <ctime>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace PNodes
        {
            enum class NodeType
            {
                CONSTANT,
                VARIABLE,
                BINARY_OP,
                UNARY_OP,
                CONDITIONAL,
                FUNCTION_CALL,
                DELAY,
                TAzER,
                COUNTER,
                ARRAY,
                LOOP,
                EVENT,
                STRING,
                PRINT,
                MATH_FUNCTION,
                LOGICAL_GATE,
                CUSTOM_FUNCTION
            };

            enum class ValueType
            {
                NUMBER,
                STRING,
                BOOLEAN,
                ARRAY
            };

            class Node
            {
            public:
                virtual ~Node() = default;
                virtual double Exec() = 0;
                virtual std::string ExecStr()
                {
                    return std::to_string(Exec());
                }

                virtual bool ExecBool()
                {
                    return Exec() != 0.0;
                }
                virtual NodeType GetType() const = 0;
                virtual ValueType GetValueType() const
                {
                    return ValueType::NUMBER;
                }

                virtual void SetValue(double)
                {
                    throw std::runtime_error("Node is not settable");
                }

                virtual void SetString(const std::string&)
                {
                    throw std::runtime_error("Node is not string settable");
                }

                virtual void ConnectInput(size_t index_a,
                                          std::shared_ptr<Node> node_a)
                {
                    if (index_a >= inputs_.size())
                        inputs_.resize(index_a + 1);
                    inputs_[index_a] = node_a;
                }

                virtual std::shared_ptr<Node> GetInput(size_t index_a) const
                {
                    if (index_a >= inputs_.size())
                        throw std::runtime_error("Invalid input index");
                    return inputs_[index_a];
                }

                virtual size_t InputCount() const
                {
                    return inputs_.size();
                }

                virtual void Reset()
                {}

            protected:
                std::vector<std::shared_ptr<Node>> inputs_;
            };

            class ConstantNode : public Node
            {
            public:
                explicit ConstantNode(double value_a) : value_(value_a)
                {}

                double Exec() override
                {
                    return value_;
                }
                
                NodeType GetType() const override
                {
                    return NodeType::CONSTANT;
                }

            private:
                double value_;
            };

            class StringConstantNode : public Node
            {
            public:
                explicit StringConstantNode(const std::string& value_a)
                    : value_(value_a)
                {}

                double Exec() override
                {
                    return 0.0;
                }
                std::string ExecStr() override
                {
                    return value_;
                }

                NodeType GetType() const override
                {
                    return NodeType::STRING;
                }

                ValueType GetValueType() const override
                {
                    return ValueType::STRING;
                }

            private:
                std::string value_;
            };

            class VariableNode : public Node
            {
            public:
                explicit VariableNode(double init_a = 0.0) : value_(init_a)
                {}

                double Exec() override
                {
                    return value_;
                }

                void SetValue(double v_a) override
                {
                    value_ = v_a;
                }
                NodeType GetType() const override
                {
                    return NodeType::VARIABLE;
                }

            private:
                double value_;
            };

            class StringVariableNode : public Node
            {
            public:
                explicit StringVariableNode(const std::string& init_a = "")
                    : value_(init_a)
                {}

                double Exec() override
                {
                    return 0.0;
                }

                std::string ExecStr() override
                {
                    return value_;
                }

                void SetString(const std::string& v_a) override
                {
                    value_ = v_a;
                }

                NodeType GetType() const override
                {
                    return NodeType::STRING;
                }

                ValueType GetValueType() const override
                {
                    return ValueType::STRING;
                }

            private:
                std::string value_;
            };

            class BinaryOpNode : public Node
            {
            public:
                using OpFunc = std::function<double(double, double)>;
                using StrOpFunc = std::function<
                        std::string(const std::string&, const std::string&)>;

                BinaryOpNode(OpFunc op_a,
                             const std::string& symbol_a,
                             ValueType type_a = ValueType::NUMBER)
                    : op_(std::move(op_a)), str_op_(nullptr), symbol_(symbol_a),
                      value_type_(type_a)
                {
                    inputs_.resize(2);
                }

                BinaryOpNode(StrOpFunc op_a, const std::string& symbol_a)
                    : op_(nullptr), str_op_(std::move(op_a)), symbol_(symbol_a),
                      value_type_(ValueType::STRING)
                {
                    inputs_.resize(2);
                }

                double Exec() override
                {
                    if (!inputs_[0] || !inputs_[1])
                        throw std::runtime_error(
                                "Missing inputs for binary op");
                    if (value_type_ != ValueType::NUMBER)
                        throw std::runtime_error(
                                "Binary op not supported for this type");
                    return op_(inputs_[0]->Exec(), inputs_[1]->Exec());
                }

                std::string ExecStr() override
                {
                    if (!inputs_[0] || !inputs_[1])
                        throw std::runtime_error(
                                "Missing inputs for binary op");
                    if (value_type_ != ValueType::STRING)
                        throw std::runtime_error(
                                "Binary op not supported for this type");
                    return str_op_(inputs_[0]->ExecStr(), inputs_[1]->ExecStr());
                }

                NodeType GetType() const override
                {
                    return NodeType::BINARY_OP;
                }

                ValueType GetValueType() const override
                {
                    return value_type_;
                }

                std::string GetSymbol() const
                {
                    return symbol_;
                }

            private:
                OpFunc op_;
                StrOpFunc str_op_;
                std::string symbol_;
                ValueType value_type_;
            };

            class UnaryOpNode : public Node
            {
            public:
                using OpFunc = std::function<double(double)>;
                using StrOpFunc =
                        std::function<std::string(const std::string&)>;

                UnaryOpNode(OpFunc op_a, const std::string& symbol_a)
                    : op_(std::move(op_a)), str_op_(nullptr), symbol_(symbol_a)
                {
                    inputs_.resize(1);
                }

                UnaryOpNode(StrOpFunc op_a, const std::string& symbol_a)
                    : op_(nullptr), str_op_(std::move(op_a)), symbol_(symbol_a)
                {
                    inputs_.resize(1);
                }

                double Exec() override
                {
                    if (!inputs_[0])
                        throw std::runtime_error("Missing input for unary op");
                    if (str_op_)
                        return 0.0;
                    return op_(inputs_[0]->Exec());
                }

                std::string ExecStr() override
                {
                    if (!inputs_[0])
                        throw std::runtime_error("Missing input for unary op");
                    if (op_)
                        return "";
                    return str_op_(inputs_[0]->ExecStr());
                }

                NodeType GetType() const override
                {
                    return NodeType::UNARY_OP;
                }

                std::string GetSymbol() const
                {
                    return symbol_;
                }

            private:
                OpFunc op_;
                StrOpFunc str_op_;
                std::string symbol_;
            };

            class ConditionalNode : public Node
            {
            public:
                ConditionalNode()
                {
                    inputs_.resize(3);
                }

                double Exec() override
                {
                    if (!inputs_[0] || !inputs_[1] || !inputs_[2])
                        throw std::runtime_error(
                                "Missing inputs for conditional");
                    return inputs_[0]->ExecBool() ? inputs_[1]->Exec()
                                                  : inputs_[2]->Exec();
                }

                std::string ExecStr() override
                {
                    if (!inputs_[0] || !inputs_[1] || !inputs_[2])
                        throw std::runtime_error(
                                "Missing inputs for conditional");
                    return inputs_[0]->ExecBool() ? inputs_[1]->ExecStr()
                                                  : inputs_[2]->ExecStr();
                }

                NodeType GetType() const override
                {
                    return NodeType::CONDITIONAL;
                }
            };

            class DelayNode : public Node
            {
            public:
                DelayNode() : last_value_(0.0)
                {}

                double Exec() override
                {
                    if (!inputs_.empty() && inputs_[0])
                    {
                        last_value_ = inputs_[0]->Exec();
                    }
                    return last_value_;
                }

                void Reset() override
                {
                    last_value_ = 0.0;
                }

                NodeType GetType() const override
                {
                    return NodeType::DELAY;
                }

            private:
                double last_value_;
            };

            class TimerNode : public Node
            {
            public:
                TimerNode() : start_tAze_(std::chrono::steady_clock::now())
                {}

                double Exec() override
                {
                    auto now_ = std::chrono::steady_clock::now();
                    auto duration_ = std::chrono::duration_cast<
                            std::chrono::milliseconds>(now_ - start_tAze_);
                    return duration_.count() / 1000.0;
                }

                void Reset() override
                {
                    start_tAze_ = std::chrono::steady_clock::now();
                }

                NodeType GetType() const override
                {
                    return NodeType::TAzER;
                }

            private:
                std::chrono::steady_clock::time_point start_tAze_;
            };

            class CounterNode : public Node
            {
            public:
                CounterNode() : count_(0)
                {}

                double Exec() override
                {
                    if (!inputs_.empty() && inputs_[0] && inputs_[0]->ExecBool())
                    {
                        count_++;
                    }
                    return count_;
                }

                void Reset() override
                {
                    count_ = 0;
                }

                NodeType GetType() const override
                {
                    return NodeType::COUNTER;
                }

            private:
                int count_;
            };

            class PrintNode : public Node
            {
            public:
                PrintNode()
                {
                    inputs_.resize(1);
                }

                double Exec() override
                {
                    if (!inputs_[0])
                        return 0.0;

                    switch (inputs_[0]->GetValueType())
                    {
                    case ValueType::NUMBER:
                        std::cout << inputs_[0]->Exec();
                        break;
                    case ValueType::STRING:
                        std::cout << inputs_[0]->ExecStr();
                        break;
                    case ValueType::BOOLEAN:
                        std::cout << (inputs_[0]->ExecBool() ? "true" : "false");
                        break;
                    default:
                        std::cout << "Unprintable value";
                    }
                    std::cout << std::endl;
                    return 0.0;
                }

                NodeType GetType() const override
                {
                    return NodeType::PRINT;
                }
            };

            class MathFunctionNode : public Node
            {
            public:
                using MathFunc = std::function<double(double)>;

                MathFunctionNode(MathFunc func_a, const std::string& name_a)
                    : func_(std::move(func_a)), name_(name_a)
                {
                    inputs_.resize(1);
                }

                double Exec() override
                {
                    if (!inputs_[0])
                        throw std::runtime_error(
                                "Missing input for math function");
                    return func_(inputs_[0]->Exec());
                }

                NodeType GetType() const override
                {
                    return NodeType::MATH_FUNCTION;
                }

                std::string GetName() const
                {
                    return name_;
                }

            private:
                MathFunc func_;
                std::string name_;
            };

            class LogicalGateNode : public Node
            {
            public:
                using LogicFunc = std::function<bool(bool, bool)>;

                LogicalGateNode(LogicFunc func_a, const std::string& name_a)
                    : func_(std::move(func_a)), name_(name_a)
                {
                    inputs_.resize(2);
                }

                double Exec() override
                {
                    if (!inputs_[0] || !inputs_[1])
                        throw std::runtime_error(
                                "Missing inputs for logic gate");
                    return func_(inputs_[0]->ExecBool(), inputs_[1]->ExecBool())
                                   ? 1.0
                                   : 0.0;
                }

                NodeType GetType() const override
                {
                    return NodeType::LOGICAL_GATE;
                }

                std::string GetName() const
                {
                    return name_;
                }

            private:
                LogicFunc func_;
                std::string name_;
            };

            class ForLoopNode : public Node
            {
            public:
                ForLoopNode()
                {
                    inputs_.resize(4);
                }

                double Exec() override
                {
                    if (!inputs_[0] || !inputs_[1] || !inputs_[2] || !inputs_[3])
                        throw std::runtime_error("Missing inputs for for-loop");

                    double start_ = inputs_[0]->Exec();
                    double end_ = inputs_[1]->Exec();
                    double step_ = inputs_[2]->Exec();
                    double result_ = 0.0;

                    for (double i_ = start_; i_ < end_; i_ += step_)
                    {
                        auto index_var_ = std::make_shared<VariableNode>(i_);
                        inputs_[3]->ConnectInput(0, index_var_);
                        result_ = inputs_[3]->Exec();
                    }
                    return result_;
                }

                NodeType GetType() const override
                {
                    return NodeType::LOOP;
                }
            };

            class CustomFunctionNode : public Node
            {
            public:
                CustomFunctionNode(
                        std::function<double(const std::vector<double>&)> func_a)
                    : func_(std::move(func_a))
                {}

                double Exec() override
                {
                    std::vector<double> args_;
                    for (size_t i_ = 0; i_ < inputs_.size(); ++i_)
                    {
                        if (inputs_[i_])
                        {
                            args_.push_back(inputs_[i_]->Exec());
                        }
                    }
                    return func_(args_);
                }

                NodeType GetType() const override
                {
                    return NodeType::CUSTOM_FUNCTION;
                }

            private:
                std::function<double(const std::vector<double>&)> func_;
            };

            class NodeFactory
            {
            public:
                static std::shared_ptr<Node> CreateConstant(double value_a);
                static std::shared_ptr<Node> CreateStringConstant(
                        const std::string& value_a);
              
                static std::shared_ptr<Node> CreateVariable(double init_a = 0.0);
                static std::shared_ptr<Node> CreateStringVariable(
                        const std::string& init_a = "");
                
                static std::shared_ptr<Node> CreateBinaryOp(
                        const std::string& op_type_a,
                        std::shared_ptr<Node> lhs_a = nullptr,
                        std::shared_ptr<Node> rhs_a = nullptr);
                
                static std::shared_ptr<Node> CreateUnaryOp(
                        const std::string& op_type_a,
                        std::shared_ptr<Node> input_a = nullptr);
                
                static std::shared_ptr<Node> CreateConditional(
                        std::shared_ptr<Node> cond_a = nullptr,
                        std::shared_ptr<Node> true_branch_a = nullptr,
                        std::shared_ptr<Node> false_branch_a = nullptr);
                
                static std::shared_ptr<Node> CreateDelay(
                        std::shared_ptr<Node> input_a = nullptr);
                
                static std::shared_ptr<Node> CreateTAzer();
                static std::shared_ptr<Node> CreateCounter(
                        std::shared_ptr<Node> increment_a = nullptr);
                
                static std::shared_ptr<Node> CreatePrinter(
                        std::shared_ptr<Node> input_a = nullptr);
                
                static std::shared_ptr<Node> CreateMathFunction(
                        const std::string& func_name_a,
                        std::shared_ptr<Node> input_a = nullptr);
                
                static std::shared_ptr<Node> CreateLogicalGate(
                        const std::string& gate_type_a,
                        std::shared_ptr<Node> lhs_a = nullptr,
                        std::shared_ptr<Node> rhs_a = nullptr);
                
                static std::shared_ptr<Node> CreateForLoop(
                        std::shared_ptr<Node> start_a = nullptr,
                        std::shared_ptr<Node> end_a = nullptr,
                        std::shared_ptr<Node> step_a = nullptr,
                        std::shared_ptr<Node> body_a = nullptr);
                
                static std::shared_ptr<Node> CreateCustomFunction(
                        const std::function<double(const std::vector<double>&)>&
                                func_a);
            };

            class NodeGraph
            {
            public:
                void AddNode(const std::string& name_a,
                             std::shared_ptr<Node> node_a)
                {
                    nodes_[name_a] = node_a;
                }

                std::shared_ptr<Node> GetNode(const std::string& name_a) const
                {
                    auto it_ = nodes_.find(name_a);
                    return it_ != nodes_.end() ? it_->second : nullptr;
                }

                double Execute(const std::string& output_node_a = "output")
                {
                    auto node_ = GetNode(output_node_a);
                    if (!node_)
                        throw std::runtime_error("Output node not found: "
                                                 + output_node_a);
                    return node_->Exec();
                }

                std::string ExecuteStr(const std::string& output_node_a = "output")
                {
                    auto node_ = GetNode(output_node_a);
                    if (!node_)
                        throw std::runtime_error("Output node not found: "
                                                 + output_node_a);
                    return node_->ExecStr();
                }

                void SetVariable(const std::string& name_a, double value_a)
                {
                    auto node_ = std::dynamic_pointer_cast<VariableNode>(
                            GetNode(name_a));
                   
                    if (!node_)
                        throw std::runtime_error("Variable not found: " + name_a);
                    node_->SetValue(value_a);
                }

                void SetStringVariable(const std::string& name_a,
                                       const std::string& value_a)
                {
                    auto node_ = std::dynamic_pointer_cast<StringVariableNode>(
                            GetNode(name_a));
                    
                    if (!node_)
                        throw std::runtime_error("String variable not found: "
                                                 + name_a);
                    node_->SetString(value_a);
                }

                void ResetState()
                {
                    for (auto& pair_ : nodes_)
                    {
                        pair_.second->Reset();
                    }
                }

            private:
                std::unordered_map<std::string, std::shared_ptr<Node>> nodes_;
            };
        } // namespace PNodes
    } // namespace Aid
} // namespace CE_Kernel