// ynPawn.cpp


#include "Player/ynPawn.h"
#include "yn_nine_task.h"

// Sets default values
AynPawn::AynPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AynPawn::BeginPlay()
{
	Super::BeginPlay();
	
	FString NetRoleString = yn_nine_taskFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("ynPawn::BeginPlay() %s[%s]"), *yn_nine_taskFunctionLibrary::GetNetModeString(this), *NetRoleString);
	yn_nine_taskFunctionLibrary::MyprintString(this, CombinedString, 10.f);
}

void AynPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	FString NetRoleString = yn_nine_taskFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("ynPawn::PossessedBy() %s[%s]"), *yn_nine_taskFunctionLibrary::GetNetModeString(this), *NetRoleString);
	yn_nine_taskFunctionLibrary::MyprintString(this, CombinedString, 10.f);
}

// Called every frame
void AynPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AynPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

