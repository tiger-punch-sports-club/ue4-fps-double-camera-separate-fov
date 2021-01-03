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

	FVector2D CrosshairDrawPoint(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

#if 0
	const FVector2D CrosshairCenterOffset(0, 0);

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->Player);
		if (LocalPlayer && LocalPlayer->ViewportClient && LocalPlayer->ViewportClient->Viewport)
		{
			auto* Viewport = LocalPlayer->ViewportClient->Viewport;
			const auto ViewportSize = Viewport->GetSizeXY();

			const FVector2D TargetResolution(1920.0f, 1080.f);

			const auto MaxFit = DarkMagic::GetMaxFittingResolution(TargetResolution.X, TargetResolution.Y,
                                                           static_cast<float>(ViewportSize.X), static_cast<float>(ViewportSize.Y));

			if (MaxFit.AspectCorrection == DarkMagic::ResolutionInformation::EAspectCorrection::PILLAR_BOX)
			{
				CrosshairDrawPoint.X += CrosshairCenterOffset.X * MaxFit.PixelScale.X;
				CrosshairDrawPoint.Y += CrosshairCenterOffset.Y * MaxFit.PixelScale.Y;
			}
			else
			{
				const FVector2D PixelScale(Canvas->ClipX / TargetResolution.X, Canvas->ClipY / TargetResolution.Y);
				CrosshairDrawPoint.X += CrosshairCenterOffset.X * PixelScale.X;
				CrosshairDrawPoint.Y += CrosshairCenterOffset.Y * PixelScale.Y;
			}
			
		}
	}
#endif

	const FVector2D CrosshairSize(CrosshairTex->GetSurfaceWidth(), CrosshairTex->GetSurfaceHeight());
	const FVector2D CrosshairDrawPosition( (CrosshairDrawPoint.X - CrosshairSize.X * 0.5f),
										   (CrosshairDrawPoint.Y - CrosshairSize.Y * 0.5f));

	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, CrosshairSize, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );
}
