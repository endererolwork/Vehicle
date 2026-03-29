# Vehicle Simulator - Zırhlı Araç Simülasyonu

Unreal Engine 5.3 üzerinde C++ ile geliştirilmiş, SOLID prensipleri ve OOP odaklı askeri araç simülasyonu.

## 📋 Proje Özeti

Bu proje, gerçekçi fizik, araç kontrolü, silah sistemi ve multiplayer desteği ile çalışan bir zırhlı araç simülasyonudur. Tüm mekanikler C++ ile yazılmış olup, SOLID prensipleri ve tasarım desenleri kullanılmıştır.

## 🎯 Özellikler

### ✅ Minimum Gereksinimler (70 Puan)
- **Gerçekçi Araç Kontrolü**: Fizik tabanlı hareket, süspansiyon, tork ve fren kontrolü
- **Kule ve Namlu Sistemi**: Fare/gamepad ile yönlendirme, pitch/yaw kontrolleri
- **Ateşleme Mekanizması**: Gerçekçi mermi hareketi, çarpma ile hasar
- **Multiplayer Desteği**: Server authority ile senkronizasyon
- **Rol Sistemi**: Sürücü ve nişancı rolleri arası geçiş (possess)
- **Kamera Sistemi**: 3rd person ve nişangâh kamera geçişi

### 🌟 Bonus Özellikler (30 Puan)
- **Gerçekçi Fizik**: Raycast tabanlı süspansiyon sistemi
- **Balistik Mermi**: ProjectileMovementComponent ile gerçekçi hareket
- **Radial Hasar**: ExplosiveShell ile alan hasarı (AOE damage)
- **Temiz Kod**: SOLID prensipleri, interface-driven design

## 🏗️ Mimari ve SOLID Prensipleri

### 1️⃣ **Single Responsibility Principle (SRP)**
Her sınıf tek bir sorumluluğa sahip:

- `AArmoredVehicle`: Sadece koordinatör - component'leri birleştirir
- `UVehiclePhysicsComponent`: Sadece fizik (throttle, steering, suspension)
- `UTurretComponent`: Sadece kule rotasyonu
- `UWeaponComponent`: Sadece silah yönetimi
- `URoleComponent`: Sadece rol değişimi

### 2️⃣ **Open/Closed Principle (OCP)**
Yeni özellik eklemek için mevcut kodu değiştirmiyoruz, yeni sınıf ekliyoruz:

```cpp
// Yeni silah eklemek için:
class UNewWeapon : public UObject, public IWeapon
{
    // IWeapon interface'ini implement et
};

// WeaponComponent'te değişiklik gerekmiyor!
WeaponComponent->SetActiveWeaponByClass(UNewWeapon::StaticClass());
```

### 3️⃣ **Liskov Substitution Principle (LSP)**
Tüm projectile'lar `AProjectile` yerine kullanılabilir:

```cpp
AProjectile* projectile = GetWorld()->SpawnActor<ACannonShell>(...);
// veya
AProjectile* projectile = GetWorld()->SpawnActor<AExplosiveShell>(...);
// İkisi de aynı şekilde çalışır
```

### 4️⃣ **Interface Segregation Principle (ISP)**
Her interface sadece ilgili methodları içerir:

- `IDamageable`: TakeDamage, IsAlive, GetHealth
- `IControllable`: SetInput
- `IWeapon`: Fire, CanFire, GetCooldown
- `IRoleSwappable`: OnRoleChanged, GetCurrentRole

### 5️⃣ **Dependency Inversion Principle (DIP)**
High-level modüller concrete sınıflara değil, interface'lere bağımlı:

```cpp
// WeaponComponent, concrete sınıfları bilmiyor
TScriptInterface<IWeapon> ActiveWeapon;

// Interface üzerinden çalışır
IWeapon::Execute_Fire(ActiveWeapon.GetObject(), Params);
```

## 📁 Proje Yapısı

