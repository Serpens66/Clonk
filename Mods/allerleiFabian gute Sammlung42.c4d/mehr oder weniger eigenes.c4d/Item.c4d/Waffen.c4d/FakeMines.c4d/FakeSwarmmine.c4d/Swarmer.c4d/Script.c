/*-- Schwärmer --*/

#strict 3
#include MIN_

local xdir, ydir;

func Activate() {
    DoDamage(nil);

	var speed = RandomX(nil,50);
	var angle = Random(360);
    xdir = Sin(angle, speed);
    ydir = -(Cos(angle, speed) ?? 0);

    SetSpeed(xdir, ydir);
    SetAction("Fly");
    
    var tmp = CreateObject(MFLG,nil,nil,GetController());
    tmp->Set(this());
    tmp->SetR(Random(360));
    tmp->SetCon(70);
}

protected func DecreaseSpeed() {
    if((xdir ?? 0) > 0)
      S42MigrationStep(xdir, -1, true);
    if((xdir ?? 0) < 0)
      S42MigrationStep(xdir, 1, true);
    if((ydir ?? 0) > 0)
      S42MigrationStep(ydir, -1, true);
    if((ydir ?? 0) < 0)
      S42MigrationStep(ydir, 1, true);

    SetSpeed(xdir, ydir);
    if(!((GetActTime() ?? 0)%(RandomX(20,22) ?? 0))) Warning();
    Check();
}

protected func Warning() {
    CreateParticle("PSpark",nil,nil,nil,nil,40,RGBa(nil,64,255,120),this());
}

protected func Triggered() {                // ausgelöst (nach delay)
    FakeExplode(15);
}

private func MaxDamage() { return(6); }
private func ExploDist() { return(16); }
private func ExploDelay() { return(5); }

public func IsEquipment() { return(nil); }
