// ynPawn.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ynPawn.generated.h"

UCLASS()
class YN_NINE_TASK_API AynPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AynPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController)override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
