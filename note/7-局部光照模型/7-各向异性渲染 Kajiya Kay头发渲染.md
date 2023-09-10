# 各向异性渲染 - Kajiya Kay头发渲染

## 介绍

基于之前的各向异性得结果，我们可以再推广到头发渲染。Kajiya Kay头发渲染是一种经验模型。它把头发模型为一条一条细管。然后利用上一章的数学基础，但当然Kajiya Kay也加了几个细节。包括多Primary highlight, Second HighLight, Shift Tangent, 的各向异性得结果，我们可以再推广到头发渲染。Kajiya Kay头发渲染是一种经验模型。它把头发模型为一条一条细管。然后利用上一章的数学基础，但当然Kajiya Kay也加了几个细节。包括多Primary highlight, Second HighLight, Shift Tangent, 等概念。

- **各向异性高光**
- **漫反射**
- **Primary & Secondary 高光**
- **Shift Tangent**
- **Shift Tangent Map**
- **AmbientOcclusion**
- **最终效果**
- **代码**

## 各向异性高光

因为上一章已经说明了，高光部分 $I_{s anisotopic}$ 由以下公式得到
$$
I_{s anisotopic} = L_i * k_s * (\vec{N_c} \cdot \vec{H})^n = L_i * k_s * (\sqrt{1-(\vec{T} \cdot \vec{H})^2})^n
$$


```glsl
vec3 shiftTangent(vec3 T, vec3 N, float shift)
{
    return normalize(T + shift * N);
}

float hairStrandSpecular(vec3 T, vec3 V, vec3 L, float specPower)
{
    vec3 H = normalize(V + L);

    float dotHT = dot(T, H);
    float sinTH = sqrt(1 - dotHT * HdotT);

    return saturate(pow(sinTH, specPower)) * _SpecularScale;
}

vec4 getSpecular(vec4 lightColor0,
    vec4 primaryColor, float primaryShift,
    vec4 secondaryColor, float secondaryShift,
    vec3 N, vec3 T, vec3 V, vec3 L, float specPower, vec2 uv)
{
    vec4 specular = vec4(0.0, 0.0, 0.0, 0.0);
    specular += primaryColor * hairStrandSpecular(T, V, L, specPower) * _SpecularScale;;

    return specular;
}

vec4 frag(v2f i) : SV_Target
{
    vec3 N = normalize(i.normal);
    vec3 T = normalize(i.tangent);
    vec3 B = normalize(i.binormal);
    vec3 V = normalize(UnityWorldSpaceViewDir(i.pos));
    vec3 L = normalize(UnityWorldSpaceLightDir(i.pos));
    vec3 H = normalize(L + V);

    vec4 specular = getSpecular(_LightColor0, _PrimaryColor, _PrimaryShift, _SecondaryColor, _SecondaryShift, N, B, V, L, _specPower, i.uv);

    vec4 col = (specular);
    col.a = 1.0f;

    return col;
}
```

## 漫反射

Kajiya-Kay 的diffuse可以简单的使用N.L
$$
I_d = L_i * k_d * (\vec{N_c} \cdot \vec{L})
$$


```glsl
vec4 getAmbientAndDiffuse(vec4 lightColor0, vec4 diffuseColor, vec3 N, vec3 L, vec2 uv)
{
    return (lightColor0 * diffuseColor * saturate(dot(N, L)) + vec4(0.2, 0.2, 0.2, 1.0))
        * tex2D(_HairTex, uv);
}
```

## **Primary & Secondary 高光**

大家可以看一下下图的BaseColor，Primary 及 Secondary 高光。Primary高光是偏白的。然后Secondary 高光带点BaseColor。之后主色调Base Color。

