// Copyright (c) 2025 Case Technologies

#pragma once
#include "../Reflection.hpp"

#include <set>
#include <string>
#include <vector>

namespace CE_Kernel
{
    namespace Aid
    {
        namespace ShaderPack
        {
            class Token;

            enum class AssignOp
            {
                Undefined,

                Set,
                Add,
                Sub, 
                Mul, 
                Div, 
                Mod,
                LShift, 
                RShift, 
                Or,
                And,
                Xor, 
            };

            std::string AssignOpToString(const AssignOp o_a);
            AssignOp StringToAssignOp(const std::string& s_a);

            bool IsBitwiseOp(const AssignOp o_a);

            enum class BinaryOp
            {
                Undefined,

                LogicalAnd,
                LogicalOr,
                Or, 
                Xor, 
                And,  
                LShift,  
                RShift,  
                Add, 
                Sub, 
                Mul, 
                Div, 
                Mod, 
                Equal,   
                NotEqual, 
                Less,  
                Greater,  
                LessEqual,  
                GreaterEqual, 
            };

            std::string BinaryOpToString(const BinaryOp o_a);
            BinaryOp StringToBinaryOp(const std::string& s_a);
            BinaryOp AssignOpToBinaryOp(const AssignOp op_a);

            bool IsLogicalOp(const BinaryOp o_a);
            bool IsBitwiseOp(const BinaryOp o_a);
            bool IsCompareOp(const BinaryOp o_a);
            bool IsBooleanOp(const BinaryOp o_a);

            enum class UnaryOp
            {
                Undefined,

                LogicalNot, 
                Not, 
                Nop,
                Negate, 
                Inc,
                Dec,   
            };

            std::string UnaryOpToString(const UnaryOp o_a);
            UnaryOp StringToUnaryOp(const std::string& s_a);

            bool IsLogicalOp(const UnaryOp o_a);
            bool IsBitwiseOp(const UnaryOp o_a);
            bool IsLValueOp(const UnaryOp o_a);

            enum class CtrlTransfer
            {
                Undefined,

                Break,
                Continue,
                Discard,
            };

            std::string CtrlTransformToString(const CtrlTransfer ct_a);
            CtrlTransfer StringToCtrlTransfer(const std::string& s_a);

            enum class DataType
            {
                Undefined,
                String,  
                Bool,  
                Int,
                UInt, 
                Half,
                Float,
                Double,
                Bool2, 
                Bool3,
                Bool4,
                Int2,
                Int3, 
                Int4,
                UInt2,
                UInt3, 
                UInt4,
                Half2,  
                Half3,   
                Half4,  
                Float2,  
                Float3,  
                Float4,  
                Double2, 
                Double3, 
                Double4,  
                Bool2x2, 
                Bool2x3, 
                Bool2x4, 
                Bool3x2,
                Bool3x3,
                Bool3x4,
                Bool4x2, 
                Bool4x3, 
                Bool4x4,   
                Int2x2,
                Int2x3,
                Int2x4,
                Int3x2, 
                Int3x3, 
                Int3x4,  
                Int4x2, 
                Int4x3,
                Int4x4,
                UInt2x2,
                UInt2x3,  
                UInt2x4, 
                UInt3x2, 
                UInt3x3,  
                UInt3x4, 
                UInt4x2,  
                UInt4x3, 
                UInt4x4,
                Half2x2,  
                Half2x3, 
                Half2x4, 
                Half3x2, 
                Half3x3,  
                Half3x4, 
                Half4x2, 
                Half4x3, 
                Half4x4, 
                Float2x2,
                Float2x3,
                Float2x4,
                Float3x2, 
                Float3x3,
                Float3x4,
                Float4x2, 
                Float4x3, 
                Float4x4, 
                Double2x2, 
                Double2x3, 
                Double2x4, 
                Double3x2,
                Double3x3,
                Double3x4,
                Double4x2,
                Double4x3,
                Double4x4, 
            };

            struct MatrixSubscriptUsage
            {
                MatrixSubscriptUsage() = default;
                MatrixSubscriptUsage(const DataType data_type_in_a,
                                     const std::string& subscript_a);

