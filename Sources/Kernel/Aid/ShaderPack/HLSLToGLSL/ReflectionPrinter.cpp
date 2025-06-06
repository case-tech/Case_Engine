#include "ReflectionPrinter.hpp"

#include "Report/ReportIdents.hpp"

#include <algorithm>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            ReflectionPrinter::ReflectionPrinter(std::ostream& output_a)
                : output_ {output_a}
            {}

            void ReflectionPrinter::PrintReflection(
                    const Reflection::ReflectionData& reflection_data_a,
                    bool referenced_only_a)
            {
                output_ << R_CodeReflection() << ':' << std::endl;
                indent_handler_.IncIndent();
                {
                    PrintReflectionObjects(reflection_data_a.macros_, "Macros");
                    PrintReflectionObjects(reflection_data_a.records_,
                                           "Structures",
                                           referenced_only_a);
                    PrintReflectionObjects(reflection_data_a.input_attributes_,
                                           "Input Attributes",
                                           referenced_only_a);
                    PrintReflectionObjects(reflection_data_a.output_attributes_,
                                           "Output Attributes",
                                           referenced_only_a);
                    PrintReflectionObjects(reflection_data_a.uniforms_,
                                           "Uniforms",
                                           referenced_only_a);
                    PrintReflectionObjects(reflection_data_a.resources_,
                                           "Resources",
                                           referenced_only_a);
                    PrintReflectionObjects(reflection_data_a.constant_buffers_,
                                           "Constant Buffers",
                                           referenced_only_a);
                    PrintReflectionObjects(reflection_data_a.sampler_states_,
                                           "Sampler States",
                                           referenced_only_a);
                    PrintReflectionObjects(reflection_data_a.static_sampler_states_,
                                           "Static Sampler States");
                    PrintReflectionAttribute(reflection_data_a.num_threads_,
                                             "Number of Threads");
                }
                indent_handler_.DecIndent();
            }

            template <typename T>
            bool HasAnyReferencedObjects(const std::vector<T>& list_a)
            {
                auto it_ = std::find_if(list_a.begin(),
                                       list_a.end(),
                                       [](const T& entry) -> bool {
                                           return entry.referenced_;
                                       });
                return (it_ != list_a.end());
            }

            template <typename T>
            std::size_t GetMaxSlotLength(const std::vector<T>& container_a,
                                         int& max_slot_a)
            {
                for (const auto& entry_ : container_a)
                    max_slot_a = std::max(max_slot_a, entry_.slot_);
                return std::to_string(max_slot_a).size_();
            }

            template <typename T>
            std::size_t GetMaxSlotLength(const std::vector<T>& container_a,
                                         int& max_slot_a,
                                         bool referenced_only_a)
            {
                for (const auto& entry_ : container_a)
                {
                    if (!referenced_only_a || entry_.referenced_)
                        max_slot_a = std::max(max_slot_a, entry_.slot_);
                }
                return std::to_string(max_slot_a).size();
            }

            std::ostream& ReflectionPrinter::IndentOut()
            {
                output_ << indent_handler_.FullIndent();
                return output_;
            }

            void ReflectionPrinter::PrintReflectionObjects(
                    const std::vector<std::string>& idents_a,
                    const char* title_a)
            {
                IndentOut() << title_a << ':' << std::endl;
                ScopedIndent indent_ {indent_handler_};

                if (!idents_a.empty())
                {
                    for (const auto& i_ : idents_a)
                        IndentOut() << i_ << std::endl;
                } else
                    IndentOut() << "< none >" << std::endl;
            }

            void ReflectionPrinter::PrintFields(
                    const std::vector<Reflection::Field>& objects_a,
                    bool referenced_only_a)
            {
                if (!objects_a.empty()
                    && (!referenced_only_a || HasAnyReferencedObjects(objects_a)))
                {
                    for (const auto& obj_ : objects_a)
                    {
                        if (!referenced_only_a || obj_.referenced_)
                        {
                            output_ << indent_handler_.FullIndent();
                            output_ << obj_.name_ << " <Field";
                            if (obj_.size_ != ~0)
                                output_ << "(offset: " << obj_.offset_
                                        << ", size: " << obj_.size_ << ')';
                            output_ << '>' << std::endl;
                        }
                    }
                }
            }

            void ReflectionPrinter::PrintReflectionObjects(
                    const std::vector<Reflection::Record>& objects_a,
                    const char* title_a,
                    bool referenced_only_a)
            {
                IndentOut() << title_a << ':' << std::endl;
                ScopedIndent indent_ {indent_handler_};

                if (!objects_a.empty()
                    && (!referenced_only_a || HasAnyReferencedObjects(objects_a)))
                {
                    for (const auto& obj_ : objects_a)
                    {
                        if (!referenced_only_a || obj_.referenced_)
                        {
                            output_ << indent_handler_.FullIndent();
                            output_ << obj_.name_ << " <Structure";
                            if (obj_.size_ != ~0)
                                output_ << "(size: " << obj_.size_
                                        << ", padding: " << obj_.padding_ << ')';
                            output_ << '>' << std::endl;

                            ScopedIndent indent1_ {indent_handler_};
                            PrintFields(obj_.fields_, referenced_only_a);
                        }
                    }
                } 
                else
                    IndentOut() << "< none >" << std::endl;
            }

            void ReflectionPrinter::PrintReflectionObjects(
                    const std::vector<Reflection::Attribute>& objects_a,
                    const char* title_a,
                    bool referenced_only_a)
            {
                IndentOut() << title_a << ':' << std::endl;
                ScopedIndent indent_ {indent_handler_};

                if (!objects_a.empty()
                    && (!referenced_only_a || HasAnyReferencedObjects(objects_a)))
                {
                    int max_slot_ = -1;
                    auto max_slot_len_ = GetMaxSlotLength(objects_a,
                                                       max_slot_,
                                                       referenced_only_a);

                    for (const auto& obj_ : objects_a)
                    {
                        if (!referenced_only_a || obj_.referenced_)
                        {
                            output_ << indent_handler_.FullIndent();
                            if (max_slot_ >= 0)
                            {
                                if (obj_.slot_ >= 0)
                                    output_ << std::string(
                                            max_slot_len_
                                                    - std::to_string(obj_.slot_)
                                                              .size(),
                                            ' ')
                                            << obj_.slot_ << ": ";
                                else
                                    output_ << std::string(max_slot_len_, ' ')
                                            << "  ";
                            }
                            output_ << obj_.name_ << std::endl;
                        }
                    }
                } 
                else
                    IndentOut() << "< none >" << std::endl;
            }

            void ReflectionPrinter::PrintReflectionObjects(
                    const std::vector<Reflection::Resource>& objects_a,
                    const char* title_a,
                    bool referenced_only_a)
            {
                IndentOut() << title_a << ':' << std::endl;
                ScopedIndent indent_ {indent_handler_};

                if (!objects_a.empty()
                    && (!referenced_only_a || HasAnyReferencedObjects(objects_a)))
                {
                    int max_slot_ = -1;
                    auto max_slot_len_ = GetMaxSlotLength(objects_a,
                                                       max_slot_,
                                                       referenced_only_a);

                    for (const auto& obj_ : objects_a)
                    {
                        if (!referenced_only_a || obj_.referenced_)
                        {
                            output_ << indent_handler_.FullIndent();
                            if (max_slot_ >= 0)
                            {
                                if (obj_.slot_ >= 0)
                                    output_ << std::string(
                                            max_slot_len_
                                                    - std::to_string(obj_.slot_)
                                                              .size(),
                                            ' ')
                                            << obj_.slot_ << ": ";
                                else
                                    output_ << std::string(max_slot_len_, ' ')
                                            << "  ";
                            }
                            output_ << obj_.name_ << " <" << ToString(obj_.type_)
                                    << '>' << std::endl;
                        }
                    }
                } else
                    IndentOut() << "< none >" << std::endl;
            }

            void ReflectionPrinter::PrintReflectionObjects(
                    const std::vector<Reflection::ConstantBuffer>& objects_a,
                    const char* title_a,
                    bool referenced_only_a)
            {
                IndentOut() << title_a << ':' << std::endl;
                ScopedIndent indent_ {indent_handler_};

                if (!objects_a.empty()
                    && (!referenced_only_a || HasAnyReferencedObjects(objects_a)))
                {
                    int max_slot_ = -1;
                    auto max_slot_len_ = GetMaxSlotLength(objects_a,
                                                       max_slot_,
                                                       referenced_only_a);

                    for (const auto& obj_ : objects_a)
                    {
                        if (!referenced_only_a || obj_.referenced_)
                        {
                            output_ << indent_handler_.FullIndent();
                            if (max_slot_ >= 0)
                            {
                                if (obj_.slot_ >= 0)
                                    output_ << std::string(
                                            max_slot_len_
                                                    - std::to_string(obj_.slot_)
                                                              .size(),
                                            ' ')
                                            << obj_.slot_ << ": ";
                                else
                                    output_ << std::string(max_slot_len_, ' ')
                                            << "  ";
                            }
                            output_ << obj_.name_ << " <" << ToString(obj_.type_);
                            if (obj_.size_ != ~0)
                                output_ << "(size: " << obj_.size_
                                        << ", padding: " << obj_.padding_ << ')';
                            output_ << '>' << std::endl;

                            ScopedIndent indent1_ {indent_handler_};
                            PrintFields(obj_.fields_, referenced_only_a);
                        }
                    }
                } 
                else
                    IndentOut() << "< none >" << std::endl;
            }

            void ReflectionPrinter::PrintReflectionObjects(
                    const std::vector<Reflection::SamplerState>& objects_a,
                    const char* title_a,
                    bool referenced_only_a)
            {
                IndentOut() << title_a << ':' << std::endl;
                ScopedIndent indent_ {indent_handler_};

                if (!objects_a.empty()
                    && (!referenced_only_a || HasAnyReferencedObjects(objects_a)))
                {
                    int max_slot_ = -1;
                    auto max_slot_len_ = GetMaxSlotLength(objects_a,
                                                       max_slot_,
                                                       referenced_only_a);

                    for (const auto& obj_ : objects_a)
                    {
                        if (!referenced_only_a || obj_.referenced_)
                        {
                            output_ << indent_handler_.FullIndent();
                            if (max_slot_ >= 0)
                            {
                                if (obj_.slot_ >= 0)
                                    output_ << std::string(
                                            max_slot_len_
                                                    - std::to_string(obj_.slot_)
                                                              .size(),
                                            ' ')
                                            << obj_.slot_ << ": ";
                                else
                                    output_ << std::string(max_slot_len_, ' ')
                                            << "  ";
                            }
                            output_ << obj_.name_ << std::endl;
                        }
                    }
                } 
                else
                    IndentOut() << "< none >" << std::endl;
            }

            void ReflectionPrinter::PrintReflectionObjects(
                    const std::vector<Reflection::StaticSamplerState>&
                            sampler_states_a,
                    const char* title_a)
            {
                IndentOut() << title_a << ':' << std::endl;
                ScopedIndent indent_ {indent_handler_};

                if (!sampler_states_a.empty())
                {
                    for (const auto& sampler_ : sampler_states_a)
                    {
                        IndentOut() << sampler_.name_ << std::endl;
                        indent_handler_.IncIndent();
                        {
                            const auto& desc_ = sampler_.desc_;
                            const auto& brd_col_ = desc_.border_color_;
                            IndentOut() << "AddressU       = "
                                        << ToString(desc_.address_u_) << std::endl;
                            IndentOut() << "AddressV       = "
                                        << ToString(desc_.address_v_) << std::endl;
                            IndentOut() << "AddressW       = "
                                        << ToString(desc_.address_w_) << std::endl;
                            IndentOut()
                                    << "BorderColor    = { " << brd_col_[0]
                                    << ", " << brd_col_[1] << ", " << brd_col_[2]
                                    << ", " << brd_col_[3] << " }" << std::endl;
                            IndentOut() << "ComparisonFunc = "
                                        << ToString(desc_.comparison_func_)
                                        << std::endl;
                            IndentOut() << "Filter         = "
                                        << ToString(desc_.filter_) << std::endl;
                            IndentOut()
                                    << "MaxAnisotropy  = " << desc_.max_anisotropy_
                                    << std::endl;
                            IndentOut() << "MaxLOD         = " << desc_.max_lod_
                                        << std::endl;
                            IndentOut() << "MinLOD         = " << desc_.min_lod_
                                        << std::endl;
                            IndentOut()
                                    << "MipLODBias     = " << desc_.mip_lod_bias_
                                    << std::endl;
                        }
                        indent_handler_.DecIndent();
                    }
                } 
                else
                    IndentOut() << "< none >" << std::endl;
            }

            void ReflectionPrinter::PrintReflectionAttribute(
                    const Reflection::NumThreads& num_threads_a,
                    const char* title_a)
            {
                if (num_threads_a.x_ > 0 || num_threads_a.y_ > 0 || num_threads_a.z_ > 0)
                {
                    IndentOut() << title_a << ':' << std::endl;
                    ScopedIndent indent_ {indent_handler_};

                    IndentOut() << "X = " << num_threads_a.x_ << std::endl;
                    IndentOut() << "Y = " << num_threads_a.y_ << std::endl;
                    IndentOut() << "Z = " << num_threads_a.z_ << std::endl;
                }
            }
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel