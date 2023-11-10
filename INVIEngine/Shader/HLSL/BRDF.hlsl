// Copyright Epic Games, Inc. All Rights Reserved.

/*=============================================================================
	BRDF.usf: Bidirectional reflectance distribution functions.
=============================================================================*/

#pragma once

struct BxDFContext
{
	float NoV;
	float NoL;
	float VoL;
	float NoH;
	float VoH;
	float XoV;
	float XoL;
	float XoH;
	float YoV;
	float YoL;
	float YoH;
#if 1	// Shit code until we get member functions
};

void Init(inout BxDFContext Context, half3 N, half3 V, half3 L)
{
	Context.NoL = dot(N, L);
	Context.NoV = dot(N, V);
	Context.VoL = dot(V, L);
	float InvLenH = rsqrt(2 + 2 * Context.VoL);
	Context.NoH = saturate((Context.NoL + Context.NoV) * InvLenH);
	Context.VoH = saturate(InvLenH + InvLenH * Context.VoL);
	//NoL = saturate( NoL );
	//NoV = saturate( abs( NoV ) + 1e-5 );

	Context.XoV = 0.0f;
	Context.XoL = 0.0f;
	Context.XoH = 0.0f;
	Context.YoV = 0.0f;
	Context.YoL = 0.0f;
	Context.YoH = 0.0f;
}

void Init(inout BxDFContext Context, half3 N, half3 X, half3 Y, half3 V, half3 L)
{
	Context.NoL = dot(N, L);
	Context.NoV = dot(N, V);
	Context.VoL = dot(V, L);
	float InvLenH = rsqrt(2 + 2 * Context.VoL);
	Context.NoH = saturate((Context.NoL + Context.NoV) * InvLenH);
	Context.VoH = saturate(InvLenH + InvLenH * Context.VoL);
	//NoL = saturate( NoL );
	//NoV = saturate( abs( NoV ) + 1e-5 );

	Context.XoV = dot(X, V);
	Context.XoL = dot(X, L);
	Context.XoH = (Context.XoL + Context.XoV) * InvLenH;
	Context.YoV = dot(Y, V);
	Context.YoL = dot(Y, L);
	Context.YoH = (Context.YoL + Context.YoV) * InvLenH;
}

// [ de Carpentier 2017, "Decima Engine: Advances in Lighting and AA" ]
void SphereMaxNoH(inout BxDFContext Context, float SinAlpha, bool bNewtonIteration)
{
	if (SinAlpha > 0)
	{
		float CosAlpha = sqrt(1 - Pow2(SinAlpha));
	
		float RoL = 2 * Context.NoL * Context.NoV - Context.VoL;
		if (RoL >= CosAlpha)
		{
			Context.NoH = 1;
			Context.XoH = 0;
			Context.YoH = 0;
			Context.VoH = abs(Context.NoV);
		}
		else
		{
			float rInvLengthT = SinAlpha * rsqrt(1 - RoL * RoL);
			float NoTr = rInvLengthT * (Context.NoV - RoL * Context.NoL);
// Enable once anisotropic materials support area lights
#if 0
			float XoTr = rInvLengthT * ( Context.XoV - RoL * Context.XoL );
			float YoTr = rInvLengthT * ( Context.YoV - RoL * Context.YoL );
#endif
			float VoTr = rInvLengthT * (2 * Context.NoV * Context.NoV - 1 - RoL * Context.VoL);

			if (bNewtonIteration)
			{
				// dot( cross(N,L), V )
				float NxLoV = sqrt(saturate(1 - Pow2(Context.NoL) - Pow2(Context.NoV) - Pow2(Context.VoL) + 2 * Context.NoL * Context.NoV * Context.VoL));

				float NoBr = rInvLengthT * NxLoV;
				float VoBr = rInvLengthT * NxLoV * 2 * Context.NoV;

				float NoLVTr = Context.NoL * CosAlpha + Context.NoV + NoTr;
				float VoLVTr = Context.VoL * CosAlpha + 1 + VoTr;

				float p = NoBr * VoLVTr;
				float q = NoLVTr * VoLVTr;
				float s = VoBr * NoLVTr;

				float xNum = q * (-0.5 * p + 0.25 * VoBr * NoLVTr);
				float xDenom = p * p + s * (s - 2 * p) + NoLVTr * ((Context.NoL * CosAlpha + Context.NoV) * Pow2(VoLVTr) + q * (-0.5 * (VoLVTr + Context.VoL * CosAlpha) - 0.5));
				float TwoX1 = 2 * xNum / (Pow2(xDenom) + Pow2(xNum));
				float SinTheta = TwoX1 * xDenom;
				float CosTheta = 1.0 - TwoX1 * xNum;
				NoTr = CosTheta * NoTr + SinTheta * NoBr;
				VoTr = CosTheta * VoTr + SinTheta * VoBr;
			}

			Context.NoL = Context.NoL * CosAlpha + NoTr; // dot( N, L * CosAlpha + T * SinAlpha )
// Enable once anisotropic materials support area lights
#if 0
			Context.XoL = Context.XoL * CosAlpha + XoTr;
			Context.YoL = Context.YoL * CosAlpha + YoTr;
#endif
			Context.VoL = Context.VoL * CosAlpha + VoTr;

			float InvLenH = rsqrt(2 + 2 * Context.VoL);
			Context.NoH = saturate((Context.NoL + Context.NoV) * InvLenH);
// Enable once anisotropic materials support area lights
#if 0
			Context.XoH = ((Context.XoL + Context.XoV) * InvLenH);	// dot(X, (L+V)/|L+V|)
			Context.YoH = ((Context.YoL + Context.YoV) * InvLenH);
#endif
			Context.VoH = saturate(InvLenH + InvLenH * Context.VoL);
		}
	}
}
#else
	void Init( half3 N, half3 V, half3 L )
	{
		NoL = dot(N, L);
		NoV = dot(N, V);
		VoL = dot(V, L);
		float InvLenH = rsqrt( 2 + 2 * VoL );
		NoH = saturate( ( NoL + NoV ) * InvLenH );
		VoH = saturate( InvLenH + InvLenH * VoL );
		//NoL = saturate( NoL );
		//NoV = saturate( abs( NoV ) + 1e-5 );

		XoV = 0.0f;
		XoL = 0.0f;
		XoH = 0.0f;
		YoV = 0.0f;
		YoL = 0.0f;
		YoH = 0.0f;
	}

	void Init( half3 N, half3 X, half3 Y, half3 V, half3 L )
	{
		NoL = dot(N, L);
		NoV = dot(N, V);
		VoL = dot(V, L);
		float InvLenH = rsqrt( 2 + 2 * VoL );
		NoH = saturate( ( NoL + NoV ) * InvLenH );
		VoH = saturate( InvLenH + InvLenH * VoL );
		//NoL = saturate( NoL );
		//NoV = saturate( abs( NoV ) + 1e-5 );

		XoV = dot(X, V);
		XoL = dot(X, L);
		XoH = (XoL + XoV) * InvLenH;
		YoV = dot(Y, V);
		YoL = dot(Y, L);
		YoH = (YoL + YoV) * InvLenH;
	}

	// [ de Carpentier 2017, "Decima Engine: Advances in Lighting and AA" ]
	void SphereMaxNoH( float SinAlpha, bool bNewtonIteration )
	{
		float CosAlpha = sqrt( 1 - Pow2( SinAlpha ) );
	
		float RoL = 2 * NoL * NoV - VoL;
		if( RoL >= CosAlpha )
		{
			NoH = 1;
			XoH = 0;
			YoH = 0;
			VoH = abs( NoV );
		}
		else
		{
			float rInvLengthT = SinAlpha * rsqrt( 1 - RoL*RoL );
			float NoTr = rInvLengthT * ( NoV - RoL * NoL );
// Enable once anisotropic materials support area lights
#if 0
			float XoTr = rInvLengthT * ( XoV - RoL * XoL );
			float YoTr = rInvLengthT * ( YoV - RoL * YoL );
#endif
			float VoTr = rInvLengthT * ( 2 * NoV*NoV - 1 - RoL * VoL );

			if (bNewtonIteration && SinAlpha != 0)
			{
				// dot( cross(N,L), V )
				float NxLoV = sqrt( saturate( 1 - Pow2(NoL) - Pow2(NoV) - Pow2(VoL) + 2 * NoL * NoV * VoL ) );

				float NoBr = rInvLengthT * NxLoV;
				float VoBr = rInvLengthT * NxLoV * 2 * NoV;

				float NoLVTr = NoL * CosAlpha + NoV + NoTr;
				float VoLVTr = VoL * CosAlpha + 1   + VoTr;

				float p = NoBr   * VoLVTr;
				float q = NoLVTr * VoLVTr;
				float s = VoBr   * NoLVTr;

				float xNum = q * ( -0.5 * p + 0.25 * VoBr * NoLVTr );
				float xDenom = p*p + s * (s - 2*p) + NoLVTr * ( (NoL * CosAlpha + NoV) * Pow2(VoLVTr) + q * (-0.5 * (VoLVTr + VoL * CosAlpha) - 0.5) );
				float TwoX1 = 2 * xNum / ( Pow2(xDenom) + Pow2(xNum) );
				float SinTheta = TwoX1 * xDenom;
				float CosTheta = 1.0 - TwoX1 * xNum;
				NoTr = CosTheta * NoTr + SinTheta * NoBr;
				VoTr = CosTheta * VoTr + SinTheta * VoBr;
			}

			NoL = NoL * CosAlpha + NoTr;
// Enable once anisotropic materials support area lights
#if 0
			XoL = XoL * CosAlpha + XoTr;
			YoL = YoL * CosAlpha + YoTr;
#endif
			VoL = VoL * CosAlpha + VoTr;

			float InvLenH = rsqrt( 2 + 2 * VoL );
			NoH = saturate( ( NoL + NoV ) * InvLenH );
// Enable once anisotropic materials support area lights
#if 0
			XoH = ((XoL + XoV) * InvLenH);	// dot(X, (L+V)/|L+V|)
			YoH = ((YoL + YoV) * InvLenH);
#endif
			VoH = saturate( InvLenH + InvLenH * VoL );
		}
	}
};
#endif

// Physically based shading model
// parameterized with the below options
// [ Karis 2013, "Real Shading in Unreal Engine 4" slide 11 ]

// E = Random sample for BRDF.
// N = Normal of the macro surface.
// H = Normal of the micro surface.
// V = View vector going from surface's position towards the view's origin.
// L = Light ray direction

// D = Microfacet NDF
// G = Shadowing and masking
// F = Fresnel

// Vis = G / (4*NoL*NoV)
// f = Microfacet specular BRDF = D*G*F / (4*NoL*NoV) = D*Vis*F


float3 Diffuse_Lambert(float3 DiffuseColor)
{
	return DiffuseColor * (1 / PI);
}

// [Burley 2012, "Physically-Based Shading at Disney"]
float3 Diffuse_Burley(float3 DiffuseColor, float Roughness, float NoV, float NoL, float VoH)
{
	float FD90 = 0.5 + 2 * VoH * VoH * Roughness;
	float FdV = 1 + (FD90 - 1) * Pow5(1 - NoV);
	float FdL = 1 + (FD90 - 1) * Pow5(1 - NoL);
	return DiffuseColor * ((1 / PI) * FdV * FdL);
}

// [Gotanda 2012, "Beyond a Simple Physically Based Blinn-Phong Model in Real-Time"]
float3 Diffuse_OrenNayar(float3 DiffuseColor, float Roughness, float NoV, float NoL, float VoH)
{
	float a = Roughness * Roughness;
	float s = a; // / ( 1.29 + 0.5 * a );
	float s2 = s * s;
	float VoL = 2 * VoH * VoH - 1; // double angle identity
	float Cosri = VoL - NoV * NoL;
	float C1 = 1 - 0.5 * s2 / (s2 + 0.33);
	float C2 = 0.45 * s2 / (s2 + 0.09) * Cosri * (Cosri >= 0 ? rcp(max(NoL, NoV)) : 1);
	return DiffuseColor / PI * (C1 + C2) * (1 + Roughness * 0.5);
}

// [Gotanda 2014, "Designing Reflectance Models for New Consoles"]
float3 Diffuse_Gotanda(float3 DiffuseColor, float Roughness, float NoV, float NoL, float VoH)
{
	float a = Roughness * Roughness;
	float a2 = a * a;
	float F0 = 0.04;
	float VoL = 2 * VoH * VoH - 1; // double angle identity
	float Cosri = VoL - NoV * NoL;
#if 1
	float a2_13 = a2 + 1.36053;
	float Fr = (1 - (0.542026 * a2 + 0.303573 * a) / a2_13) * (1 - pow(1 - NoV, 5 - 4 * a2) / a2_13) * ((-0.733996 * a2 * a + 1.50912 * a2 - 1.16402 * a) * pow(1 - NoV, 1 + rcp(39 * a2 * a2 + 1)) + 1);
	//float Fr = ( 1 - 0.36 * a ) * ( 1 - pow( 1 - NoV, 5 - 4*a2 ) / a2_13 ) * ( -2.5 * Roughness * ( 1 - NoV ) + 1 );
	float Lm = (max(1 - 2 * a, 0) * (1 - Pow5(1 - NoL)) + min(2 * a, 1)) * (1 - 0.5 * a * (NoL - 1)) * NoL;
	float Vd = (a2 / ((a2 + 0.09) * (1.31072 + 0.995584 * NoV))) * (1 - pow(1 - NoL, (1 - 0.3726732 * NoV * NoV) / (0.188566 + 0.38841 * NoV)));
	float Bp = Cosri < 0 ? 1.4 * NoV * NoL * Cosri : Cosri;
	float Lr = (21.0 / 20.0) * (1 - F0) * (Fr * Lm + Vd + Bp);
	return DiffuseColor / PI * Lr;
#else
	float a2_13 = a2 + 1.36053;
	float Fr = ( 1 - ( 0.542026*a2 + 0.303573*a ) / a2_13 ) * ( 1 - pow( 1 - NoV, 5 - 4*a2 ) / a2_13 ) * ( ( -0.733996*a2*a + 1.50912*a2 - 1.16402*a ) * pow( 1 - NoV, 1 + rcp(39*a2*a2+1) ) + 1 );
	float Lm = ( max( 1 - 2*a, 0 ) * ( 1 - Pow5( 1 - NoL ) ) + min( 2*a, 1 ) ) * ( 1 - 0.5*a + 0.5*a * NoL );
	float Vd = ( a2 / ( (a2 + 0.09) * (1.31072 + 0.995584 * NoV) ) ) * ( 1 - pow( 1 - NoL, ( 1 - 0.3726732 * NoV * NoV ) / ( 0.188566 + 0.38841 * NoV ) ) );
	float Bp = Cosri < 0 ? 1.4 * NoV * Cosri : Cosri / max( NoL, 1e-8 );
	float Lr = (21.0 / 20.0) * (1 - F0) * ( Fr * Lm + Vd + Bp );
	return DiffuseColor / PI * Lr;
#endif
}

// [ Chan 2018, "Material Advances in Call of Duty: WWII" ]
// It has been extended here to fade out retro reflectivity contribution from area light in order to avoid visual artefacts.
float3 Diffuse_Chan(float3 DiffuseColor, float a2, float NoV, float NoL, float VoH, float NoH, float RetroReflectivityWeight)
{
	// We saturate each input to avoid out of range negative values which would result in weird darkening at the edge of meshes (resulting from tangent space interpolation).
	NoV = saturate(NoV);
	NoL = saturate(NoL);
	VoH = saturate(VoH);
	NoH = saturate(NoH);

	// a2 = 2 / ( 1 + exp2( 18 * g )
	float g = saturate((1.0 / 18.0) * log2(2 / a2 - 1));

	float F0 = VoH + Pow5(1 - VoH);
	float FdV = 1 - 0.75 * Pow5(1 - NoV);
	float FdL = 1 - 0.75 * Pow5(1 - NoL);

	// Rough (F0) to smooth (FdV * FdL) response interpolation
	float Fd = lerp(F0, FdV * FdL, saturate(2.2 * g - 0.5));

	// Retro reflectivity contribution.
	float Fb = ((34.5 * g - 59) * g + 24.5) * VoH * exp2(-max(73.2 * g - 21.2, 8.9) * sqrt(NoH));
	// It fades out when lights become area lights in order to avoid visual artefacts.
	Fb *= RetroReflectivityWeight;
	
	return DiffuseColor * ((1 / PI) * (Fd + Fb));
}

// [Blinn 1977, "Models of light reflection for computer synthesized pictures"]
float D_Blinn(float a2, float NoH)
{
	float n = 2 / a2 - 2;
	return (n + 2) / (2 * PI) * PhongShadingPow(NoH, n); // 1 mad, 1 exp, 1 mul, 1 log
}