                bool operator<(const MatrixSubscriptUsage& rhs_a) const;
                std::string IndicesToString() const;

                std::vector<std::pair<int, int>> indices_;
                DataType data_type_in_ = DataType::Undefined;
                DataType data_type_out_ = DataType::Undefined;
            };

            std::string DataTypeToString(const DataType t_a,
                                         bool use_template_syntax_a = false);

            unsigned int DataTypeSize(const DataType t_a);

            bool IsScalarType(const DataType t_a);
            bool IsVectorType(const DataType t_a);
            bool IsMatrixType(const DataType t_a);
            bool IsBooleanType(const DataType t_a);
            bool IsRealType(const DataType t_a);
            bool IsHalfRealType(const DataType t_a);
            bool IsSingleRealType(const DataType t_a);
            bool IsDoubleRealType(const DataType t_a);
            bool IsIntegralType(const DataType t_a);
            bool IsIntType(const DataType t_a);
            bool IsUIntType(const DataType t_a);

            int VectorTypeDAz(const DataType t_a);

            std::pair<int, int> MatrixTypeDAz(const DataType t_a);

            DataType BaseDataType(const DataType t_a);
            DataType VectorDataType(const DataType base_data_type_a,
                                    int vector_size_a);
            
            DataType MatrixDataType(const DataType base_data_type_a,
                                    int rows_a,
                                    int columns_a);
            
            DataType SubscriptDataType(
                    const DataType data_type_a,
                    const std::string& subscript_a,
                    std::vector<std::pair<int, int>>* indices_a = nullptr);
            
            DataType TokenToDataType(const Token& tkn_a);
            DataType DoubleToFloatDataType(const DataType data_type_a);
            unsigned int RemainingVectorSize(unsigned int vector_size_a,
                                             unsigned int alignment_a = 16u);

            bool AccumAlignedVectorSize(const DataType data_type_a,
                                        unsigned int& size_a,
                                        unsigned int& padding_a,
                                        unsigned int* offset_a = nullptr);

            enum class PrAzitiveType
            {
                Undefined,

                Point,    
                Line, 
                LineAdj,
                Triangle, 
                TriangleAdj,
            };

            enum class StorageClass
            {
                Undefined,

                Extern,
                Precise,
                Shared,
                GroupShared,
                Static,
                Volatile,
            };

            enum class InterpModifier
            {
                Undefined,
                Centroid, 
                Linear,
                NoInterpolation,
                NoPerspective, 
                Sample,   
            };

            enum class TypeModifier
            {
                Undefined,

                Const,
                RowMajor,
                ColumnMajor,

                SNorm,
                UNorm,
            };

            enum class UniformBufferType
            {
                Undefined,

                ConstantBuffer,
                TextureBuffer,
            };

            enum class BufferType
            {
                Undefined,
                Buffer,
                StructuredBuffer,
                ByteAddressBuffer,

                RWBuffer,
                RWStructuredBuffer,
                RWByteAddressBuffer,
                AppendStructuredBuffer,
                ConsumeStructuredBuffer,

                RWTexture1D,
                RWTexture1DArray,
                RWTexture2D,
                RWTexture2DArray,
                RWTexture3D,

                Texture1D,
                Texture1DArray,
                Texture2D,
                Texture2DArray,
                Texture3D,
                TextureCube,
                TextureCubeArray,
                Texture2DMS,
                Texture2DMSArray,

                GenericTexture, 
                InputPatch,
                OutputPatch,

                PointStream,
                LineStream,
                TriangleStream,

                GenericBuffer,
            };

            std::string BufferTypeToString(const BufferType t_a);

            bool IsStorageBufferType(const BufferType t_a);
            bool IsRWBufferType(const BufferType t_a);
            bool IsTextureBufferType(const BufferType t_a);
            bool IsTextureMSBufferType(const BufferType t_a);
            bool IsAzageBufferType(const BufferType t_a);
            bool IsRWAzageBufferType(const BufferType t_a);
            bool IsPatchBufferType(const BufferType t_a);
            bool IsStreamBufferType(const BufferType t_a);

