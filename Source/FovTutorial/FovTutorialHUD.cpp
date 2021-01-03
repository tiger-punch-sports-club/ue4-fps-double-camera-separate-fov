// Copyright Epic Games, Inc. All Rights Reserved.

#include "FovTutorialHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "DarkMagic/Utils.h"
#include "UObject/ConstructorHelpers.h"

AFovTutorialHUD::AFovTutorialHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
}


void AFovTutorialHUD::DrawHUD()
{
	Super::DrawHUD();
	
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	
	const FVector2D TargetOffset(0, 0);
	FVector2D Offset;

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->Player);
		if (LocalPlayer && LocalPlayer->ViewportClient && LocalPlayer->ViewportClient->Viewport)
		{
			auto* Viewport = LocalPlayer->ViewportClient->Viewport;
            
			const auto ViewportSize = Viewport->GetSizeXY();
			const FVector2D TargetResolution(1920.0f, 1080.f);

			const auto MaxFit = DarkMagic::GetMaxFittingResolution(TargetResolution.X, TargetResolution.Y, ViewportSize.X, ViewportSize.Y);

			if (MaxFit.AspectCorrection == DarkMagic::ResolutionInformation::EAspectCorrection::PILLAR_BOX)
			{
				Offset.X = TargetOffset.X * MaxFit.PixelScale.X;
				Offset.Y = TargetOffset.Y * MaxFit.PixelScale.Y;
			}
			else
			{
				const FVector2D PixelScale(Canvas->ClipX / TargetResolution.X, Canvas->ClipY / TargetResolution.Y);
				Offset.X = TargetOffset.X * PixelScale.X;
				Offset.Y = TargetOffset.Y * PixelScale.Y;
			}
		}
	}

	const float Scale = 1.0f;
	const FVector2D CrosshairSize(CrosshairTex->GetSurfaceWidth() * Scale, CrosshairTex->GetSurfaceHeight() * Scale);
    
	const FVector2D CrosshairDrawPosition(Center.X + Offset.X - CrosshairSize.X * 0.5f,
                                          Center.Y + Offset.Y - CrosshairSize.Y * 0.5f);


	// draw the crosshair
	FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, CrosshairSize, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);
}