// [Beckmann 1963, "The scattering of electromagnetic waves from rough surfaces"]
float D_Beckmann(float a2, float NoH)
{
	float NoH2 = NoH * NoH;
	return exp((NoH2 - 1) / (a2 * NoH2)) / (PI * a2 * NoH2 * NoH2);
}

// GGX / Trowbridge-Reitz
// [Walter et al. 2007, "Microfacet models for refraction through rough surfaces"]
float D_GGX(float a2, float NoH)
{
	float d = (NoH * a2 - NoH) * NoH + 1; // 2 mad
	return a2 / (PI * d * d); // 4 mul, 1 rcp
}

// Anisotropic GGX
// [Burley 2012, "Physically-Based Shading at Disney"]
float D_GGXaniso(float ax, float ay, float NoH, float XoH, float YoH)
{
// The two formulations are mathematically equivalent
#if 1
	float a2 = ax * ay;
	float3 V = float3(ay * XoH, ax * YoH, a2 * NoH);
	float S = dot(V, V);

	return (1.0f / PI) * a2 * Square(a2 / S);
#else
	float d = XoH*XoH / (ax*ax) + YoH*YoH / (ay*ay) + NoH*NoH;
	return 1.0f / ( PI * ax*ay * d*d );
#endif
}

float Vis_Implicit()
{
	return 0.25;
}

// [Neumann et al. 1999, "Compact metallic reflectance models"]
float Vis_Neumann(float NoV, float NoL)
{
	return 1 / (4 * max(NoL, NoV));
}

// [Kelemen 2001, "A microfacet based coupled specular-matte brdf model with importance sampling"]
float Vis_Kelemen(float VoH)
{
	// constant to prevent NaN
	return rcp(4 * VoH * VoH + 1e-5);
}

// Tuned to match behavior of Vis_Smith
// [Schlick 1994, "An Inexpensive BRDF Model for Physically-Based Rendering"]
float Vis_Schlick(float a2, float NoV, float NoL)
{
	float k = sqrt(a2) * 0.5;
	float Vis_SchlickV = NoV * (1 - k) + k;
	float Vis_SchlickL = NoL * (1 - k) + k;
	return 0.25 / (Vis_SchlickV * Vis_SchlickL);
}

// Smith term for GGX
// [Smith 1967, "Geometrical shadowing of a random rough surface"]
float Vis_Smith(float a2, float NoV, float NoL)
{
	float Vis_SmithV = NoV + sqrt(NoV * (NoV - NoV * a2) + a2);
	float Vis_SmithL = NoL + sqrt(NoL * (NoL - NoL * a2) + a2);
	return rcp(Vis_SmithV * Vis_SmithL);
}

// Appoximation of joint Smith term for GGX
// [Heitz 2014, "Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs"]
float Vis_SmithJointApprox(float a2, float NoV, float NoL)
{
	float a = sqrt(a2);
	float Vis_SmithV = NoL * (NoV * (1 - a) + a);
	float Vis_SmithL = NoV * (NoL * (1 - a) + a);
	return 0.5 * rcp(Vis_SmithV + Vis_SmithL);
}

// [Heitz 2014, "Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs"]
float Vis_SmithJoint(float a2, float NoV, float NoL)
{
	float Vis_SmithV = NoL * sqrt(NoV * (NoV - NoV * a2) + a2);
	float Vis_SmithL = NoV * sqrt(NoL * (NoL - NoL * a2) + a2);
	return 0.5 * rcp(Vis_SmithV + Vis_SmithL);
}

// [Heitz 2014, "Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs"]
float Vis_SmithJointAniso(float ax, float ay, float NoV, float NoL, float XoV, float XoL, float YoV, float YoL)
{
	float Vis_SmithV = NoL * length(float3(ax * XoV, ay * YoV, NoV));
	float Vis_SmithL = NoV * length(float3(ax * XoL, ay * YoL, NoL));
	return 0.5 * rcp(Vis_SmithV + Vis_SmithL);
}

float3 F_None(float3 SpecularColor)
{
	return SpecularColor;
}

// [Schlick 1994, "An Inexpensive BRDF Model for Physically-Based Rendering"]
float3 F_Schlick(float3 SpecularColor, float VoH)
{
	float Fc = Pow5(1 - VoH); // 1 sub, 3 mul
	//return Fc + (1 - Fc) * SpecularColor;		// 1 add, 3 mad
	
	// Anything less than 2% is physically impossible and is instead considered to be shadowing
	return saturate(50.0 * SpecularColor.g) * Fc + (1 - Fc) * SpecularColor;
}

