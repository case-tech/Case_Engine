struct VSInput {
    float3 pos : POSITION;
    float2 tex : TEXCOORD0;
};

struct VSOutput {
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

VSOutput main(VSInput input) {
    VSOutput output;
    output.pos = float4(input.pos, 1.0);
    output.tex = input.tex;
    return output;
}
