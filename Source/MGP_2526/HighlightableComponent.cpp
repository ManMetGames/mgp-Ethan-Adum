// Fill out your copyright notice in the Description page of Project Settings.


#include "HighlightableComponent.h"

// Sets default values for this component's properties
UHighlightableComponent::UHighlightableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHighlightableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHighlightableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HighlightableComponent.generated.h"

UENUM(BlueprintType)
enum class ETeamType : uint8
{
    Enemy,
    Ally
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YOURGAME_API UHighlightableComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHighlightableComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Highlight")
    ETeamType Team = ETeamType::Enemy;

    // Stencil values: 1 = Enemy (red), 2 = Ally (green)
    UFUNCTION(BlueprintCallable, Category = "Highlight")
    void EnableHighlight(bool bEnable);

protected:
    virtual void BeginPlay() override;

private:
    void SetCustomDepthOnMeshes(bool bEnable);
};


#include "HighlightableComponent.h"
#include "Components/PrimitiveComponent.h"

UHighlightableComponent::UHighlightableComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHighlightableComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UHighlightableComponent::EnableHighlight(bool bEnable)
{
    SetCustomDepthOnMeshes(bEnable);
}

void UHighlightableComponent::SetCustomDepthOnMeshes(bool bEnable)
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    // Determine stencil value based on team
    int32 StencilValue = (Team == ETeamType::Enemy) ? 1 : 2;

    // Find all primitive components and enable custom depth
    TArray<UPrimitiveComponent*> PrimitiveComponents;
    Owner->GetComponents<UPrimitiveComponent>(PrimitiveComponents);

    for (UPrimitiveComponent* PrimComp : PrimitiveComponents)
    {
        if (PrimComp)
        {
            PrimComp->SetRenderCustomDepth(bEnable);
            if (bEnable)
            {
                PrimComp->SetCustomDepthStencilValue(StencilValue);
            }
        }
    }
}
