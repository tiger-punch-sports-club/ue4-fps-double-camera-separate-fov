// Copyright Epic Games, Inc. All Rights Reserved.

#include "FovTutorialHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
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

	// Draw very simple crosshair
	const FVector2D TargetResolution(1920.0f, 1080.f);

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
		
	const FVector2D CrosshairSize(CrosshairTex->GetSurfaceWidth(), CrosshairTex->GetSurfaceHeight());

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition( (Center.X - CrosshairSize.X * 0.5f),
										   (Center.Y - CrosshairSize.Y * 0.5f));

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, CrosshairSize, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );
}