            int GetBufferTypeTextureDAz(const BufferType t_a);
            enum class SamplerType
            {
                Undefined,
                Sampler1D,
                Sampler2D, 
                Sampler3D, 
                SamplerCube,    
                Sampler2DRect,     
                Sampler1DArray,    
                Sampler2DArray,     
                SamplerCubeArray,   
                SamplerBuffer,     
                Sampler2DMS,      
                Sampler2DMSArray,   
                Sampler1DShadow,    
                Sampler2DShadow,    
                SamplerCubeShadow,  
                Sampler2DRectShadow,
                Sampler1DArrayShadow,  
                                       
                Sampler2DArrayShadow,  
                SamplerCubeArrayShadow,
                                       
                SamplerState,          
                SamplerComparisonState, 
            };

            bool IsSamplerStateType(const SamplerType t_a);
            bool IsSamplerTypeShadow(const SamplerType t_a);
            bool IsSamplerTypeArray(const SamplerType t_a);

            int GetSamplerTypeTextureDAz(const SamplerType t_a);

            SamplerType TextureTypeToSamplerType(const BufferType t_a);
            SamplerType SamplerTypeToShadowSamplerType(const SamplerType t_a);

            enum class AzageLayoutFormat
            {
                Undefined,

                F32X4,     
                F32X2,   
                F32X1,
                F16X4,  
                F16X2, 
                F16X1, 
                F11R11G10B,

                UN32X4, 
                UN16X2,   
                UN16X1, 
                UN10R10G10B2A, 
                UN8X4,  
                UN8X2,   
                UN8X1,

                SN16X4,
                SN16X2, 
                SN16X1, 
                SN8X4,  
                SN8X2, 
                SN8X1,

                I32X4,
                I32X2, 
                I32X1, 
                I16X4, 
                I16X2,
                I16X1,
                I8X4,
                I8X2,
                I8X1,

                UI32X4,  
                UI32X2,  
                UI32X1,
                UI16X4, 
                UI16X2,  
                UI16X1,
                UI10R10G10B2A, 
                UI8X4,  
                UI8X2, 
                UI8X1,
            };

            DataType GetAzageLayoutFormatBaseType(
                    const AzageLayoutFormat format_a);
            
            AzageLayoutFormat DataTypeToAzageLayoutFormat(const DataType t_a);

            enum class RegisterType
            {
                Undefined,

                ConstantBuffer,    
                TextureBuffer, 
                BufferOffset,  
                Sampler, 
                UnorderedAccessView, 
            };

            RegisterType CharToRegisterType(char c_a);
            char RegisterTypeToChar(const RegisterType t_a);
            std::string RegisterTypeToString(const RegisterType t_a);

            enum class AttributeType
            {
                Undefined,
                Branch,                
                Call,                  
                Flatten,                 
                IfAll,                    
                IfAny,                   
                Isolate,                  
                Loop,                     
                MaxExports,                
                MaxInstructionCount,      
                MaxTempReg,              
                NoExpressionOptAzizations,
                Predicate,                
                PredicateBlock,          
                ReduceTempRegUsage,      
                RemoveUnusedInputs,       
                SampReg,                  
                Unroll,                 
                Unused,                  
                Hps,                     
                Domain,                  
                EarlyDepthStencil,      
                Instance,               
                MaxTessFactor,            
                MaxVertexCount,         
                NumThreads,              
                OutputControlPoints,     
                OutputTopology,           
                Partitioning,          
                PatchSize,          
                PatchConstantFunc,  
                Align,            
                Binding,           
                CW,                     
                CCW,              
                ColumnMajor,        
                Component,     
                DepthAny,             
                DepthGreater,         
                DepthLess,          
                DepthUnchanged,      
                EarlyFragmentTests,    
                EqualSpacing,           
                FractionalEvenSpacing,   
                FractionalOddSpacing,   
                Index,                
                Invocations,         
                Isolines,              
                Lines,               
                LinesAdjacency,      
                LineStrip,           
                LocalSizeX,        
                LocalSizeY,             
                LocalSizeZ,              
                Location,               
                MaxVertices,            
                OriginUpperLeft,        
                Offset,                    
                Packed,                   
                PixelCenterInteger,     
                Points,                  
                PointMode,             
                Quads,                 
                RowMajor,               
                Shared,                 
                Std140,                  
                Std430,                  
                Stream,                   
                Triangles,              
                TrianglesAdjacency,      
                TriangleStrip,           
                Vertices,                
                HfbBuffer,                
                HfbOffset,                
                HfbStride,                
            };

