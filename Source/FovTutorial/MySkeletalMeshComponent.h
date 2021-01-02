// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "MySkeletalMeshComponent.generated.h"

struct FMatrices
{
	FMatrix ViewMatrix;
	FMatrix InverseViewMatrix;
	FMatrix ProjectionMatrix;
	FMatrix InverseProjectionMatrix;
	FMatrix InverseViewProjectionMatrix;
	float NearClippingPlaneDistance;
};

/**
 * 
 */
UCLASS()
class FOVTUTORIAL_API UMySkeletalMeshComponent : public USkeletalMeshComponent
{
public:	
	UPROPERTY(EditAnywhere, Category=Mesh)
	float DesiredHorizontalFov;
	
	GENERATED_BODY()
	virtual FMatrix GetRenderMatrix() const override;

	FMatrices GetMatrices(UWorld& World) const;
};
