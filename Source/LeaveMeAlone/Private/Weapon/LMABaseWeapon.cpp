// : LeaveMeAlone Game by Netologiya. All RightsReserved.

#include "Weapon/LMABaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);

// Sets default values
ALMABaseWeapon::ALMABaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	SetRootComponent(WeaponComponent);
}

void ALMABaseWeapon::Fire()
{
	if (!FireRateTimer.IsValid())
	{
		GetWorldTimerManager().SetTimer(FireRateTimer, this, &ALMABaseWeapon::Shoot, CurrentAmmoWeapon.FireRate, true, 0);
	}
	else
	{
		//¬озобнавл€ем таймер с поправкой на врем€ которое он уничтожалс€
		GetWorldTimerManager().SetTimer(FireRateTimer, this, &ALMABaseWeapon::Shoot, CurrentAmmoWeapon.FireRate, true,
			GetWorldTimerManager().GetTimerRemaining(TimerClearTimerFire));
		//ќстанавливаем уничтожение таймера уничтожени€ следующего выстрела
		GetWorldTimerManager().ClearTimer(TimerClearTimerFire);
	}
}

void ALMABaseWeapon::FireStop()
{
	GetWorldTimerManager().PauseTimer(FireRateTimer);
	//далее таймер следующего выстрела уничножаетс€ через врем€ оставшеес€ до следующего выстрела.
	GetWorldTimerManager().SetTimer(
		TimerClearTimerFire, FTimerDelegate::CreateLambda([this]() {
		GetWorldTimerManager().ClearTimer(FireRateTimer); }),
		GetWorldTimerManager().GetTimerRemaining(FireRateTimer), false);
}

void ALMABaseWeapon::ChangeClip()
{
	CurrentAmmoWeapon.Bullets = AmmoWeapon.Bullets;
}

// Called when the game starts or when spawned
void ALMABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmoWeapon = AmmoWeapon;
}

void ALMABaseWeapon::Shoot()
{
	const FTransform SocketTransform = WeaponComponent->GetSocketTransform("Muzzle");
	const FVector TraceStart = SocketTransform.GetLocation();
	const FVector ShootDirection = SocketTransform.GetRotation().GetForwardVector();
	const FVector TraceEnd = TraceStart + ShootDirection * TraceDistance;
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Black, false, 1.0f, 0, 2.0f);

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
	if (HitResult.bBlockingHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 24, FColor::Red, false, 1.0f);
	}

	DecrementBullets();
}

void ALMABaseWeapon::DecrementBullets()
{
	CurrentAmmoWeapon.Bullets--;
	UE_LOG(LogWeapon, Display, TEXT("Bullets = %s"), *FString::FromInt(CurrentAmmoWeapon.Bullets));
	if (IsCurrentClipEmpty())
	{
		OnCurrentlyClipEmpty.Broadcast();
		ChangeClip();
	}
}

bool ALMABaseWeapon::IsCurrentClipEmpty() const
{
	return CurrentAmmoWeapon.Bullets == 0;
}

bool ALMABaseWeapon::IsCurrentClipFull() const
{
	return CurrentAmmoWeapon.Bullets == AmmoWeapon.Bullets;
}

// Called every frame
void ALMABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
