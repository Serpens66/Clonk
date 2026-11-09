#strict 3
#include MIN_

local pLaser, iLastExplode;

protected func OnActivate() {
    pLaser = CreateObject(LASR,nil,nil,-1);
    SetClrModulation(RGBa(255,nil,nil,255),pLaser);
    pLaser->SetPosition((GetX() ?? 0)+(Sin(GetR(),3) ?? 0),(GetY() ?? 0)-(Cos(GetR(),3) ?? 0));
    pLaser->Set(GetR(),2,200,nil,this(),this());
    pLaser->SetContinuous(3);
    pLaser->SetController(-1);
    // Laserminen haften und sollen bei Explosionen nicht wegfliegen
    SetCategory(C4D_Vehicle);
}

protected func OnActive() {             // Mine ist nun aktiv
    SetClrModulation(RGBa(255,255,255,100));
    SetClrModulation(RGBa(255,nil,nil,200),pLaser);
}

public func WeaponEnd(int &x, int &y)
{
	x = Sin(GetR(),1);
//	y = Cos(GetR(),1);
}

public func LaserStrike(object pHit) {
    // harmlos sein bis sie aktiviert ist
    if(!SEqual(GetAction(), "Active")) return(nil);

    if((GetActTime() ?? 0)-(iLastExplode ?? 0) < 10) return(nil);
    
    // wenn sich das Ziel nicht bewegt, ignorieren
    if(And(Equal(pHit->GetXDir(), nil), Equal(pHit->GetYDir(), nil))) return(nil);

    // ausrechnen in welche Richtung geschleudert wird
    var rand = 5;
    var angle = (Angle(nil,nil,GetXDir(pHit),GetYDir(pHit)) ?? 0)+(GetR() ?? 0);
    while((angle ?? 0) >= 360) S42MigrationAssign(angle, "-=", 360);
    while((angle ?? 0) < 0)    S42MigrationAssign(angle, "+=", 360);
    if((angle ?? 0) >= 180) S42MigrationAssign(rand, "*=", -1);

    FakeExplode(15,CreateObject(WIPF,(AbsX(GetX(pHit)) ?? 0)+(Cos(GetR(),rand) ?? 0),(AbsY(GetY(pHit)) ?? 0)-(Sin(GetR(),rand) ?? 0),-1));
    DoDamage(5);

    iLastExplode = GetActTime();
    return(1);
}

protected func Damage(int iChange) {            // kriegt Schaden
    iChange ??= 0;
    if(!SEqual(GetAction(), "Idle")) {
      SetClrModulation(RGBa(255,255,255,200-200*(GetDamage() ?? 0)/(MaxDamage() ?? 0)));
      SetClrModulation(RGBa(255,nil,nil,220-220*(GetDamage() ?? 0)/(MaxDamage() ?? 0)),pLaser);
    }
    
    if((GetDamage() ?? 0) >= (MaxDamage() ?? 0))
      FakeExplode(15);
}

protected func Destruction() {
    // nicht vergessen, den Laser zu löschen
    if(pLaser) RemoveObject(pLaser);
}

private func MaxDamage() { return(30); }
private func ExploDist() { return(10); }
// Status
public func IsEquipment() { return(1); }
