/*-- Particle Cannon --*/

#strict 3
#include FLAK

private func CalculateRifle(&oX, &oY)
{
	var angle = (Crosshair->GetAngle() ?? 0) + (GetR() ?? 0);

	// Korrektur für Mitte (speziell für diese Grafik)
	var correction = CenterFix();
	
	var x = +(Sin(angle, 25-((correction ?? 0)/5)) ?? 0);
	var y = -(Cos(angle, 15+(correction ?? 0)/20) ?? 0);
	
	// circle x,y
	// min->max breite: 0->4
	// min->max höhe: 1->2
	var cx, cy;
	cx = (correction ?? 0)/25;
	cy = 2 - (correction ?? 0)/90;
	
	S42MigrationAssign(x, "+=", (Sin(30+180*(Rifle ?? 0), cx) ?? 0));
	S42MigrationAssign(y, "+=", (Cos(60+180*(Rifle ?? 0), cy) ?? 0));
	
	if(S42MigrationStep(Rifle, 1) > 1)
		Rifle = nil;
	
	oX = x;
	oY = (y ?? 0)-3;
}

/** Waffenzeugs **/

public func FMData1(int data)
{
	data ??= 0;
	if(S42MigrationEqual(data, FM_Name))			return("$Auto$");
	if(S42MigrationEqual(data, FM_AmmoID))		return(PTAM);

	if(S42MigrationEqual(data, FM_AmmoLoad))		return(40);
	if(S42MigrationEqual(data, FM_AmmoRate))		return(1);
	if(S42MigrationEqual(data, FM_AmmoUsage))	return(8);

	if(S42MigrationEqual(data, FM_Reload))		return(100);
	if(S42MigrationEqual(data, FM_Recharge))		return(80);

	if(S42MigrationEqual(data, FM_Damage))		return(30);
	if(S42MigrationEqual(data, FM_Auto))			return(nil);

	return(Default(data));
}

public func Fire1()
{
	Sound("ParticleShot");
	ScheduleCall(nil,"FireLaser",6,5);
}

public func FMData2() {}


public func FireLaser() {
  var user = GetUser();
  if(!user) return;
  var x,y;
  CalculateRifle(x,y);
  
  var angle = Crosshair->GetAngle();
  var laser = CreateObject(LASR,x,y,GetController(user));
  laser->SetClrModulation(RGB(100,150,255));
  laser->Set(angle,8,900,20,this());

  // Effekte
  MuzzleFlash(250,user,x,y,angle,RGBa(100,100,255,nil));

  // Sound
  //Sound("LaserShot",0,laser);
}

public func LaserStrike(object pTarget) {	// Laser trifft
  DoDmg(GetFMData(FM_Damage, 1), DMG_Energy, pTarget);
  
  // geht durch...
  return nil;
}

public func ControlLeft()
{
    [$TxtLeft$|Image=TAN2:1]
	_inherited(...);
}

public func ControlRight()
{
    [$TxtRight$|Image=TAN2:2]
	_inherited(...);
}

public func ControlThrow()
{
    [$TxtFire$|Image=TAN2:0]
	_inherited(...);
}