float3 F_Schlick(float3 F0, float3 F90, float VoH)
{
	float Fc = Pow5(1 - VoH);
	return F90 * Fc + (1 - Fc) * F0;
}

float3 F_Fresnel(float3 SpecularColor, float VoH)
{
	float3 SpecularColorSqrt = sqrt(clamp(float3(0, 0, 0), float3(0.99, 0.99, 0.99), SpecularColor));
	float3 n = (1 + SpecularColorSqrt) / (1 - SpecularColorSqrt);
	float3 g = sqrt(n * n + VoH * VoH - 1);
	return 0.5 * Square((g - VoH) / (g + VoH)) * (1 + Square(((g + VoH) * VoH - 1) / ((g - VoH) * VoH + 1)));
}



//---------------
// EnvBRDF
//---------------
void ModifyGGXAnisotropicNormalRoughness(float3 WorldTangent, float Anisotropy, inout float Roughness, inout float3 N, float3 V)
{
	if (abs(Anisotropy) > 0.0f)
	{
		float3 X = WorldTangent;
		float3 Y = normalize(cross(N, X));

		float3 AnisotropicDir = Anisotropy >= 0.0f ? Y : X;
		float3 AnisotropicT = cross(AnisotropicDir, V);
		float3 AnisotropicN = cross(AnisotropicT, AnisotropicDir);

		float AnisotropicStretch = abs(Anisotropy) * saturate(5.0f * Roughness);
		N = normalize(lerp(N, AnisotropicN, AnisotropicStretch));
#if 0
		Roughness *= saturate(1.2f - abs(Anisotropy));
#endif
	}
}

// Convert a roughness and an anisotropy factor into GGX alpha values respectively for the major and minor axis of the tangent frame
void GetAnisotropicRoughness(float Alpha, float Anisotropy, out float ax, out float ay)
{
#if 1
	// Anisotropic parameters: ax and ay are the roughness along the tangent and bitangent	
	// Kulla 2017, "Revisiting Physically Based Shading at Imageworks"
	ax = max(Alpha * (1.0 + Anisotropy), 0.001f);
	ay = max(Alpha * (1.0 - Anisotropy), 0.001f);
#else
	float K = sqrt(1.0f - 0.95f * Anisotropy);
	ax = max(Alpha / K, 0.001f);
	ay = max(Alpha * K, 0.001f);
#endif
}

// Convert a roughness and an anisotropy factor, into two roughnesses respectively for the major and minor axis of the tangent frame
float2 GetAnisotropicRoughness(float Roughness, float Anisotropy)
{
	// Anisotropic parameters: ax and ay are the roughness along the tangent and bitangent	
	// Kulla 2017, "Revisiting Physically Based Shading at Imageworks"
	float2 Out = saturate(Roughness);
	Anisotropy = clamp(Anisotropy, -1.0, 1.0);
	Out.x = max(Roughness * sqrt(1.0 + Anisotropy), 0.001f);
	Out.y = max(Roughness * sqrt(1.0 - Anisotropy), 0.001f);
	return Out;
}

// Return anisotropy factor (+1: Tangent is major axis, -1: Bitangent is the major axis), and the original roughness
void GetAnisotropicFactor(float RoughnessX, float RoughnessY, inout float Anisotropy, inout float OriginalRoughness)
{
	const float MinRoughness = 0.001f;
	float r = Pow2(max(RoughnessX, MinRoughness) / max(RoughnessY, MinRoughness));
	Anisotropy = (r - 1.0) / (r + 1.0);
	OriginalRoughness = (RoughnessX + RoughnessY) / (sqrt(1.0 + Anisotropy) + sqrt(1.0 - Anisotropy));
}

#ifndef PreIntegratedGF
Texture2D PreIntegratedGF;
SamplerState PreIntegratedGFSampler;
#endif

// [Karis 2013, "Real Shading in Unreal Engine 4" slide 11]
half3 EnvBRDF(half3 SpecularColor, half Roughness, half NoV)
{
	// Importance sampled preintegrated G * F
	float2 AB = Texture2DSampleLevel(PreIntegratedGF, PreIntegratedGFSampler, float2(NoV, Roughness), 0).rg;

	// Anything less than 2% is physically impossible and is instead considered to be shadowing 
	float3 GF = SpecularColor * AB.x + saturate(50.0 * SpecularColor.g) * AB.y;
	return GF;
}