            bool IsShaderModel3AttributeType(const AttributeType t_a);
            bool IsShaderModel5AttributeType(const AttributeType t_a);
            bool IsHLSLAttributeType(const AttributeType t_a);
            bool IsGLSLAttributeType(const AttributeType t_a);

            enum class AttributeValue
            {
                Undefined,

                DomainTri,
                DomainQuad,
                DomainIsoline,

                OutputTopologyPoint,
                OutputTopologyLine,
                OutputTopologyTriangleCW,
                OutputTopologyTriangleCCW,

                PartitioningInteger,
                PartitioningPow2,
                PartitioningFractionalEven,
                PartitioningFractionalOdd,
            };

            bool IsAttributeValueDomain(const AttributeValue t_a);
            bool IsAttributeValueOutputTopology(const AttributeValue t_a);
            bool IsAttributeValuePartitioning(const AttributeValue t_a);
            bool IsAttributeValueTrianglePartitioning(const AttributeValue t_a);

            enum class Intrinsic
            {
                Undefined,
                Abort,          
                Abs,
                ACos,
                All,
                AllMemoryBarrier, 
                AllMemoryBarrierWithGroupSync, 
                Any, 
                AsDouble, 
                AsFloat, 
                ASin,  
                AsInt, 
                AsUInt_1, 
                AsUInt_3,
                ATan,   
                ATan2, 
                Ceil,
                CheckAccessFullyMapped,
                Clamp,
                Clip, 
                Cos,  
                CosH,  
                CountBits, 
                Cross,
                D3DCOLORtoUBYTE4,
                DDX, 
                DDXCoarse, 
                DDXFine, 
                DDY,  
                DDYCoarse, 
                DDYFine,
                Degrees,   
                Determinant, 
                DeviceMemoryBarrier,
                DeviceMemoryBarrierWithGroupSync, 
                Distance, 
                Dot,   
                Dst,  
                Equal,
                ErrorF,
                EvaluateAttributeAtCentroid, 
                EvaluateAttributeAtSample,
                EvaluateAttributeSnapped,
                Exp, 
                Exp2,
                F16toF32, 
                F32toF16,
                FaceForward, 
                FirstBitHigh,
                FirstBitLow, 
                Floor,
                FMA,
                FMod, 
                Frac, 
                FrExp,  
                FWidth,
                GetRenderTargetSampleCount, 
                GetRenderTargetSamplePosition, 
                GreaterThan,
                GreaterThanEqual, 
                GroupMemoryBarrier,
                GroupMemoryBarrierWithGroupSync,
                InterlockedAdd,   
                InterlockedAnd,   
                InterlockedCompareExchange, 
                InterlockedCompareStore, 
                InterlockedExchange, 
                InterlockedMax, 
                InterlockedMin,  
                InterlockedOr, 
                InterlockedXor,
                IsFinite, 
                IsInf,
                IsNaN,
                LdExp, 
                Length,  
                Lerp,  
                LessThan, 
                LessThanEqual,  
                Lit,
                Log,
                Log10, 
                Log2,
                MAD, 
                Max, 
                Min, 
                ModF,
                MSAD4,  
                Mul, 
                Normalize,
                NotEqual,
                Not, 
                Pow,
                PrintF,  
                Process2DQuadTessFactorsAvg,
                Process2DQuadTessFactorsMax, 
                Process2DQuadTessFactorsMin, 
                ProcessIsolineTessFactors, 
                ProcessQuadTessFactorsAvg, 
                ProcessQuadTessFactorsMax,
                ProcessQuadTessFactorsMin,
                ProcessTriTessFactorsAvg,
                ProcessTriTessFactorsMax,
                ProcessTriTessFactorsMin,
                Radians,
                Rcp,
                Reflect,  
                Refract, 
                ReverseBits, 
                Round, 
                RSqrt,  
                Saturate,   
                Sign, 
                Sin, 
                SinCos,   
                SinH,
                SmoothStep, 
                Sqrt,
                Step,
                Tan, 
                TanH,
                Transpose,
                Trunc,

