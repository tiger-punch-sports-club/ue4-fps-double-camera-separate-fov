#pragma once
#include <algorithm>
#include <math.h>
#include <stdint.h>
#define DARK_MAGIC_PI 3.14159265358979323846f

namespace DarkMagic
{
    // ********************************
    // Common structs
    // ********************************
    struct FVec2
    {
        float X, Y;
        FVec2(float X, float Y) : X(X), Y(Y) {} 
    };

    // ********************************
    // Interpolation
    // float t = percentage [0, 1] range
    // Used for lerping from A to B
    // ********************************
    inline float SinLerp(float T)
    {
        return sinf(T * DARK_MAGIC_PI * 0.5f);
    }

    inline float CosLerp(float T)
    {
        return 1.0f - cosf(T * DARK_MAGIC_PI * 0.5f);
    }

    inline float SmoothStep(float T)
    {
        return T * T * (3.0f - 2.0f * T);
    }

    inline float SmootherStep(float T)
    {
        return T * T * T * (T * (6.0f * T - 15.0f) + 10.0f);
    }

    // ********************************
    // Calculations
    // ********************************
    inline uint32_t CalculateMipMapLevels(uint32_t Width, uint32_t Height)
    {
        return 1 + static_cast<uint32_t>(floor(log2(std::max(Width, Height))));
    }

    inline uint32_t NextPow2(uint32_t Value)
    {
        Value--;
        Value |= Value >> 1;
        Value |= Value >> 2;
        Value |= Value >> 4;
        Value |= Value >> 8;
        Value |= Value >> 16;
        Value++;
        return Value;
    }

    inline float TanDeg(float A)
    {
        return tanf(DARK_MAGIC_PI / 180.0f * A);
    }

    inline float AtanDeg(float A)
    {
        return 180.0f / DARK_MAGIC_PI * atanf(A);
    }

    // ********************************
    // Memory alignment
    // ********************************
#define ALIGN_256(MemorySize) (((MemorySize) + 255) & ~255)

    inline uint32_t Align256(uint32_t MemorySize)
    {
        return (MemorySize + 255) & ~255;
    }

    // ***********************************
    // Field Of View
    // Calculations use degrees instead of radians
    // ***********************************
    inline float HorFovToVertFov(float HorizontalFov, float Width, float Height)
    {
        const float VerticalTan2 = TanDeg(HorizontalFov / 2.0f);
        const float VerticalAspect = Height / Width;
        const float DesiredVerticalFOV = AtanDeg(VerticalTan2 * VerticalAspect) * 2.0f;
        return DesiredVerticalFOV;
    }

    inline float VertFovToHorFov(float VerticalFov, float Width, float Height)
    {
        const float HorizontalTan2 = TanDeg(VerticalFov / 2.0f);
        const float HorizontalAspect = Width / Height;
        const float HorizontalFOV = AtanDeg(HorizontalTan2 * HorizontalAspect) * 2.0f;
        return HorizontalFOV;
    }

    inline float HorFovToHorPlus(float TargetHorizontalFov, float TargetWidth, float TargetHeight, float CurrentWidth, float CurrentHeight)
    {     
        const float DesiredVerticalFOV = HorFovToVertFov(TargetHorizontalFov, TargetWidth, TargetHeight);
        return VertFovToHorFov(DesiredVerticalFOV, CurrentWidth, CurrentHeight);
    }

    // ***********************************
    // Max. fitting resolution
    // ***********************************
    struct ResolutionInformation
    {
        FVec2 MaxFittingResolution;
        FVec2 PixelScale;

        enum class EAspectCorrection
        {
            NONE,
            LETTER_BOX,
            PILLAR_BOX
        } AspectCorrection;
    };
    
    inline ResolutionInformation GetMaxFittingResolution(float TargetWidth, float TargetHeight, float CurrentWidth, float CurrentHeight)
    {
        const uint32 CurrentHeightU32 = static_cast<uint32>(CurrentHeight);

        const float TargetAspectRatio = TargetWidth / TargetHeight;

        uint32 Width = static_cast<uint32>(CurrentWidth);
        uint32 Height = static_cast<uint32>(Width / TargetAspectRatio + 0.5f);

        if (Height > CurrentHeightU32)
        {
            Height = CurrentHeightU32;
            Width = static_cast<uint32>(Height * TargetAspectRatio + 0.5f);
        }

        const float CurrentAspect = CurrentWidth / CurrentHeight;
        ResolutionInformation::EAspectCorrection AspectCorrection {};     
        if (TargetAspectRatio == CurrentAspect)
        {
            AspectCorrection = ResolutionInformation::EAspectCorrection::NONE;
        }
        else if (TargetAspectRatio < CurrentAspect)
        {
            AspectCorrection = ResolutionInformation::EAspectCorrection::PILLAR_BOX;
        }
        else if (TargetAspectRatio > CurrentAspect)
        {
            AspectCorrection = ResolutionInformation::EAspectCorrection::LETTER_BOX;
        }

        const float MaxFittingWidth = static_cast<float>(Width);
        const float MaxFittingHeight = static_cast<float>(Height);
        
        const float PixelScaleX = CurrentWidth / TargetWidth;
        const float PixelScaleY = CurrentHeight / TargetHeight;

        return { FVec2(MaxFittingWidth, MaxFittingHeight), FVec2(PixelScaleX, PixelScaleY), AspectCorrection };
    }

    
}