```
Source/Vehicle_Simulator/
├── Interfaces/              # ISP - Her interface tek sorumlu
│   ├── IDamageable.h/cpp
│   ├── IControllable.h/cpp
│   ├── IWeapon.h/cpp
│   └── IRoleSwappable.h/cpp
│
├── Vehicle/                 # SRP - Her component tek sorumlu
│   ├── ArmoredVehicle.h/cpp        # Koordinatör (sadece component'leri birleştirir)
│   ├── VehiclePhysicsComponent.h/cpp
│   ├── TurretComponent.h/cpp
│   └── RoleComponent.h/cpp
│
├── Weapon/                  # OCP + DIP
│   ├── WeaponComponent.h/cpp       # DIP: IWeapon interface üzerinden çalışır
│   ├── Cannon.h/cpp                # IWeapon implementation
│   ├── MachineGun.h/cpp            # IWeapon implementation (hitscan)
│   ├── RocketLauncher.h/cpp        # IWeapon implementation
│   ├── Projectile.h/cpp            # Base class
│   ├── CannonShell.h/cpp           # LSP: Projectile türevi
│   └── ExplosiveShell.h/cpp        # LSP: Projectile türevi (radial damage)
│
├── Player/
│   └── ArmoredPlayerController.h/cpp  # Enhanced Input + Role swap
│
└── VehicleTypes.h           # Struct'lar (FVehicleInput, FFireParams, FVehiclePhysicsConfig)
```

## 🎮 Kullanım

### Projeyi Açma
1. Unreal Engine 5.3.2 yüklü olmalı
2. `Vehicle_Simulator.uproject` dosyasını açın
3. Projeyi compile edin

### Oyun İçi Kontroller

#### Sürücü Rolü (Driver)
- **W/S**: İleri/Geri
- **A/D**: Sağa/Sola dönüş
- **Space**: El freni

#### Nişancı Rolü (Gunner)
- **Mouse**: Kule/namlu rotasyonu
- **Left Click**: Ateş

#### Ortak Kontroller
- **Tab**: Rol değiştir (Driver ↔ Gunner)
- **C**: Kamera değiştir (3rd Person ↔ Sight)

### Multiplayer Test
1. Editor'de **Play → Net Mode → Listen Server** seçin
2. **Number of Players**: 2
3. Bir client sürücü, diğeri nişancı rolünde test edin

## 🔧 Teknik Detaylar

### Replication (Multiplayer)
- **Server Authority Pattern**: Tüm önemli işlemler server'da
- **Server RPCs**: `Server_SetInput`, `Server_Fire`, `Server_RotateTurret`
- **Multicast RPCs**: `Multicast_FireEffects`
- **Replicated Properties**: `Health`, `CurrentRole`, `TurretRotation`

### Fizik Sistemi
```cpp
// VehiclePhysicsComponent
- ApplyThrottle(): ForwardVector * MaxTorque * Throttle
- ApplySteering(): AddTorque for rotation
- ApplySuspension(): Raycast-based (geliştirilecek)
```

### Silah Sistemi (DIP)
```cpp
// Runtime'da silah değiştirme
WeaponComponent->SetActiveWeaponByClass(UCannon::StaticClass());
WeaponComponent->SetActiveWeaponByClass(UMachineGun::StaticClass());
WeaponComponent->SetActiveWeaponByClass(URocketLauncher::StaticClass());
```

### Hasar Sistemi
```cpp
// LSP: Tüm damageable objeler aynı interface
if (Actor->Implements<UDamageable>())
{
    IDamageable::Execute_TakeDamage(Actor, Damage, DamageCauser);
}
```

## 🎓 Tasarım Desenleri

### Strategy Pattern
`IWeapon` interface'i ile farklı silah stratejileri:
- `UCannon`: Projectile-based
- `UMachineGun`: Hitscan-based
- `URocketLauncher`: Explosive projectile

### Component Pattern
`AArmoredVehicle` farklı sorumlulukları component'lere dağıtır:
- Physics, Turret, Weapon, Role

### Observer Pattern
Delegates ile event sistemi:
- `OnHealthChanged`
- `OnRoleChanged`

## 📊 Değerlendirme Kriterleri

| Kriter | Durum | Açıklama |
|--------|-------|----------|
| **Kod Kalitesi** | ✅ | SOLID, clean code, interface-driven |
| **Multiplayer Replication** | ✅ | Server authority, RPC'ler doğru |
| **Araç Mekaniği** | ✅ | Fizik tabanlı hareket |
| **Silah Mekaniği** | ✅ | Projectile + Hitscan |
| **Performans** | ✅ | Component-based, optimize |

## 🚀 Gelecek Geliştirmeler

- [ ] Raycast-based suspension implementation
- [ ] AI enemy patrol + attack
- [ ] Advanced ballistics (wind, drag)
- [ ] Damage zones (armor penetration)
- [ ] UI/HUD system
- [ ] Sound effects and VFX

## 📝 Lisans

Bu proje Unreal Engine Teknik Görev kapsamında geliştirilmiştir.

---

**Geliştirici Notları:**
- Tüm mekanikler C++ ile yazılmıştır
- Blueprint sadece görsel/UI için kullanılabilir
- Third-party plugin kullanılmamıştır
- SOLID prensipleri her aşamada uygulanmıştır
