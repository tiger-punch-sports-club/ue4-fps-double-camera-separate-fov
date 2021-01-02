// Fill out your copyright notice in the Description page of Project Settings.


#include "MySkeletalMeshComponent.h"
#include "DarkMagic/Utils.h"

FMatrix UMySkeletalMeshComponent::GetRenderMatrix() const
{
	FMatrix InverseOldViewProjectionMatrix;
	FMatrix NewViewProjectionMatrix;

	const FTransform ComponentTransform = GetComponentTransform();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->Player);

		if (LocalPlayer && LocalPlayer->ViewportClient && LocalPlayer->ViewportClient->Viewport)
		{
			FSceneViewFamilyContext ViewFamily
			(
				FSceneViewFamily::ConstructionValues(LocalPlayer->ViewportClient->Viewport, GetWorld()->Scene,
				                                     LocalPlayer->ViewportClient->EngineShowFlags)
				.SetRealtimeUpdate(true)
			);

			const FMatrices Matrices = GetMatrices(*GetWorld());

			const auto ViewportSize = LocalPlayer->ViewportClient->Viewport->GetSizeXY();
			const float Width = ViewportSize.X;
			const float Height = ViewportSize.Y;
			const float TargetWidth = 1920.0f;
			const float TargetHeight = 1080.0f;
			const float NearClippingPlaneDistance = Matrices.NearClippingPlaneDistance;

			FMatrix NewProjectionMatrix;

			const auto MaxFit = DarkMagic::GetMaxFittingResolution(1920.0f, 1080.f, ViewportSize.X, ViewportSize.Y);

			static_assert(ERHIZBuffer::IsInverted, "Z Buffer not inverted. Need to use FPerspectiveMatrix instead");

			if (MaxFit.AspectCorrection == DarkMagic::ResolutionInformation::EAspectCorrection::PILLAR_BOX)
			{
				const float HorPlusFov = DarkMagic::HorFovToHorPlus(DesiredHorizontalFov, 1920.0f, 1080.f, ViewportSize.X, ViewportSize.Y);
				const float DesiredHalfFovRad = HorPlusFov * PI / 360.0f;
				NewProjectionMatrix = FReversedZPerspectiveMatrix(DesiredHalfFovRad, Width, Height, NearClippingPlaneDistance);
			}
			else
			{
				const float DesiredHalfFovRad = DesiredHorizontalFov * PI / 360.0f;
				NewProjectionMatrix = FReversedZPerspectiveMatrix(DesiredHalfFovRad, TargetWidth, TargetHeight, NearClippingPlaneDistance);
			}

			const FMatrix ViewMatrix = Matrices.ViewMatrix;

			NewViewProjectionMatrix = ViewMatrix * NewProjectionMatrix;
			InverseOldViewProjectionMatrix = Matrices.InverseViewProjectionMatrix;
		}

	}

	const FMatrix ModelMatrix = ComponentTransform.ToMatrixWithScale();
	return ModelMatrix * NewViewProjectionMatrix * InverseOldViewProjectionMatrix;
}

FMatrices UMySkeletalMeshComponent::GetMatrices(UWorld& World) const
{
	FMatrices Matrices;

	// Source: https://answers.unrealengine.com/questions/25526/custom-mesh-fov.html
	APlayerController* PlayerController = World.GetFirstPlayerController();
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->Player);

	//Get View Origin
	FVector ViewOrigin;
	FRotator ViewRotation;
	PlayerController->GetPlayerViewPoint(/*out*/ ViewOrigin, /*out*/ ViewRotation);

	FMatrix ViewRotationMatrix = FInverseRotationMatrix(ViewRotation) * FMatrix(
        FPlane(0, 0, 1, 0),
        FPlane(1, 0, 0, 0),
        FPlane(0, 1, 0, 0),
        FPlane(0, 0, 0, 1));

	if (!ViewRotationMatrix.GetOrigin().IsNearlyZero(0.0f))
	{
		ViewOrigin += ViewRotationMatrix.InverseTransformPosition(FVector::ZeroVector);
		ViewRotationMatrix = ViewRotationMatrix.RemoveTranslation();
	}

	// Calculate view matrix
	const FMatrix ViewMatrix = FTranslationMatrix(-ViewOrigin) * ViewRotationMatrix;

	// Calculate project matrix
	const int32 X = FMath::TruncToInt(LocalPlayer->Origin.X * LocalPlayer->ViewportClient->Viewport->GetSizeXY().X);
	const int32 Y = FMath::TruncToInt(LocalPlayer->Origin.Y * LocalPlayer->ViewportClient->Viewport->GetSizeXY().Y);
	const uint32 SizeX = FMath::TruncToInt(LocalPlayer->Size.X * LocalPlayer->ViewportClient->Viewport->GetSizeXY().X);
	const uint32 SizeY = FMath::TruncToInt(LocalPlayer->Size.Y * LocalPlayer->ViewportClient->Viewport->GetSizeXY().Y);
	const FIntRect UnconstrainedRectangle = FIntRect(X, Y, X + SizeX, Y + SizeY);

	FSceneViewProjectionData ProjectionData;
	ProjectionData.SetViewRectangle(UnconstrainedRectangle);

	FMinimalViewInfo OutViewInfo;
	if (PlayerController->PlayerCameraManager != nullptr)
	{
		OutViewInfo = PlayerController->PlayerCameraManager->GetCameraCachePOV();
		OutViewInfo.FOV = PlayerController->PlayerCameraManager->GetFOVAngle();
	}

	PlayerController->GetPlayerViewPoint(/*out*/ OutViewInfo.Location, /*out*/ OutViewInfo.Rotation);

	FMinimalViewInfo::CalculateProjectionMatrixGivenView(OutViewInfo, LocalPlayer->AspectRatioAxisConstraint,
                                                         LocalPlayer->ViewportClient->Viewport, /*inout*/ ProjectionData);

	Matrices.ViewMatrix = ViewMatrix;
	Matrices.InverseViewMatrix = ViewMatrix.Inverse();
	Matrices.ProjectionMatrix = ProjectionData.ProjectionMatrix;
	Matrices.InverseProjectionMatrix = ProjectionData.ProjectionMatrix.Inverse();
	Matrices.InverseViewProjectionMatrix = Matrices.InverseProjectionMatrix * Matrices.InverseViewMatrix;
	Matrices.NearClippingPlaneDistance = Matrices.ProjectionMatrix.M[3][2];
	return Matrices;
}