half3 EnvBRDF(half3 F0, half3 F90, half Roughness, half NoV)
{
	// Importance sampled preintegrated G * F
	float2 AB = Texture2DSampleLevel(PreIntegratedGF, PreIntegratedGFSampler, float2(NoV, Roughness), 0).rg;
	float3 GF = F0 * AB.x + F90 * AB.y;
	return GF;
}

half2 EnvBRDFApproxLazarov(half Roughness, half NoV)
{
	// [ Lazarov 2013, "Getting More Physical in Call of Duty: Black Ops II" ]
	// Adaptation to fit our G term.
	const half4 c0 = { -1, -0.0275, -0.572, 0.022 };
	const half4 c1 = { 1, 0.0425, 1.04, -0.04 };
	half4 r = Roughness * c0 + c1;
	half a004 = min(r.x * r.x, exp2(-9.28 * NoV)) * r.x + r.y;
	half2 AB = half2(-1.04, 1.04) * a004 + r.zw;
	return AB;
}

half3 EnvBRDFApprox(half3 SpecularColor, half Roughness, half NoV)
{
	half2 AB = EnvBRDFApproxLazarov(Roughness, NoV);

	// Anything less than 2% is physically impossible and is instead considered to be shadowing
	// Note: this is needed for the 'specular' show flag to work, since it uses a SpecularColor of 0
	float F90 = saturate(50.0 * SpecularColor.g);

	return SpecularColor * AB.x + F90 * AB.y;
}

half3 EnvBRDFApprox(half3 F0, half3 F90, half Roughness, half NoV)
{
	half2 AB = EnvBRDFApproxLazarov(Roughness, NoV);
	return F0 * AB.x + F90 * AB.y;
}

half EnvBRDFApproxNonmetal(half Roughness, half NoV)
{
	// Same as EnvBRDFApprox( 0.04, Roughness, NoV )
	const half2 c0 = { -1, -0.0275 };
	const half2 c1 = { 1, 0.0425 };
	half2 r = Roughness * c0 + c1;
	return min(r.x * r.x, exp2(-9.28 * NoV)) * r.x + r.y;
}

void EnvBRDFApproxFullyRough(inout half3 DiffuseColor, inout half3 SpecularColor)
{
	// Factors derived from EnvBRDFApprox( SpecularColor, 1, 1 ) == SpecularColor * 0.4524 - 0.0024
	DiffuseColor += SpecularColor * 0.45;
	SpecularColor = 0;
	// We do not modify Roughness here as this is done differently at different places.
}
void EnvBRDFApproxFullyRough(inout half3 DiffuseColor, inout half SpecularColor)
{
	DiffuseColor += SpecularColor * 0.45;
	SpecularColor = 0;
}
void EnvBRDFApproxFullyRough(inout half3 DiffuseColor, inout half3 F0, inout half3 F90)
{
	DiffuseColor += F0 * 0.45;
	F0 = F90 = 0;
}


float D_InvBlinn(float a2, float NoH)
{
	float A = 4;
	float Cos2h = NoH * NoH;
	float Sin2h = 1 - Cos2h;
	//return rcp( PI * (1 + A*m2) ) * ( 1 + A * ClampedPow( Sin2h, 1 / m2 - 1 ) );
	return rcp(PI * (1 + A * a2)) * (1 + A * exp(-Cos2h / a2));
}

float D_InvBeckmann(float a2, float NoH)
{
	float A = 4;
	float Cos2h = NoH * NoH;
	float Sin2h = 1 - Cos2h;
	float Sin4h = Sin2h * Sin2h;
	return rcp(PI * (1 + A * a2) * Sin4h) * (Sin4h + A * exp(-Cos2h / (a2 * Sin2h)));
}

float D_InvGGX(float a2, float NoH)
{
	float A = 4;
	float d = (NoH - a2 * NoH) * NoH + a2;
	return rcp(PI * (1 + A * a2)) * (1 + 4 * a2 * a2 / (d * d));
}

float Vis_Cloth(float NoV, float NoL)
{
	return rcp(4 * (NoL + NoV - NoL * NoV));
}