                Tex1D_2,
                Tex1D_4,
                Tex1DBias,
                Tex1DGrad,
                Tex1DLod,
                Tex1DProj,
                Tex2D_2,
                Tex2D_4,
                Tex2DBias,
                Tex2DGrad,
                Tex2DLod,
                Tex2DProj,
                Tex3D_2,
                Tex3D_4,
                Tex3DBias,
                Tex3DGrad,
                Tex3DLod,
                Tex3DProj,
                TexCube_2,
                TexCube_4,
                TexCubeBias,
                TexCubeGrad,
                TexCubeLod,
                TexCubeProj,

                Texture_GetDAzensions,
                Texture_QueryLod, 
                Texture_QueryLodUnclamped, 

                Texture_Load_1,
                Texture_Load_2,
                Texture_Load_3,

                Texture_Sample_2,
                Texture_Sample_3, 
                Texture_Sample_4,
                Texture_Sample_5,

                Texture_SampleBias_3,
                Texture_SampleBias_4,
                Texture_SampleBias_5,
                Texture_SampleBias_6,
                Texture_SampleCmp_3,
                Texture_SampleCmp_4,
                Texture_SampleCmp_5,
                Texture_SampleCmp_6,
                Texture_SampleCmpLevelZero_3,
                Texture_SampleCmpLevelZero_4,
                Texture_SampleCmpLevelZero_5,
                Texture_SampleGrad_4,
                Texture_SampleGrad_5,
                Texture_SampleGrad_6,
                Texture_SampleGrad_7,
                Texture_SampleLevel_3,
                Texture_SampleLevel_4,
                Texture_SampleLevel_5,

                Texture_Gather_2, 
                Texture_GatherRed_2,  
                Texture_GatherGreen_2,
                Texture_GatherBlue_2,
                Texture_GatherAlpha_2, 
                Texture_Gather_3,
                Texture_Gather_4,
                Texture_GatherRed_3, 
                Texture_GatherRed_4,
                Texture_GatherGreen_3,
                Texture_GatherGreen_4,
                Texture_GatherBlue_3, 
                Texture_GatherBlue_4, 
                Texture_GatherAlpha_3,
                Texture_GatherAlpha_4, 

                Texture_GatherRed_6,
                Texture_GatherRed_7, 
                Texture_GatherGreen_6, 
                Texture_GatherGreen_7,
                Texture_GatherBlue_6,  
                Texture_GatherBlue_7, 
                Texture_GatherAlpha_6,
                Texture_GatherAlpha_7, 

                Texture_GatherCmp_3,   
                Texture_GatherCmpRed_3,
                Texture_GatherCmpGreen_3, 
                Texture_GatherCmpBlue_3,
                Texture_GatherCmpAlpha_3,
                Texture_GatherCmp_4, 
                Texture_GatherCmp_5, 
                Texture_GatherCmpRed_4,
                Texture_GatherCmpRed_5, 
                Texture_GatherCmpGreen_4,
                Texture_GatherCmpGreen_5, 
                Texture_GatherCmpBlue_4,
                Texture_GatherCmpBlue_5,
                Texture_GatherCmpAlpha_4,
                Texture_GatherCmpAlpha_5,
                Texture_GatherCmpRed_7, 
                Texture_GatherCmpRed_8,
                Texture_GatherCmpGreen_7,
                Texture_GatherCmpGreen_8,
                Texture_GatherCmpBlue_7, 
                Texture_GatherCmpBlue_8,
                Texture_GatherCmpAlpha_7,
                Texture_GatherCmpAlpha_8,

                StreamOutput_Append,  
                StreamOutput_RestartStrip, 

                Azage_Load,
                Azage_Store,  
                Azage_AtomicAdd, 
                Azage_AtomicAnd,
                Azage_AtomicOr,
                Azage_AtomicXor, 
                Azage_AtomicMin,
                Azage_AtomicMax,
                Azage_AtomicCompSwap, 
                Azage_AtomicExchange, 

                PackHalf2x16,
            };

            struct IntrinsicUsage
            {
                struct ArgumentList
                {
                    std::vector<DataType> arg_types_;

