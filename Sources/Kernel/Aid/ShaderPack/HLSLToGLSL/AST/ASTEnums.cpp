#include "ASTEnums.hpp"

#include "../Exception.hpp"
#include "../Report/ReportIdents.hpp"
#include "Token.hpp"

#include <algorithm>
#include <map>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            [[noreturn]]
            static void MapFailed(const std::string& from_a,
                                  const std::string& to_a)
            {
                throw std::invalid_argument(R_FailedToMap(from_a, to_a));
            }

            template <typename T>
            std::string TypeToString(const std::map<T, std::string>& type_map_a,
                                     const T& type_a,
                                     const char* type_name_a)
            {
                auto it_ = type_map_a.find(type_a);
                if (it_ != type_map_a.end())
                    return it_->second;
                MapFailed(type_name_a, "string");
            }

            template <typename T>
            T StringToType(const std::map<T, std::string>& type_map_a,
                           const std::string& str_a,
                           const char* type_name_a)
            {
                for (const auto& entry_ : type_map_a)
                {
                    if (entry_.second == str_a)
                        return entry_.first;
                }
                MapFailed("string", type_name_a);
            }

            template <typename T>
            std::string TypeToStringSecondary(
                    const std::map<std::string, T>& type_map_a,
                    const T& type_a,
                    const char* type_name_a)
            {
                for (const auto& entry_ : type_map_a)
                {
                    if (entry_.second == type_a)
                        return entry_.first;
                }
                MapFailed(type_name_a, "string");
            }

            template <typename T>
            T StringToTypeSecondary(const std::map<std::string, T>& type_map_a,
                                    const std::string& str_a,
                                    const char* type_name_a)
            {
                auto it_ = type_map_a.find(str_a);
                if (it_ != type_map_a.end())
                    return it_->second;
                MapFailed("string", type_name_a);
            }

            static const std::map<AssignOp, std::string> g_map_assign_op_ 
            {
                    {AssignOp::Set, "="},
                    {AssignOp::Add, "+="},
                    {AssignOp::Sub, "-="},
                    {AssignOp::Mul, "*="},
                    {AssignOp::Div, "/="},
                    {AssignOp::Mod, "%="},
                    {AssignOp::LShift, "<<="},
                    {AssignOp::RShift, ">>="},
                    {AssignOp::Or, "|="},
                    {AssignOp::And, "&="},
                    {AssignOp::Xor, "^="},
            };

            std::string AssignOpToString(const AssignOp o_a)
            {
                return TypeToString(g_map_assign_op_, o_a, "AssignOp");
            }

            AssignOp StringToAssignOp(const std::string& s_a)
            {
                return StringToType(g_map_assign_op_, s_a, "AssignOp");
            }

            bool IsBitwiseOp(const AssignOp o_a)
            {
                return (o_a >= AssignOp::LShift && o_a <= AssignOp::Xor);
            }

            static const std::map<BinaryOp, std::string> g_map_binary_op_ 
            {
                    {BinaryOp::LogicalAnd, "&&"},
                    {BinaryOp::LogicalOr, "||"},
                    {BinaryOp::Or, "|"},
                    {BinaryOp::Xor, "^"},
                    {BinaryOp::And, "&"},
                    {BinaryOp::LShift, "<<"},
                    {BinaryOp::RShift, ">>"},
                    {BinaryOp::Add, "+"},
                    {BinaryOp::Sub, "-"},
                    {BinaryOp::Mul, "*"},
                    {BinaryOp::Div, "/"},
                    {BinaryOp::Mod, "%"},
                    {BinaryOp::Equal, "=="},
                    {BinaryOp::NotEqual, "!="},
                    {BinaryOp::Less, "<"},
                    {BinaryOp::Greater, ">"},
                    {BinaryOp::LessEqual, "<="},
                    {BinaryOp::GreaterEqual, ">="},
            };

            std::string BinaryOpToString(const BinaryOp o_a)
            {
                return TypeToString(g_map_binary_op_, o_a, "BinaryOp");
            }

            BinaryOp StringToBinaryOp(const std::string& s_a)
            {
                return StringToType(g_map_binary_op_, s_a, "BinaryOp");
            }

            BinaryOp AssignOpToBinaryOp(const AssignOp op_a)
            {
                switch (op_a)
                {
                case AssignOp::Add:
                    return BinaryOp::Add;
                case AssignOp::Sub:
                    return BinaryOp::Sub;
                case AssignOp::Mul:
                    return BinaryOp::Mul;
                case AssignOp::Div:
                    return BinaryOp::Div;
                case AssignOp::Mod:
                    return BinaryOp::Mod;
                case AssignOp::LShift:
                    return BinaryOp::LShift;
                case AssignOp::RShift:
                    return BinaryOp::RShift;
                case AssignOp::Or:
                    return BinaryOp::Or;
                case AssignOp::And:
                    return BinaryOp::And;
                case AssignOp::Xor:
                    return BinaryOp::Xor;
                default:
                    return BinaryOp::Undefined;
                }
            }

            bool IsLogicalOp(const BinaryOp o_a)
            {
                return (o_a >= BinaryOp::LogicalAnd && o_a <= BinaryOp::LogicalOr);
            }

            bool IsBitwiseOp(const BinaryOp o_a)
            {
                return (o_a >= BinaryOp::Or && o_a <= BinaryOp::RShift);
            }

            bool IsCompareOp(const BinaryOp o_a)
            {
                return (o_a >= BinaryOp::Equal && o_a <= BinaryOp::GreaterEqual);
            }

            bool IsBooleanOp(const BinaryOp o_a)
            {
                return (IsLogicalOp(o_a) || IsCompareOp(o_a));
            }

            static const std::map<UnaryOp, std::string> g_map_unary_op_ 
            {
                    {UnaryOp::LogicalNot, "!"},
                    {UnaryOp::Not, "~"},
                    {UnaryOp::Nop, "+"},
                    {UnaryOp::Negate, "-"},
                    {UnaryOp::Inc, "++"},
                    {UnaryOp::Dec, "--"},
            };

            std::string UnaryOpToString(const UnaryOp o_a)
            {
                return TypeToString(g_map_unary_op_, o_a, "UnaryOp");
            }

            UnaryOp StringToUnaryOp(const std::string& s_a)
            {
                return StringToType(g_map_unary_op_, s_a, "UnaryOp");
            }

            bool IsLogicalOp(const UnaryOp o_a)
            {
                return (o_a == UnaryOp::LogicalNot);
            }

            bool IsBitwiseOp(const UnaryOp o_a)
            {
                return (o_a == UnaryOp::Not);
            }

            bool IsLValueOp(const UnaryOp o_a)
            {
                return (o_a == UnaryOp::Inc || o_a == UnaryOp::Dec);
            }

            static const std::map<CtrlTransfer, std::string> g_map_ctrl_transfer_ 
            {
                    {CtrlTransfer::Break, "break"},
                    {CtrlTransfer::Continue, "continue"},
                    {CtrlTransfer::Discard, "discard"},
            };

            std::string CtrlTransformToString(const CtrlTransfer ct_a)
            {
                return TypeToString(g_map_ctrl_transfer_, ct_a, "CtrlTransfer");
            }

            CtrlTransfer StringToCtrlTransfer(const std::string& s_a)
            {
                return StringToType(g_map_ctrl_transfer_, s_a, "CtrlTransfer");
            }

            MatrixSubscriptUsage::MatrixSubscriptUsage(
                    const DataType data_type_in_a,
                    const std::string& subscript_a)
                : data_type_in_ {data_type_in_a},
                  data_type_out_ {
                          SubscriptDataType(data_type_in_a, subscript_a, &indices_)}
            {}

            bool MatrixSubscriptUsage::operator<(
                    const MatrixSubscriptUsage& rhs_a) const
            {
                if (data_type_in_ < rhs_a.data_type_in_)
                {
                    return true;
                }
                if (data_type_in_ > rhs_a.data_type_in_)
                {
                    return false;
                }
                return indices_ < rhs_a.indices_;
            }

            std::string MatrixSubscriptUsage::IndicesToString() const
            {
                std::string s_;

                for (const auto& i_ : indices_)
                {
                    s_ += '_';
                    s_ += std::to_string(i_.first);
                    s_ += std::to_string(i_.second);
                }

                return s_;
            }

            std::string DataTypeToString(const DataType t_a,
                                         bool use_template_syntax_a)
            {
                if (t_a == DataType::String)
                    return "string";
                else if (IsScalarType(t_a))
                {
                    switch (t_a)
                    {
                    case DataType::Bool:
                        return "bool";
                    case DataType::Int:
                        return "int";
                    case DataType::UInt:
                        return "uint";
                    case DataType::Half:
                        return "half";
                    case DataType::Float:
                        return "float";
                    case DataType::Double:
                        return "double";
                    default:
                        break;
                    }
                } 
                else if (IsVectorType(t_a))
                {
                    auto dAz_ = VectorTypeDAz(t_a);
                    if (use_template_syntax_a)
                        return "vector<" + DataTypeToString(BaseDataType(t_a))
                               + ", " + std::to_string(dAz_) + ">";
                    else
                        return DataTypeToString(BaseDataType(t_a))
                               + std::to_string(dAz_);
                } 
                else if (IsMatrixType(t_a))
                {
                    auto dAz_ = MatrixTypeDAz(t_a);
                    if (use_template_syntax_a)
                        return "matrix<" + DataTypeToString(BaseDataType(t_a))
                               + ", " + std::to_string(dAz_.first) + ", "
                               + std::to_string(dAz_.second) + ">";
                    else
                        return DataTypeToString(BaseDataType(t_a))
                               + std::to_string(dAz_.first) + "x"
                               + std::to_string(dAz_.second);
                }
                return R_Undefined;
            }

            unsigned int DataTypeSize(const DataType t_a)
            {
                if (t_a >= DataType::Bool && t_a <= DataType::Double4x4)
                {
                    auto dAz_ = MatrixTypeDAz(t_a);
                    auto dAz_size_ = static_cast<unsigned int>(dAz_.first
                                                             * dAz_.second);
                    if (IsDoubleRealType(t_a))
                        return dAz_size_ * 8;
                    if (IsIntegralType(t_a) || IsSingleRealType(t_a))
                        return dAz_size_ * 4;
                    if (IsHalfRealType(t_a))
                        return dAz_size_ * 2;
                    return dAz_size_;
                }
                return 0;
            }

            bool IsScalarType(const DataType t_a)
            {
                return (t_a >= DataType::Bool && t_a <= DataType::Double);
            }

            bool IsVectorType(const DataType t_a)
            {
                return (t_a >= DataType::Bool2 && t_a <= DataType::Double4);
            }

            bool IsMatrixType(const DataType t_a)
            {
                return (t_a >= DataType::Bool2x2 && t_a <= DataType::Double4x4);
            }

            bool IsBooleanType(const DataType t_a)
            {
                return ((t_a == DataType::Bool)
                        || (t_a >= DataType::Bool2 && t_a <= DataType::Bool4)
                        || (t_a >= DataType::Bool2x2 && t_a <= DataType::Bool4x4));
            }

            bool IsRealType(const DataType t_a)
            {
                return ((t_a >= DataType::Half && t_a <= DataType::Double)
                        || (t_a >= DataType::Half2 && t_a <= DataType::Double4)
                        || (t_a >= DataType::Half2x2
                            && t_a <= DataType::Double4x4));
            }

            bool IsHalfRealType(const DataType t_a)
            {
                return ((t_a == DataType::Half)
                        || (t_a >= DataType::Half2 && t_a <= DataType::Half4)
                        || (t_a >= DataType::Half2x2 && t_a <= DataType::Half4x4));
            }

            bool IsSingleRealType(const DataType t_a)
            {
                return ((t_a == DataType::Float)
                        || (t_a >= DataType::Float2 && t_a <= DataType::Float4)
                        || (t_a >= DataType::Float2x2
                            && t_a <= DataType::Float4x4));
            }

            bool IsDoubleRealType(const DataType t_a)
            {
                return ((t_a == DataType::Double)
                        || (t_a >= DataType::Double2 && t_a <= DataType::Double4)
                        || (t_a >= DataType::Double2x2
                            && t_a <= DataType::Double4x4));
            }

            bool IsIntegralType(const DataType t_a)
            {
                return ((t_a >= DataType::Int && t_a <= DataType::UInt)
                        || (t_a >= DataType::Int2 && t_a <= DataType::UInt4)
                        || (t_a >= DataType::Int2x2 && t_a <= DataType::UInt4x4));
            }

            bool IsIntType(const DataType t_a)
            {
                return ((t_a == DataType::Int)
                        || (t_a >= DataType::Int2 && t_a <= DataType::Int4)
                        || (t_a >= DataType::Int2x2 && t_a <= DataType::Int4x4));
            }

            bool IsUIntType(const DataType t_a)
            {
                return ((t_a == DataType::UInt)
                        || (t_a >= DataType::UInt2 && t_a <= DataType::UInt4)
                        || (t_a >= DataType::UInt2x2 && t_a <= DataType::UInt4x4));
            }

            int VectorTypeDAz(const DataType t_a)
            {
                switch (t_a)
                {
                case DataType::Bool:
                case DataType::Int:
                case DataType::UInt:
                case DataType::Half:
                case DataType::Float:
                case DataType::Double:
                    return 1;

                case DataType::Bool2:
                case DataType::Int2:
                case DataType::UInt2:
                case DataType::Half2:
                case DataType::Float2:
                case DataType::Double2:
                    return 2;

                case DataType::Bool3:
                case DataType::Int3:
                case DataType::UInt3:
                case DataType::Half3:
                case DataType::Float3:
                case DataType::Double3:
                    return 3;

                case DataType::Bool4:
                case DataType::Int4:
                case DataType::UInt4:
                case DataType::Half4:
                case DataType::Float4:
                case DataType::Double4:
                    return 4;

                default:
                    return 0;
                }
            }

            std::pair<int, int> MatrixTypeDAz(const DataType t_a)
            {
                switch (t_a)
                {
                case DataType::Bool:
                case DataType::Int:
                case DataType::UInt:
                case DataType::Half:
                case DataType::Float:
                case DataType::Double:
                    return {1, 1};

                case DataType::Bool2:
                case DataType::Int2:
                case DataType::UInt2:
                case DataType::Half2:
                case DataType::Float2:
                case DataType::Double2:
                    return {2, 1};

                case DataType::Bool3:
                case DataType::Int3:
                case DataType::UInt3:
                case DataType::Half3:
                case DataType::Float3:
                case DataType::Double3:
                    return {3, 1};

                case DataType::Bool4:
                case DataType::Int4:
                case DataType::UInt4:
                case DataType::Half4:
                case DataType::Float4:
                case DataType::Double4:
                    return {4, 1};

                case DataType::Bool2x2:
                case DataType::Int2x2:
                case DataType::UInt2x2:
                case DataType::Half2x2:
                case DataType::Float2x2:
                case DataType::Double2x2:
                    return {2, 2};

                case DataType::Bool2x3:
                case DataType::Int2x3:
                case DataType::UInt2x3:
                case DataType::Half2x3:
                case DataType::Float2x3:
                case DataType::Double2x3:
                    return {2, 3};

                case DataType::Bool2x4:
                case DataType::Int2x4:
                case DataType::UInt2x4:
                case DataType::Half2x4:
                case DataType::Float2x4:
                case DataType::Double2x4:
                    return {2, 4};

                case DataType::Bool3x2:
                case DataType::Int3x2:
                case DataType::UInt3x2:
                case DataType::Half3x2:
                case DataType::Float3x2:
                case DataType::Double3x2:
                    return {3, 2};

                case DataType::Bool3x3:
                case DataType::Int3x3:
                case DataType::UInt3x3:
                case DataType::Half3x3:
                case DataType::Float3x3:
                case DataType::Double3x3:
                    return {3, 3};

                case DataType::Bool3x4:
                case DataType::Int3x4:
                case DataType::UInt3x4:
                case DataType::Half3x4:
                case DataType::Float3x4:
                case DataType::Double3x4:
                    return {3, 4};

                case DataType::Bool4x2:
                case DataType::Int4x2:
                case DataType::UInt4x2:
                case DataType::Half4x2:
                case DataType::Float4x2:
                case DataType::Double4x2:
                    return {4, 2};

                case DataType::Bool4x3:
                case DataType::Int4x3:
                case DataType::UInt4x3:
                case DataType::Half4x3:
                case DataType::Float4x3:
                case DataType::Double4x3:
                    return {4, 3};

                case DataType::Bool4x4:
                case DataType::Int4x4:
                case DataType::UInt4x4:
                case DataType::Half4x4:
                case DataType::Float4x4:
                case DataType::Double4x4:
                    return {4, 4};

                default:
                    return {0, 0};
                }
            }

            DataType BaseDataType(const DataType t_a)
            {
#define FIND_BASETYPE(TYPENAME)                                                \
    if ((t_a >= DataType::TYPENAME##2 && t_a <= DataType::TYPENAME##4)             \
        || (t_a >= DataType::TYPENAME##2x2 && t_a <= DataType::TYPENAME##4x4))     \
    {                                                                          \
        return DataType::TYPENAME;                                             \
    }

                FIND_BASETYPE(Bool);
                FIND_BASETYPE(Int);
                FIND_BASETYPE(UInt);
                FIND_BASETYPE(Half);
                FIND_BASETYPE(Float);
                FIND_BASETYPE(Double);

                return t_a;

#undef FIND_BASETYPE
            }

            static int Idx(const DataType t_a)
            {
                return static_cast<int>(t_a);
            };

            DataType VectorDataType(const DataType base_data_type_a, int vector_size_a)
            {
                if (IsScalarType(base_data_type_a))
                {
                    if (vector_size_a >= 2 && vector_size_a <= 4)
                    {
                        auto offset_ = (Idx(base_data_type_a) - Idx(DataType::Bool));
                        auto idx_ = Idx(DataType::Bool2) + offset_ * 3
                                   + (vector_size_a - 2);

                        return static_cast<DataType>(idx_);
                    } 
                    else if (vector_size_a == 1)
                        return base_data_type_a;
                }
                return DataType::Undefined;
            }

            DataType MatrixDataType(const DataType base_data_type_a,
                                    int rows_a,
                                    int columns_a)
            {
                if (IsScalarType(base_data_type_a))
                {
                    if (rows_a == 1 && columns_a == 1)
                        return base_data_type_a;
                    if (rows_a == 1)
                        return VectorDataType(base_data_type_a, columns_a);
                    if (columns_a == 1)
                        return VectorDataType(base_data_type_a, rows_a);

                    if (rows_a >= 2 && rows_a <= 4 && columns_a >= 2 && columns_a <= 4)
                    {
                        auto offset_ = (Idx(base_data_type_a) - Idx(DataType::Bool));
                        auto idx_ = Idx(DataType::Bool2x2) + offset_ * 9
                                   + (rows_a - 2) * 3 + (columns_a - 2);

                        return static_cast<DataType>(idx_);
                    }
                }
                return DataType::Undefined;
            }

            static DataType SubscriptDataTypeVector(
                    const DataType data_type_a,
                    const std::string& subscript_a,
                    int vector_size_a,
                    std::vector<std::pair<int, int>>* indices_a)
            {
                auto IsValidSubscript = [&](std::string compare_subscript_,
                                            int vector_size_) -> bool {
                    compare_subscript_.resize(vector_size_);

                    std::vector<std::pair<int, int>> subscript_indices_;
                    subscript_indices_.reserve(vector_size_);

                    for (auto chr_ : subscript_a)
                    {
                        auto pos_ = compare_subscript_.find(chr_);
                        if (pos_ != std::string::npos)
                            subscript_indices_.push_back(
                                    {static_cast<int>(pos_), 0});
                        else
                            return false;
                    }

                    if (indices_a)
                        *indices_a = std::move(subscript_indices_);

                    return true;
                };

                auto subscript_size_ = subscript_a.size();

                if (subscript_size_ < 1 || subscript_size_ > 4)
                    InvalidArg(R_VectorSubscriptCantHaveNComps(subscript_size_));

                if (vector_size_a < 1 || vector_size_a > 4)
                    InvalidArg(R_InvalidVectorDAzension(vector_size_a));

                bool valid_subscript_ = (IsValidSubscript("xyzw", vector_size_a)
                                       || IsValidSubscript("rgba", vector_size_a));

                if (!valid_subscript_)
                    InvalidArg(R_InvalidVectorSubscript(subscript_a,
                                                        DataTypeToString(
                                                                data_type_a)));

                return VectorDataType(BaseDataType(data_type_a),
                                      static_cast<int>(subscript_size_));
            }

            static void ParseNextMatrixSubscript(
                    const std::string& s_a,
                    std::size_t& i_a,
                    char& zero_base_a,
                    int rows_a,
                    int cols_a,
                    std::vector<std::pair<int, int>>* indices_a)
            {
                if (i_a + 3 > s_a.size())
                    InvalidArg(R_IncompleteMatrixSubscript(s_a));
                if (s_a[i_a] != '_')
                    InvalidArg(
                            R_InvalidCharInMatrixSubscript(std::string(1, s_a[i_a]),
                                                           s_a));
                ++i_a;

                if (s_a[i_a] == 'm')
                {
                    if (zero_base_a == 1)
                        InvalidArg(R_InvalidMatrixSubscriptMixture(s_a));
                    else
                        zero_base_a = 0;

                    ++i_a;
                    if (i_a + 2 > s_a.size())
                        InvalidArg(R_IncompleteMatrixSubscript(s_a));
                } else
                {
                    if (zero_base_a == 0)
                        InvalidArg(R_InvalidMatrixSubscriptMixture(s_a));
                    else
                        zero_base_a = 1;
                }

                int subscript_indices_[2] = {0};

                const int max_idx_[2] = {rows_a, cols_a};
                const JoinableString matrix_dAz_str_(" {0}x{1}");

                for (int j_ = 0; j_ < 2; ++j_)
                {
                    if (s_a[i_a] >= '0' + zero_base_a
                        && s_a[i_a] < '0' + max_idx_[j_] + zero_base_a)
                    {
                        subscript_indices_[j_] = s_a[i_a] - ('0' + zero_base_a);
                    } 
                    else
                    {
                        InvalidArg(R_InvalidCharInMatrixSubscript(
                                std::string(1, s_a[i_a]),
                                s_a,
                                (zero_base_a == 0 ? R_ZeroBased : R_OneBased)
                                        + matrix_dAz_str_(rows_a, cols_a)));
                    }
                    ++i_a;
                }

                if (indices_a)
                    indices_a->push_back(
                            {subscript_indices_[0], subscript_indices_[1]});
            }

            static DataType SubscriptDataTypeMatrix(
                    const DataType data_type_a,
                    const std::string& subscript_a,
                    int rows_a,
                    int cols_a,
                    std::vector<std::pair<int, int>>* indices_a)
            {
                if (rows_a < 1 || rows_a > 4 || cols_a < 1 || cols_a > 4)
                    InvalidArg(R_InvalidMatrixDAzension(rows_a, cols_a));

                int vector_size_ = 0;
                char zero_base_ = -1;

                for (std::size_t i_ = 0; i_ < subscript_a.size(); ++vector_size_)
                    ParseNextMatrixSubscript(subscript_a,
                                             i_,
                                             zero_base_,
                                             rows_a,
                                             cols_a,
                                             indices_a);

                return VectorDataType(BaseDataType(data_type_a), vector_size_);
            }

            DataType SubscriptDataType(
                    const DataType data_type_a,
                    const std::string& subscript_a,
                    std::vector<std::pair<int, int>>* indices_a)
            {
                auto matrix_dAz_ = MatrixTypeDAz(data_type_a);
                if (matrix_dAz_.second == 1)
                    return SubscriptDataTypeVector(data_type_a,
                                                   subscript_a,
                                                   matrix_dAz_.first,
                                                   indices_a);
                else
                    return SubscriptDataTypeMatrix(data_type_a,
                                                   subscript_a,
                                                   matrix_dAz_.first,
                                                   matrix_dAz_.second,
                                                   indices_a);
            }

            static DataType IntLiteralTokenToDataType(const Token& tkn_a)
            {
                const auto& s_ = tkn_a.Spell();

                if (!s_.empty())
                {
                    if (s_.back() == 'u' || s_.back() == 'U')
                        return DataType::UInt;
                }

                return DataType::Int;
            }

            static DataType FloatLiteralTokenToDataType(const Token& tkn_a)
            {
                const auto& s_ = tkn_a.Spell();

                if (!s_.empty())
                {
                    if (s_.back() == 'f' || s_.back() == 'F')
                        return DataType::Float;

                    if (s_.back() == 'h' || s_.back() == 'H')
                        return DataType::Half;
                }

                return DataType::Double;
            }

            DataType TokenToDataType(const Token& tkn_a)
            {
                switch (tkn_a.Type())
                {
                case Token::Types::BoolLiteral:
                    return DataType::Bool;
                case Token::Types::IntLiteral:
                    return IntLiteralTokenToDataType(tkn_a);
                case Token::Types::FloatLiteral:
                    return FloatLiteralTokenToDataType(tkn_a);
                case Token::Types::StringLiteral:
                    return DataType::String;
                default:
                    break;
                }
                return DataType::Undefined;
            }

            DataType DoubleToFloatDataType(const DataType data_type_a)
            {
                if (data_type_a == DataType::Double)
                    return DataType::Float;
                if (data_type_a >= DataType::Double2
                    && data_type_a <= DataType::Double4)
                    return static_cast<DataType>(Idx(data_type_a)
                                                 - Idx(DataType::Double2)
                                                 + Idx(DataType::Float2));
                if (data_type_a >= DataType::Double2x2
                    && data_type_a <= DataType::Double4x4)
                    return static_cast<DataType>(Idx(data_type_a)
                                                 - Idx(DataType::Double2x2)
                                                 + Idx(DataType::Float2x2));
                return data_type_a;
            }

            unsigned int RemainingVectorSize(unsigned int vector_size_a,
                                             unsigned int alignment_a)
            {
                return (alignment_a - vector_size_a % alignment_a) % alignment_a;
            }

            bool AccumAlignedVectorSize(const DataType data_type_a,
                                        unsigned int& size_a,
                                        unsigned int& padding_a,
                                        unsigned int* offset_a)
            {
                auto data_size_ = DataTypeSize(data_type_a);
                if (data_size_ > 0)
                {
                    auto remaining_size_ = RemainingVectorSize(size_a);
                    if (remaining_size_ > 0 && data_size_ > remaining_size_)
                    {
                        size_a += remaining_size_;
                        padding_a += remaining_size_;
                    }

                    if (offset_a != nullptr)
                        *offset_a = size_a;

                    size_a += data_size_;

                    return true;
                }
                return false;
            }

            static const std::map<BufferType, std::string> g_map_buffer_type_ 
            {
                    {BufferType::Buffer, "Buffer"},
                    {BufferType::StructuredBuffer, "StructuredBuffer"},
                    {BufferType::ByteAddressBuffer, "ByteAddressBuffer"},

                    {BufferType::RWBuffer, "RWBuffer"},
                    {BufferType::RWStructuredBuffer, "RWStructuredBuffer"},
                    {BufferType::RWByteAddressBuffer, "RWByteAddressBuffer"},
                    {BufferType::AppendStructuredBuffer,
                     "AppendStructuredBuffer"},
                    {BufferType::ConsumeStructuredBuffer,
                     "ConsumeStructuredBuffer"},

                    {BufferType::RWTexture1D, "RWTexture1D"},
                    {BufferType::RWTexture1DArray, "RWTexture1DArray"},
                    {BufferType::RWTexture2D, "RWTexture2D"},
                    {BufferType::RWTexture2DArray, "RWTexture2DArray"},
                    {BufferType::RWTexture3D, "RWTexture3D"},

                    {BufferType::Texture1D, "Texture1D"},
                    {BufferType::Texture1DArray, "Texture1DArray"},
                    {BufferType::Texture2D, "Texture2D"},
                    {BufferType::Texture2DArray, "Texture2DArray"},
                    {BufferType::Texture3D, "Texture3D"},
                    {BufferType::TextureCube, "TextureCube"},
                    {BufferType::TextureCubeArray, "TextureCubeArray"},
                    {BufferType::Texture2DMS, "Texture2DMS"},
                    {BufferType::Texture2DMSArray, "Texture2DMSArray"},

                    {BufferType::GenericTexture, "Texture"},

                    {BufferType::InputPatch, "InputPatch"},
                    {BufferType::OutputPatch, "OutputPatch"},

                    {BufferType::PointStream, "PointStream"},
                    {BufferType::LineStream, "LineStream"},
                    {BufferType::TriangleStream, "TriangleStream"},

                    {BufferType::GenericBuffer, "buffer"},
            };

            std::string BufferTypeToString(const BufferType t_a)
            {
                return TypeToString(g_map_buffer_type_, t_a, "BufferType");
            }

            bool IsStorageBufferType(const BufferType t_a)
            {
                return ((t_a >= BufferType::StructuredBuffer
                         && t_a <= BufferType::ByteAddressBuffer)
                        || (t_a >= BufferType::RWStructuredBuffer
                            && t_a <= BufferType::ConsumeStructuredBuffer)
                        || (t_a == BufferType::GenericBuffer));
            }

            bool IsRWBufferType(const BufferType t_a)
            {
                return (t_a >= BufferType::RWBuffer
                        && t_a <= BufferType::RWTexture3D);
            }

            bool IsTextureBufferType(const BufferType t_a)
            {
                return (t_a >= BufferType::RWTexture1D
                        && t_a <= BufferType::GenericTexture);
            }

            bool IsTextureMSBufferType(const BufferType t_a)
            {
                return (t_a >= BufferType::Texture2DMS
                        && t_a <= BufferType::Texture2DMSArray);
            }

            bool IsAzageBufferType(const BufferType t_a)
            {
                return ((t_a >= BufferType::RWTexture1D
                         && t_a <= BufferType::GenericTexture)
                        || t_a == BufferType::Buffer);
            }

            bool IsRWAzageBufferType(const BufferType t_a)
            {
                return ((t_a >= BufferType::RWTexture1D
                         && t_a <= BufferType::RWTexture3D)
                        || t_a == BufferType::RWBuffer);
            }

            bool IsPatchBufferType(const BufferType t_a)
            {
                return (t_a >= BufferType::InputPatch
                        && t_a <= BufferType::OutputPatch);
            }

            bool IsStreamBufferType(const BufferType t_a)
            {
                return (t_a >= BufferType::PointStream
                        && t_a <= BufferType::TriangleStream);
            }

            int GetBufferTypeTextureDAz(const BufferType t_a)
            {
                switch (t_a)
                {
                case BufferType::Buffer:
                case BufferType::RWBuffer:
                case BufferType::Texture1D:
                case BufferType::RWTexture1D:
                    return 1;

                case BufferType::Texture1DArray:
                case BufferType::RWTexture1DArray:
                case BufferType::Texture2D:
                case BufferType::RWTexture2D:
                case BufferType::Texture2DMS:
                    return 2;

                case BufferType::Texture2DArray:
                case BufferType::RWTexture2DArray:
                case BufferType::Texture2DMSArray:
                case BufferType::Texture3D:
                case BufferType::RWTexture3D:
                case BufferType::TextureCube:
                    return 3;

                case BufferType::TextureCubeArray:
                    return 4;

                default:
                    return 0;
                }
            }

            bool IsSamplerStateType(const SamplerType t_a)
            {
                return (t_a >= SamplerType::SamplerState
                        && t_a <= SamplerType::SamplerComparisonState);
            }

            bool IsSamplerTypeShadow(const SamplerType t_a)
            {
                return (t_a >= SamplerType::Sampler1DShadow
                        && t_a <= SamplerType::SamplerCubeArrayShadow);
            }

            bool IsSamplerTypeArray(const SamplerType t_a)
            {
                return ((t_a >= SamplerType::Sampler1DArray
                         && t_a <= SamplerType::SamplerCubeArray)
                        || t_a == SamplerType::Sampler2DMSArray);
            }

            int GetSamplerTypeTextureDAz(const SamplerType t_a)
            {
                switch (t_a)
                {
                case SamplerType::Sampler1D:
                case SamplerType::SamplerBuffer:
                case SamplerType::Sampler1DShadow:
                    return 1;

                case SamplerType::Sampler2D:
                case SamplerType::Sampler2DRect:
                case SamplerType::Sampler1DArray:
                case SamplerType::Sampler2DMS:
                case SamplerType::Sampler2DShadow:
                case SamplerType::Sampler2DRectShadow:
                case SamplerType::Sampler1DArrayShadow:
                    return 2;

                case SamplerType::Sampler3D:
                case SamplerType::SamplerCube:
                case SamplerType::Sampler2DArray:
                case SamplerType::Sampler2DMSArray:
                case SamplerType::SamplerCubeShadow:
                case SamplerType::Sampler2DArrayShadow:
                    return 3;

                case SamplerType::SamplerCubeArray:
                case SamplerType::SamplerCubeArrayShadow:
                    return 4;

                default:
                    return 0;
                }
            }

            SamplerType TextureTypeToSamplerType(const BufferType t_a)
            {
                switch (t_a)
                {
                case BufferType::Texture1D:
                    return SamplerType::Sampler1D;
                case BufferType::Texture1DArray:
                    return SamplerType::Sampler1DArray;
                case BufferType::Texture2D:
                    return SamplerType::Sampler2D;
                case BufferType::Texture2DArray:
                    return SamplerType::Sampler2DArray;
                case BufferType::Texture3D:
                    return SamplerType::Sampler3D;
                case BufferType::TextureCube:
                    return SamplerType::SamplerCube;
                case BufferType::TextureCubeArray:
                    return SamplerType::SamplerCubeArray;
                default:
                    return SamplerType::Undefined;
                }
            }

            SamplerType SamplerTypeToShadowSamplerType(const SamplerType t_a)
            {
                switch (t_a)
                {
                case SamplerType::Sampler1D:
                    return SamplerType::Sampler1DShadow;
                case SamplerType::Sampler1DArray:
                    return SamplerType::Sampler1DArrayShadow;
                case SamplerType::Sampler2D:
                    return SamplerType::Sampler2DShadow;
                case SamplerType::Sampler2DArray:
                    return SamplerType::Sampler2DArrayShadow;
                case SamplerType::Sampler2DRect:
                    return SamplerType::Sampler2DRectShadow;
                case SamplerType::SamplerCube:
                    return SamplerType::SamplerCubeShadow;
                case SamplerType::SamplerCubeArray:
                    return SamplerType::SamplerCubeArrayShadow;
                default:
                    return SamplerType::Undefined;
                }
            }

            DataType GetAzageLayoutFormatBaseType(
                    const AzageLayoutFormat format_a)
            {
                if (format_a >= AzageLayoutFormat::F32X4
                    && format_a <= AzageLayoutFormat::SN8X1)
                    return DataType::Float;
                if (format_a >= AzageLayoutFormat::I32X4
                    && format_a <= AzageLayoutFormat::I8X1)
                    return DataType::Int;
                if (format_a >= AzageLayoutFormat::UI32X4
                    && format_a <= AzageLayoutFormat::UI8X1)
                    return DataType::UInt;
                return DataType::Undefined;
            }

            AzageLayoutFormat DataTypeToAzageLayoutFormat(const DataType t_a)
            {
                switch (t_a)
                {
                case DataType::Int:
                    return AzageLayoutFormat::I32X1;
                case DataType::Int2:
                    return AzageLayoutFormat::I32X2;
                case DataType::Int4:
                    return AzageLayoutFormat::I32X4;
                case DataType::UInt:
                    return AzageLayoutFormat::UI32X1;
                case DataType::UInt2:
                    return AzageLayoutFormat::UI32X2;
                case DataType::UInt4:
                    return AzageLayoutFormat::UI32X4;
                case DataType::Float:
                    return AzageLayoutFormat::F32X1;
                case DataType::Float2:
                    return AzageLayoutFormat::F32X2;
                case DataType::Float4:
                    return AzageLayoutFormat::F32X4;
                default:
                    return AzageLayoutFormat::Undefined;
                }
            }

            RegisterType CharToRegisterType(char c_a)
            {
                switch (c_a)
                {
                case 'b':
                    return RegisterType::ConstantBuffer;
                case 't':
                    return RegisterType::TextureBuffer;
                case 'c':
                    return RegisterType::BufferOffset;
                case 's':
                    return RegisterType::Sampler;
                case 'u':
                    return RegisterType::UnorderedAccessView;
                }
                return RegisterType::Undefined;
            }

            char RegisterTypeToChar(const RegisterType t_a)
            {
                switch (t_a)
                {
                case RegisterType::Undefined:
                    break;
                case RegisterType::ConstantBuffer:
                    return 'b';
                case RegisterType::TextureBuffer:
                    return 't';
                case RegisterType::BufferOffset:
                    return 'c';
                case RegisterType::Sampler:
                    return 's';
                case RegisterType::UnorderedAccessView:
                    return 'u';
                }
                return '\0';
            }

            std::string RegisterTypeToString(const RegisterType t_a)
            {
                switch (t_a)
                {
                case RegisterType::Undefined:
                    break;
                case RegisterType::ConstantBuffer:
                    return "ConstantBuffer";
                case RegisterType::TextureBuffer:
                    return "TextureBuffer";
                case RegisterType::BufferOffset:
                    return "BufferOffset";
                case RegisterType::Sampler:
                    return "Sampler";
                case RegisterType::UnorderedAccessView:
                    return "UnorderedAccessView";
                }
                return "";
            }

            bool IsShaderModel3AttributeType(const AttributeType t_a)
            {
                return (t_a >= AttributeType::Branch && t_a <= AttributeType::Hps);
            }

            bool IsShaderModel5AttributeType(const AttributeType t_a)
            {
                return (t_a >= AttributeType::Domain
                        && t_a <= AttributeType::PatchConstantFunc);
            }

            bool IsHLSLAttributeType(const AttributeType t_a)
            {
                return (t_a >= AttributeType::Branch
                        && t_a <= AttributeType::PatchConstantFunc);
            }

            bool IsGLSLAttributeType(const AttributeType t_a)
            {
                return (t_a >= AttributeType::Align
                        && t_a <= AttributeType::HfbStride);
            }

            bool IsAttributeValueDomain(const AttributeValue t_a)
            {
                return (t_a >= AttributeValue::DomainTri
                        && t_a <= AttributeValue::DomainIsoline);
            }

            bool IsAttributeValueOutputTopology(const AttributeValue t_a)
            {
                return (t_a >= AttributeValue::OutputTopologyPoint
                        && t_a <= AttributeValue::OutputTopologyTriangleCCW);
            }

            bool IsAttributeValuePartitioning(const AttributeValue t_a)
            {
                return (t_a >= AttributeValue::PartitioningInteger
                        && t_a <= AttributeValue::PartitioningFractionalOdd);
            }

            bool IsAttributeValueTrianglePartitioning(const AttributeValue t_a)
            {
                return (t_a >= AttributeValue::OutputTopologyTriangleCW
                        && t_a <= AttributeValue::OutputTopologyTriangleCCW);
            }

            bool IsGlobalIntrinsic(const Intrinsic t_a)
            {
                return (t_a >= Intrinsic::Abort && t_a <= Intrinsic::TexCubeProj);
            }

            bool IsTextureIntrinsic(const Intrinsic t_a)
            {
                return (t_a >= Intrinsic::Texture_GetDAzensions
                        && t_a <= Intrinsic::Texture_GatherCmpAlpha_8);
            }

            bool IsStreamOutputIntrinsic(const Intrinsic t_a)
            {
                return (t_a >= Intrinsic::StreamOutput_Append
                        && t_a <= Intrinsic::StreamOutput_RestartStrip);
            }

            bool IsAzageIntrinsic(const Intrinsic t_a)
            {
                return (t_a >= Intrinsic::Azage_Load
                        && t_a <= Intrinsic::Azage_Store);
            }

            bool IsInterlockedIntristic(const Intrinsic t_a)
            {
                return (t_a >= Intrinsic::InterlockedAdd
                        && t_a <= Intrinsic::InterlockedXor);
            }

            bool IsTextureGatherIntrisic(const Intrinsic t_a)
            {
                return (t_a >= Intrinsic::Texture_Gather_2
                        && t_a <= Intrinsic::Texture_GatherCmpAlpha_8);
            }

            bool IsTextureSampleIntrinsic(const Intrinsic t_a)
            {
                return (t_a >= Intrinsic::Texture_Sample_2
                        && t_a <= Intrinsic::Texture_SampleLevel_5);
            }

            bool IsTextureCompareIntrinsic(const Intrinsic t_a)
            {
                return ((t_a >= Intrinsic::Texture_SampleCmp_3
                         && t_a <= Intrinsic::Texture_SampleCmpLevelZero_5)
                        || (t_a >= Intrinsic::Texture_GatherCmp_3
                            && t_a <= Intrinsic::Texture_GatherCmpAlpha_8));
            }

            bool IsTextureCompareLevelZeroIntrinsic(const Intrinsic t_a)
            {
                return (t_a >= Intrinsic::Texture_SampleCmpLevelZero_3
                        && t_a <= Intrinsic::Texture_SampleCmpLevelZero_5);
            }

            bool IsTextureLoadIntrinsic(const Intrinsic t_a)
            {
                return (t_a >= Intrinsic::Texture_Load_1
                        && t_a <= Intrinsic::Texture_Load_3);
            }

            Intrinsic CompareOpToIntrinsic(const BinaryOp op_a)
            {
                switch (op_a)
                {
                case BinaryOp::Equal:
                    return Intrinsic::Equal;
                case BinaryOp::NotEqual:
                    return Intrinsic::NotEqual;
                case BinaryOp::Less:
                    return Intrinsic::LessThan;
                case BinaryOp::Greater:
                    return Intrinsic::GreaterThan;
                case BinaryOp::LessEqual:
                    return Intrinsic::LessThanEqual;
                case BinaryOp::GreaterEqual:
                    return Intrinsic::GreaterThanEqual;
                default:
                    return Intrinsic::Undefined;
                }
            }

            Intrinsic InterlockedToAzageAtomicIntrinsic(const Intrinsic t_a)
            {
                switch (t_a)
                {
                case Intrinsic::InterlockedAdd:
                    return Intrinsic::Azage_AtomicAdd;
                case Intrinsic::InterlockedAnd:
                    return Intrinsic::Azage_AtomicAnd;
                case Intrinsic::InterlockedOr:
                    return Intrinsic::Azage_AtomicOr;
                case Intrinsic::InterlockedXor:
                    return Intrinsic::Azage_AtomicXor;
                case Intrinsic::InterlockedMin:
                    return Intrinsic::Azage_AtomicMin;
                case Intrinsic::InterlockedMax:
                    return Intrinsic::Azage_AtomicMax;
                case Intrinsic::InterlockedCompareExchange:
                    return Intrinsic::Azage_AtomicCompSwap;
                case Intrinsic::InterlockedExchange:
                    return Intrinsic::Azage_AtomicExchange;
                default:
                    return t_a;
                }
            }

            struct GatherIntrinsicInfo
            {
                GatherIntrinsicInfo(int component_idx_a,
                                    int offset_count_a,
                                    bool is_compare_a = false)
                    : component_idx_ {component_idx_a}, offset_count_ {offset_count_a},
                      is_compare_ {is_compare_a}
                {}

                int component_idx_ = 0;
                int offset_count_ = 0;
                bool is_compare_ = false;
            };

            static std::map<Intrinsic, GatherIntrinsicInfo>
            GenerateGatherIntrinsicInfoMap()
            {
                using T = Intrinsic;

                return {
                        {T::Texture_Gather_2, {0, 0}},
                        {T::Texture_GatherRed_2, {0, 0}},
                        {T::Texture_GatherGreen_2, {1, 0}},
                        {T::Texture_GatherBlue_2, {2, 0}},
                        {T::Texture_GatherAlpha_2, {3, 0}},

                        {T::Texture_Gather_3, {0, 1}},
                        {T::Texture_Gather_4, {0, 1}},
                        {T::Texture_GatherRed_3, {0, 1}},
                        {T::Texture_GatherRed_4, {0, 1}},
                        {T::Texture_GatherGreen_3, {1, 1}},
                        {T::Texture_GatherGreen_4, {1, 1}},
                        {T::Texture_GatherBlue_3, {2, 1}},
                        {T::Texture_GatherBlue_4, {2, 1}},
                        {T::Texture_GatherAlpha_3, {3, 1}},
                        {T::Texture_GatherAlpha_4, {3, 1}},

                        {T::Texture_GatherRed_6, {0, 4}},
                        {T::Texture_GatherRed_7, {0, 4}},
                        {T::Texture_GatherGreen_6, {1, 4}},
                        {T::Texture_GatherGreen_7, {1, 4}},
                        {T::Texture_GatherBlue_6, {2, 4}},
                        {T::Texture_GatherBlue_7, {2, 4}},
                        {T::Texture_GatherAlpha_6, {3, 4}},
                        {T::Texture_GatherAlpha_7, {3, 4}},

                        {T::Texture_GatherCmp_3, {0, 0, true}},
                        {T::Texture_GatherCmpRed_3, {0, 0, true}},
                        {T::Texture_GatherCmpGreen_3, {1, 0, true}},
                        {T::Texture_GatherCmpBlue_3, {2, 0, true}},
                        {T::Texture_GatherCmpAlpha_3, {3, 0, true}},

                        {T::Texture_GatherCmp_4, {0, 1, true}},
                        {T::Texture_GatherCmp_5, {0, 1, true}},
                        {T::Texture_GatherCmpRed_4, {0, 1, true}},
                        {T::Texture_GatherCmpRed_5, {0, 1, true}},
                        {T::Texture_GatherCmpGreen_4, {1, 1, true}},
                        {T::Texture_GatherCmpGreen_5, {1, 1, true}},
                        {T::Texture_GatherCmpBlue_4, {2, 1, true}},
                        {T::Texture_GatherCmpBlue_5, {2, 1, true}},
                        {T::Texture_GatherCmpAlpha_4, {3, 1, true}},
                        {T::Texture_GatherCmpAlpha_5, {3, 1, true}},

                        {T::Texture_GatherCmpRed_7, {0, 4, true}},
                        {T::Texture_GatherCmpRed_8, {0, 4, true}},
                        {T::Texture_GatherCmpGreen_7, {1, 4, true}},
                        {T::Texture_GatherCmpGreen_8, {1, 4, true}},
                        {T::Texture_GatherCmpBlue_7, {2, 4, true}},
                        {T::Texture_GatherCmpBlue_8, {2, 4, true}},
                        {T::Texture_GatherCmpAlpha_7, {3, 4, true}},
                        {T::Texture_GatherCmpAlpha_8, {3, 4, true}},
                };
            }

            static const auto g_gather_intrinsic_info_map_ =
                    GenerateGatherIntrinsicInfoMap();

            int GetGatherIntrinsicOffsetParamCount(const Intrinsic t_a)
            {
                auto it_ = g_gather_intrinsic_info_map_.find(t_a);
                if (it_ != g_gather_intrinsic_info_map_.end())
                    return it_->second.offset_count_;
                else
                    return 0;
            }

            int GetGatherIntrinsicComponentIndex(const Intrinsic t_a)
            {
                auto it_ = g_gather_intrinsic_info_map_.find(t_a);
                if (it_ != g_gather_intrinsic_info_map_.end())
                    return it_->second.component_idx_;
                else
                    return 0;
            }

            IndexedSemantic::IndexedSemantic(Semantic semantic_a, int index_a)
                : semantic_ {semantic_a}, index_ {index_a}
            {}

            IndexedSemantic::IndexedSemantic(const std::string& user_defined_a)
                : semantic_ {Semantic::UserDefined}
            {
                auto pos_ = user_defined_a.find_last_not_of("0123456789");
                if (pos_ != std::string::npos && pos_ + 1 < user_defined_a.size())
                {
                    ++pos_;
                    user_defined_ = user_defined_a.substr(0, pos_);
                    auto indexStr = user_defined_a.substr(pos_);
                    index_ = std::stoi(indexStr);
                } 
                else
                    user_defined_ = user_defined_a;
            }

            IndexedSemantic::IndexedSemantic(const IndexedSemantic& rhs_a,
                                             int index_a)
                : semantic_ {rhs_a.semantic_}, index_ {index_a},
                  user_defined_ {rhs_a.user_defined_}
            {}

            bool IndexedSemantic::operator<(const IndexedSemantic& rhs_a) const
            {
                if (semantic_ > rhs_a.semantic_)
                    return false;
                if (semantic_ < rhs_a.semantic_)
                    return true;

                if (index_ > rhs_a.index_)
                    return false;
                if (index_ < rhs_a.index_)
                    return true;

                return user_defined_ < rhs_a.user_defined_;
            }

            bool IndexedSemantic::IsValid() const
            {
                return (semantic_ != Semantic::Undefined);
            }

            bool IndexedSemantic::IsSystemValue() const
            {
                return IsSystemSemantic(semantic_);
            }

            bool IndexedSemantic::IsUserDefined() const
            {
                return IsUserSemantic(semantic_);
            }

            std::string IndexedSemantic::ToString() const
            {
                std::string s_;

                if (semantic_ == Semantic::UserDefined)
                {
                    s_ = user_defined_;
                    std::transform(s_.begin(), s_.end(), s_.begin(), ::toupper);
                } 
                else
                    s_ = SemanticToString(semantic_);

                s_ += std::to_string(index_);

                return s_;
            }

            void IndexedSemantic::Reset()
            {
                semantic_ = Semantic::Undefined;
                index_ = 0;
                user_defined_.clear();
            }

            void IndexedSemantic::ResetIndex(int index_a)
            {
                index_ = index_a;
            }

            void IndexedSemantic::MakeUserDefined(
                    const std::string& semantic_name_a)
            {
                if (IsSystemSemantic(semantic_))
                {
                    user_defined_ = (semantic_name_a.empty()
                                            ? SemanticToString(semantic_)
                                            : semantic_name_a);
                    semantic_ = Semantic::UserDefined;
                }
            }

            bool IsSystemSemantic(const Semantic t_a)
            {
                return (t_a >= Semantic::ClipDistance
                        && t_a <= Semantic::ViewportArrayIndex);
            }

            bool IsUserSemantic(const Semantic t_a)
            {
                return (t_a == Semantic::UserDefined);
            }

            std::string SemanticToString(const Semantic t_a)
            {
#define CASE_TO_STRING(IDENT)                                                  \
    case Semantic::IDENT:                                                      \
        return ("SV_" + std::string(#IDENT))

                switch (t_a)
                {
                case Semantic::Undefined:
                    return R_Undefined;
                case Semantic::UserDefined:
                    return R_UserDefined;

                    CASE_TO_STRING(ClipDistance);
                    CASE_TO_STRING(CullDistance);
                    CASE_TO_STRING(Coverage);
                    CASE_TO_STRING(Depth);
                    CASE_TO_STRING(DepthGreaterEqual);
                    CASE_TO_STRING(DepthLessEqual);
                    CASE_TO_STRING(DispatchThreadID);
                    CASE_TO_STRING(DomainLocation);
                    CASE_TO_STRING(FragCoord);
                    CASE_TO_STRING(GroupID);
                    CASE_TO_STRING(GroupIndex);
                    CASE_TO_STRING(GroupThreadID);
                    CASE_TO_STRING(GSInstanceID);
                    CASE_TO_STRING(InnerCoverage);
                    CASE_TO_STRING(InsideTessFactor);
                    CASE_TO_STRING(InstanceID);
                    CASE_TO_STRING(IsFrontFace);
                    CASE_TO_STRING(OutputControlPointID);
                    CASE_TO_STRING(PointSize);
                    CASE_TO_STRING(PrAzitiveID);
                    CASE_TO_STRING(RenderTargetArrayIndex);
                    CASE_TO_STRING(SampleIndex);
                    CASE_TO_STRING(StencilRef);
                    CASE_TO_STRING(Target);
                    CASE_TO_STRING(TessFactor);
                    CASE_TO_STRING(VertexID);
                    CASE_TO_STRING(VertexPosition);
                    CASE_TO_STRING(ViewportArrayIndex);
                }
                return "";
#undef CASE_TO_STRING
            }

            namespace DetailsMap0
            {
                using T = Reflection::Filter;

                static const std::map<std::string, T> g_map_filter_ 
                {
                        {"MIN_MAG_MIP_POINT", T::MinMagMipPoint},
                        {"MIN_MAG_POINT_MIP_LINEAR", T::MinMagPointMipLinear},
                        {"MIN_POINT_MAG_LINEAR_MIP_POINT",
                         T::MinPointMagLinearMipPoint},
                        {"MIN_POINT_MAG_MIP_LINEAR", T::MinPointMagMipLinear},
                        {"MIN_LINEAR_MAG_MIP_POINT", T::MinLinearMagMipPoint},
                        {"MIN_LINEAR_MAG_POINT_MIP_LINEAR",
                         T::MinLinearMagPointMipLinear},
                        {"MIN_MAG_LINEAR_MIP_POINT", T::MinMagLinearMipPoint},
                        {"MIN_MAG_MIP_LINEAR", T::MinMagMipLinear},
                        {"ANISOTROPIC", T::Anisotropic},
                        {"COMPARISON_MIN_MAG_MIP_POINT",
                         T::ComparisonMinMagMipPoint},
                        {"COMPARISON_MIN_MAG_POINT_MIP_LINEAR",
                         T::ComparisonMinMagPointMipLinear},
                        {"COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT",
                         T::ComparisonMinPointMagLinearMipPoint},
                        {"COMPARISON_MIN_POINT_MAG_MIP_LINEAR",
                         T::ComparisonMinPointMagMipLinear},
                        {"COMPARISON_MIN_LINEAR_MAG_MIP_POINT",
                         T::ComparisonMinLinearMagMipPoint},
                        {"COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR",
                         T::ComparisonMinLinearMagPointMipLinear},
                        {"COMPARISON_MIN_MAG_LINEAR_MIP_POINT",
                         T::ComparisonMinMagLinearMipPoint},
                        {"COMPARISON_MIN_MAG_MIP_LINEAR",
                         T::ComparisonMinMagMipLinear},
                        {"COMPARISON_ANISOTROPIC", T::ComparisonAnisotropic},
                        {"MINAzUM_MIN_MAG_MIP_POINT", T::MinAzumMinMagMipPoint},
                        {"MINAzUM_MIN_MAG_POINT_MIP_LINEAR",
                         T::MinAzumMinMagPointMipLinear},
                        {"MINAzUM_MIN_POINT_MAG_LINEAR_MIP_POINT",
                         T::MinAzumMinPointMagLinearMipPoint},
                        {"MINAzUM_MIN_POINT_MAG_MIP_LINEAR",
                         T::MinAzumMinPointMagMipLinear},
                        {"MINAzUM_MIN_LINEAR_MAG_MIP_POINT",
                         T::MinAzumMinLinearMagMipPoint},
                        {"MINAzUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR",
                         T::MinAzumMinLinearMagPointMipLinear},
                        {"MINAzUM_MIN_MAG_LINEAR_MIP_POINT",
                         T::MinAzumMinMagLinearMipPoint},
                        {"MINAzUM_MIN_MAG_MIP_LINEAR",
                         T::MinAzumMinMagMipLinear},
                        {"MINAzUM_ANISOTROPIC", T::MinAzumAnisotropic},
                        {"MAXAzUM_MIN_MAG_MIP_POINT", T::MaxAzumMinMagMipPoint},
                        {"MAXAzUM_MIN_MAG_POINT_MIP_LINEAR",
                         T::MaxAzumMinMagPointMipLinear},
                        {"MAXAzUM_MIN_POINT_MAG_LINEAR_MIP_POINT",
                         T::MaxAzumMinPointMagLinearMipPoint},
                        {"MAXAzUM_MIN_POINT_MAG_MIP_LINEAR",
                         T::MaxAzumMinPointMagMipLinear},
                        {"MAXAzUM_MIN_LINEAR_MAG_MIP_POINT",
                         T::MaxAzumMinLinearMagMipPoint},
                        {"MAXAzUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR",
                         T::MaxAzumMinLinearMagPointMipLinear},
                        {"MAXAzUM_MIN_MAG_LINEAR_MIP_POINT",
                         T::MaxAzumMinMagLinearMipPoint},
                        {"MAXAzUM_MIN_MAG_MIP_LINEAR",
                         T::MaxAzumMinMagMipLinear},
                        {"MAXAzUM_ANISOTROPIC", T::MaxAzumAnisotropic},
                };
            } // namespace DetailsMap0

            std::string FilterToString(const Reflection::Filter t_a)
            {
                return TypeToStringSecondary(DetailsMap0::g_map_filter_,
                                             t_a,
                                             "SamplerState::Filter");
            }

            Reflection::Filter StringToFilter(const std::string& s_a)
            {
                return StringToTypeSecondary(DetailsMap0::g_map_filter_,
                                             s_a,
                                             "SamplerState::Filter");
            }

            namespace DetailsMap1
            {
                using T = Reflection::TextureAddressMode;

                static const std::map<std::string, T> g_map_tex_address_mode_ 
                {
                        {"WRAP", T::Wrap},
                        {"MIRROR", T::Mirror},
                        {"CLAMP", T::Clamp},
                        {"BORDER", T::Border},
                        {"MIRROR_ONCE", T::MirrorOnce},
                };
            } // namespace DetailsMap1

            std::string TexAddressModeToString(
                    const Reflection::TextureAddressMode t_a)
            {
                return TypeToStringSecondary(
                        DetailsMap1::g_map_tex_address_mode_,
                        t_a,
                        "SamplerState::TextureAddressMode");
            }

            Reflection::TextureAddressMode StringToTexAddressMode(
                    const std::string& s_a)
            {
                return StringToTypeSecondary(
                        DetailsMap1::g_map_tex_address_mode_,
                        s_a,
                        "SamplerState::TextureAddressMode");
            }

            namespace DetailsMap2
            {
                using T = Reflection::ComparisonFunc;

                static const std::map<std::string, T> g_map_compare_func_ 
                {
                        {"NEVER", T::Never},
                        {"LESS", T::Less},
                        {"EQUAL", T::Equal},
                        {"LESS_EQUAL", T::LessEqual},
                        {"GREATER", T::Greater},
                        {"NOT_EQUAL", T::NotEqual},
                        {"GREATER_EQUAL", T::GreaterEqual},
                        {"ALWAYS", T::Always},
                };
            } // namespace DetailsMap2

            std::string CompareFuncToString(const Reflection::ComparisonFunc t_a)
            {
                return TypeToStringSecondary(DetailsMap2::g_map_compare_func_,
                                             t_a,
                                             "SamplerState::ComparisonFunc");
            }

            Reflection::ComparisonFunc StringToCompareFunc(const std::string& s_a)
            {
                return StringToTypeSecondary(DetailsMap2::g_map_compare_func_,
                                             s_a,
                                             "SamplerState::ComparisonFunc");
            }

            std::string ResourceTypeToString(const Reflection::ResourceType t_a)
            {
#define CASE_TO_STRING(IDENT)                                                  \
    case Reflection::ResourceType::IDENT:                                      \
        return #IDENT

                switch (t_a)
                {
                case Reflection::ResourceType::Undefined:
                    return R_Undefined;

                    CASE_TO_STRING(Texture1D);
                    CASE_TO_STRING(Texture2D);
                    CASE_TO_STRING(Texture3D);
                    CASE_TO_STRING(TextureCube);
                    CASE_TO_STRING(Texture1DArray);
                    CASE_TO_STRING(Texture2DArray);
                    CASE_TO_STRING(TextureCubeArray);
                    CASE_TO_STRING(Texture2DMS);
                    CASE_TO_STRING(Texture2DMSArray);

                    CASE_TO_STRING(RWTexture1D);
                    CASE_TO_STRING(RWTexture2D);
                    CASE_TO_STRING(RWTexture3D);
                    CASE_TO_STRING(RWTextureCube);
                    CASE_TO_STRING(RWTexture1DArray);
                    CASE_TO_STRING(RWTexture2DArray);
                    CASE_TO_STRING(RWTextureCubeArray);
                    CASE_TO_STRING(RWTexture2DMS);
                    CASE_TO_STRING(RWTexture2DMSArray);

                    CASE_TO_STRING(Sampler1D);
                    CASE_TO_STRING(Sampler2D);
                    CASE_TO_STRING(Sampler3D);
                    CASE_TO_STRING(SamplerCube);
                    CASE_TO_STRING(Sampler1DArray);
                    CASE_TO_STRING(Sampler2DArray);
                    CASE_TO_STRING(SamplerCubeArray);
                    CASE_TO_STRING(Sampler2DMS);
                    CASE_TO_STRING(Sampler2DMSArray);
                    CASE_TO_STRING(Sampler2DRect);

                    CASE_TO_STRING(Buffer);
                    CASE_TO_STRING(ByteAddressBuffer);
                    CASE_TO_STRING(StructuredBuffer);
                    CASE_TO_STRING(AppendStructuredBuffer);
                    CASE_TO_STRING(ConsumeStructuredBuffer);

                    CASE_TO_STRING(RWBuffer);
                    CASE_TO_STRING(RWByteAddressBuffer);
                    CASE_TO_STRING(RWStructuredBuffer);

                    CASE_TO_STRING(ConstantBuffer);
                    CASE_TO_STRING(TextureBuffer);
                    CASE_TO_STRING(SamplerState);
                    CASE_TO_STRING(SamplerComparisonState);
                }
                return "";
#undef CASE_TO_STRING
            }

            Reflection::ResourceType UniformBufferTypeToResourceType(
                    const UniformBufferType t_a)
            {
                using Src = UniformBufferType;
                using Dst = Reflection::ResourceType;
                switch (t_a)
                {
                case Src::ConstantBuffer:
                    return Dst::ConstantBuffer;
                case Src::TextureBuffer:
                    return Dst::TextureBuffer;
                default:
                    return Dst::Undefined;
                }
            }

            Reflection::ResourceType BufferTypeToResourceType(
                    const BufferType t_a)
            {
                using Src = BufferType;
                using Dst = Reflection::ResourceType;
                switch (t_a)
                {
                case Src::Buffer:
                    return Dst::Buffer;
                case Src::StructuredBuffer:
                    return Dst::StructuredBuffer;
                case Src::ByteAddressBuffer:
                    return Dst::ByteAddressBuffer;
                case Src::RWBuffer:
                    return Dst::RWBuffer;
                case Src::RWStructuredBuffer:
                    return Dst::RWStructuredBuffer;
                case Src::RWByteAddressBuffer:
                    return Dst::RWByteAddressBuffer;
                case Src::AppendStructuredBuffer:
                    return Dst::AppendStructuredBuffer;
                case Src::ConsumeStructuredBuffer:
                    return Dst::ConsumeStructuredBuffer;
                case Src::RWTexture1D:
                    return Dst::RWTexture1D;
                case Src::RWTexture1DArray:
                    return Dst::RWTexture1DArray;
                case Src::RWTexture2D:
                    return Dst::RWTexture2D;
                case Src::RWTexture2DArray:
                    return Dst::RWTexture2DArray;
                case Src::RWTexture3D:
                    return Dst::RWTexture3D;
                case Src::Texture1D:
                    return Dst::Texture1D;
                case Src::Texture1DArray:
                    return Dst::Texture1DArray;
                case Src::Texture2D:
                    return Dst::Texture2D;
                case Src::Texture2DArray:
                    return Dst::Texture2DArray;
                case Src::Texture3D:
                    return Dst::Texture3D;
                case Src::TextureCube:
                    return Dst::TextureCube;
                case Src::TextureCubeArray:
                    return Dst::TextureCubeArray;
                case Src::Texture2DMS:
                    return Dst::Texture2DMS;
                case Src::Texture2DMSArray:
                    return Dst::Texture2DMSArray;
                default:
                    return Dst::Undefined;
                }
            }

            Reflection::ResourceType SamplerTypeToResourceType(
                    const SamplerType t_a)
            {
                using Src = SamplerType;
                using Dst = Reflection::ResourceType;
                switch (t_a)
                {
                case Src::SamplerState:
                    return Dst::SamplerState;
                case Src::SamplerComparisonState:
                    return Dst::SamplerComparisonState;
                default:
                    return Dst::Undefined;
                }
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel