#include "CannonShell.h"

ACannonShell::ACannonShell()
{
	Damage = 75.0f;
	InitialSpeed = 2500.0f;
	MaxSpeed = 2500.0f;
}

void ACannonShell::OnImpact(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// LSP: Base class davranışını koruyarak genişlet
	// Önce normal hasar ver
	Super::OnImpact(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

	// Ek özellik: zırh delme hesabı (gelecekte)
	// ArmorPenetration kullanılarak ek hasar hesaplanabilir
}