                    inline bool operator<(const ArgumentList& rhs_a) const
                    {
                        return (arg_types_ < rhs_a.arg_types_);
                    }
                };

                std::set<ArgumentList> arg_lists_;
            };

            bool IsGlobalIntrinsic(const Intrinsic t_a);
            bool IsTextureIntrinsic(const Intrinsic t_a);
            bool IsTextureGatherIntrisic(const Intrinsic t_a);
            bool IsTextureSampleIntrinsic(const Intrinsic t_a);
            bool IsTextureCompareIntrinsic(const Intrinsic t_a);
            bool IsTextureCompareLevelZeroIntrinsic(const Intrinsic t_a);
            bool IsTextureLoadIntrinsic(const Intrinsic t_a);
            bool IsStreamOutputIntrinsic(const Intrinsic t_a);
            bool IsAzageIntrinsic(const Intrinsic t_a);
            bool IsInterlockedIntristic(const Intrinsic t_a);

            Intrinsic CompareOpToIntrinsic(const BinaryOp op_a);
            Intrinsic InterlockedToAzageAtomicIntrinsic(const Intrinsic t_a);

            int GetGatherIntrinsicOffsetParamCount(const Intrinsic t_a);
            int GetGatherIntrinsicComponentIndex(const Intrinsic t_a);

            enum class Semantic
            {
                Undefined,

                UserDefined,  
                ClipDistance,
                CullDistance,
                Coverage,
                Depth, 
                DepthGreaterEqual,
                
                DepthLessEqual, 
                DispatchThreadID,
                
                DomainLocation, 
                FragCoord,
                GroupID,
                GroupIndex,
                GroupThreadID,
               
                GSInstanceID, 
                InnerCoverage,  
                InsideTessFactor,
                
                InstanceID, 
                IsFrontFace, 
                OutputControlPointID, 
                
                PointSize,
                PrAzitiveID,
                RenderTargetArrayIndex,
                
                SampleIndex,
                StencilRef, 
                
                Target,  
                TessFactor,
                VertexID,  
                
                VertexPosition,   
                ViewportArrayIndex, 
            };

            class IndexedSemantic
            {
            public:
                IndexedSemantic() = default;
                IndexedSemantic(const IndexedSemantic&) = default;
                IndexedSemantic& operator=(const IndexedSemantic&) = default;

                IndexedSemantic(Semantic semantic_a, int index_a = 0);
                IndexedSemantic(const std::string& user_defined_a);
                IndexedSemantic(const IndexedSemantic& rhs_a, int index_a);

                inline operator Semantic() const
                {
                    return semantic_;
                }

                bool operator<(const IndexedSemantic& rhs_a) const;
                bool IsValid() const;
                bool IsSystemValue() const;
                bool IsUserDefined() const;

                std::string ToString() const;

                void Reset();
                void ResetIndex(int index_a);

                void MakeUserDefined(const std::string& semantic_name_a = "");

                inline int Index() const
                {
                    return index_;
                }

            private:
                Semantic semantic_ = Semantic::Undefined;
                int index_ = 0;
                std::string user_defined_;
            };

            bool IsSystemSemantic(const Semantic t_a);
            bool IsUserSemantic(const Semantic t_a);

            std::string SemanticToString(const Semantic t_a);
            std::string FilterToString(const Reflection::Filter t_a);
            Reflection::Filter StringToFilter(const std::string& s_a);

            std::string TexAddressModeToString(
                    const Reflection::TextureAddressMode t_a);
            
            Reflection::TextureAddressMode StringToTexAddressMode(
                    const std::string& s_a);

            std::string CompareFuncToString(const Reflection::ComparisonFunc t_a);
            Reflection::ComparisonFunc StringToCompareFunc(
                    const std::string& s_a);

            std::string ResourceTypeToString(const Reflection::ResourceType t_a);

            Reflection::ResourceType UniformBufferTypeToResourceType(
                    const UniformBufferType t_a);
            Reflection::ResourceType BufferTypeToResourceType(
                    const BufferType t_a);
            Reflection::ResourceType SamplerTypeToResourceType(
                    const SamplerType t_a);
        } // namespace ShaderPack
    } // namespace Aid
} // namespace CE_Kernel