![img](https://pic3.zhimg.com/80/v2-a7aeb7a0bb36fccd2d48e210e544d48a_720w.webp)

```glsl
vec4 getSpecular(vec4 lightColor0,
    vec4 primaryColor, float primaryShift,
    vec4 secondaryColor, float secondaryShift,
    vec3 N, vec3 T, vec3 V, vec3 L, float specPower, vec2 uv)
{
    vec4 specular = vec4(0.0, 0.0, 0.0, 0.0);
    specular += primaryColor * hairStrandSpecular(T, V, L, specPower) * _SpecularScale;;
    specular += secondaryColor * hairStrandSpecular(T, V, L, specPower) * _SpecularScale;

    return specular;
}
```

## **Shift Tangent**

Kajiya-Kay模型有两个偏移切线的量，原因是改变切线头发高光也偏移。如果没有切线的偏移，我们最多只可以做到像下面没有发丝的感觉。

![img](https://pic3.zhimg.com/80/v2-99fd530e6f1fe26312f28a22247939fe_720w.webp)

我们将切线沿着法线方向偏移一个数量。

![img](https://pic3.zhimg.com/80/v2-20d652caf6ca6324572d41fc4a3a12ea_720w.webp)

## Shift Tangent Map

除了一个全局的偏移值，我们也可以控制每像素的偏移。我们叫这个map叫Shift Tangent Map

![img](https://pic3.zhimg.com/80/v2-29daee6f659354307b22ebea62e0114a_720w.webp)

**Ambient Occlusion**

最后，我们可以加上AO，这个AO的数值可以从vertex color或AO贴图得到。这个是用来模拟Base Color Shadowed。

![img](https://pic1.zhimg.com/80/v2-c272103a0d0f18c47a5817f291523194_720w.webp)

```glsl
vec4 getSpecular(vec4 lightColor0,
    vec4 primaryColor, float primaryShift,
    vec4 secondaryColor, float secondaryShift,
    vec3 N, vec3 T, vec3 V, vec3 L, float specPower, vec2 uv, float ambientOcc)
{
    vec4 specular = vec4(0.0, 0.0, 0.0, 0.0);
    specular += primaryColor * hairStrandSpecular(T, V, L, specPower) * _SpecularScale;;
    specular += secondaryColor * hairStrand(T, V, L, specPower) * _SpecularScale;

    specular *= ambientOcc;

    return specular ;
}

vec4 getAmbientAndDiffuse(vec4 lightColor0, vec4 diffuseColor, vec3 N, vec3 L, vec2 uv, float ambientOcc)
{
    float ambientDiffuse = (lightColor0 * diffuseColor * saturate(dot(N, L)) + vec4(0.2, 0.2, 0.2, 1.0))
        * tex2D(_HairTex, uv) * ambientOcc;

    return ambientDiffuse * ambientOcc;
}
```

## 最终效果

![img](https://pic1.zhimg.com/80/v2-9a807a778e5881d5e8d0970276664934_720w.webp)

## 代码

```glsl
vec3 shiftTangent(vec3 T, vec3 N, float shift)
{
    return normalize(T + shift * N);
}

float hairStrandSpecular(vec3 T, vec3 V, vec3 L, float specPower)
{
    vec3 H = normalize(V + L);

    float HdotT = dot(T, H);
    float sinTH = sqrt(1 - HdotT * HdotT);
    float dirAtten = smoothstep(-_SpecularWidth, 0, HdotT);

    return dirAtten * saturate(pow(sinTH, specPower)) * _SpecularScale;
}

vec4 getAmbientAndDiffuse(vec4 lightColor0, vec4 diffuseColor, vec3 N, vec3 L, vec2 uv)
{
    return (lightColor0 * diffuseColor * saturate(dot(N, L)) + vec4(0.2, 0.2, 0.2, 1.0))
        * tex2D(_HairTex, uv);
}

vec4 getSpecular(vec4 lightColor0,
    vec4 primaryColor, float primaryShift,
    vec4 secondaryColor, float secondaryShift,
    vec3 N, vec3 T, vec3 V, vec3 L, float specPower, vec2 uv)
{
    float shiftTex = tex2D(_SpecularShift, uv) - 0.5;

    vec3 t1 = shiftTangent(T, N, primaryShift + shiftTex);
    vec3 t2 = shiftTangent(T, N, secondaryShift + shiftTex);

    vec4 specular = vec4(0.0, 0.0, 0.0, 0.0);
    specular += primaryColor * hairStrandSpecular(t1, V, L, specPower) * _SpecularScale;;
    specular += secondaryColor * hairStrandSpecular(t2, V, L, specPower) * _SpecularScale;

    return specular;
}

vec4 frag(v2f i) : SV_Target
{
    vec3 N = normalize(i.normal);
    vec3 T = normalize(i.tangent);
    vec3 B = normalize(i.binormal);
    vec3 V = normalize(UnityWorldSpaceViewDir(i.pos));
    vec3 L = normalize(UnityWorldSpaceLightDir(i.pos));
    vec3 H = normalize(L + V);

    vec4 ambientdiffuse = getAmbientAndDiffuse(_LightColor0, _DiffuseColor, N, L, i.uv);
    vec4 specular = getSpecular(_LightColor0, _PrimaryColor, _PrimaryShift, _SecondaryColor, _SecondaryShift, N, B, V, L, _specPower, i.uv);

    vec4 col = (ambientdiffuse + specular);
    col.a = 1.0f;

    return col;
}
```

**代码Unity**

```glsl
Shader "Mac/HairShader2"
{
    Properties
    {
        _HairTex("Texture", 2D) = "white" {}
        _SpecularShift("Hair Shifted Texture", 2D) = "white" {}

        _DiffuseColor("DiffuseColor", Color) = (0.0, 0.0, 0.0, 0.0)

        _PrimaryColor("Specular1Color", Color) = (0.0, 0.0, 0.0, 0.0)
        _PrimaryShift("PrimaryShift", Range(-4, 4)) = 0.0
        _SecondaryColor("Specular2Color", Color) = (0.0, 0.0, 0.0, 0.0)
        _SecondaryShift("SecondaryShift", Range(-4, 4)) = 0.5
        
        _specPower("SpecularPower", Range(0, 50)) = 20
        _SpecularWidth("SpecularWidth", Range(0, 1)) = 0.5
        _SpecularScale("SpecularScale", Range(0, 1)) = 0.3
    }
    SubShader
    {
        Tags { "RenderType"="Transparent" }

        LOD 100

        Pass
        {
            Blend SrcAlpha OneMinusSrcAlpha

            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            // make fog work
            #pragma multi_compile_fog

            #include "UnityCG.cginc"
            #include "Lighting.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;
                float3 normal: NORMAL;
                float4 tangent: TANGENT;
            };

            struct v2f
            {
                float2 uv : TEXCOORD0;
                float4 vertex : SV_POSITION;
                float3 tangent : TEXCOORD1;
                float3 normal : TEXCOORD2;
                float3 binormal: TEXCOORD3;
                float3 pos : TEXCOORD4;
                
                UNITY_FOG_COORDS(1)
            };

            sampler2D _HairTex;
            float4 _HairTex_ST;
            sampler2D _SpecularShift;
            float4 _SpecularShift_ST;

            float4 _DiffuseColor;
            float4 _PrimaryColor;
            float _PrimaryShift;
            float4 _SecondaryColor;
            float _SecondaryShift;

            float _specPower;
            float _SpecularWidth;
            float _SpecularScale;

            v2f vert (appdata v)
            {
                v2f o;
                
                UNITY_INITIALIZE_OUTPUT(v2f, o);
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = TRANSFORM_TEX(v.uv, _HairTex);

                o.normal = UnityObjectToWorldNormal(v.normal);
                o.tangent = UnityObjectToWorldDir(v.tangent);
                o.binormal = cross(v.normal, v.tangent) * v.tangent.w * unity_WorldTransformParams.w;

                o.pos = mul(unity_ObjectToWorld, v.vertex);

                UNITY_TRANSFER_FOG(o,o.vertex);
                return o;
            }

            fixed3 shiftTangent(fixed3 T, fixed3 N, fixed shift)
            {
                return normalize(T + shift * N);
            }

            fixed hairStrandSpecular(fixed3 T, fixed3 V, fixed3 L, fixed specPower)
            {
                fixed3 H = normalize(V + L);

                fixed HdotT = dot(T, H);
                fixed sinTH = sqrt(1 - HdotT * HdotT);
                fixed dirAtten = smoothstep(-_SpecularWidth, 0, HdotT);
                
                return dirAtten * saturate(pow(sinTH, specPower)) * _SpecularScale;
            }

            fixed4 getAmbientAndDiffuse(fixed4 lightColor0, fixed4 diffuseColor, fixed3 N, fixed3 L, fixed2 uv)
            {
                return (lightColor0 * diffuseColor * saturate(dot(N, L)) + fixed4(0.2, 0.2, 0.2, 1.0)) 
                          * tex2D(_HairTex, uv);
            }

            fixed4 getSpecular(fixed4 lightColor0, 
                               fixed4 primaryColor, fixed primaryShift,
                               fixed4 secondaryColor, fixed secondaryShift,
                               fixed3 N, fixed3 T, fixed3 V, fixed3 L, fixed specPower, fixed2 uv)
            {
                float shiftTex = tex2D(_SpecularShift, uv) - 0.5;

                fixed3 t1 = shiftTangent(T, N, primaryShift + shiftTex);
                fixed3 t2 = shiftTangent(T, N, secondaryShift + shiftTex);

                fixed4 specular = fixed4(0.0, 0.0, 0.0, 0.0);
                specular += primaryColor * hairStrandSpecular(t1, V, L, specPower) * _SpecularScale;;
                specular += secondaryColor * hairStrandSpecular(t2, V, L, specPower) * _SpecularScale;

                return specular;
            }

            fixed4 frag (v2f i) : SV_Target
            {
                fixed3 N = normalize(i.normal);
                fixed3 T = normalize(i.tangent);
                fixed3 B = normalize(i.binormal);
                fixed3 V = normalize(UnityWorldSpaceViewDir(i.pos));
                fixed3 L = normalize(UnityWorldSpaceLightDir(i.pos));
                fixed3 H = normalize(L + V);

                fixed4 ambientdiffuse = getAmbientAndDiffuse(_LightColor0, _DiffuseColor, N, L, i.uv);
                fixed4 specular = getSpecular(_LightColor0, _PrimaryColor, _PrimaryShift, _SecondaryColor, _SecondaryShift, N, B, V, L, _specPower, i.uv);
                
                fixed4 col = (ambientdiffuse + specular);
                col.a = 1.0f;
                
                UNITY_APPLY_FOG(i.fogCoord, col);
                return col;
            }
            ENDCG
        }
    }
}
```