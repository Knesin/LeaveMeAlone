// : LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LMAStaminaComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStaminaChanged, float)

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LEAVEMEALONE_API ULMAStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULMAStaminaComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxStamina = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DecreaseStamina = 0.1f;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FOnStaminaChanged OnStaminaChanged;

	UFUNCTION(BlueprintCallable)
	float GetStamina() const { return Stamina; }
	void Decrease();

private:
	float Stamina = 0.0f;
};
