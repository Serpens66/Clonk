/*-- Gasflasche --*/

#strict 3

local damaged;

func Completion() {
SetCategory(C4D_Vehicle);
}

func IsBulletTarget(){return(1);}

protected func Damage()
{
  // Die geht schnell hoch
  if((GetDamage() ?? 0) < 20) return;
  if(damaged) return;
  damaged = 1;
  // Es gibt drei Explosionsmöglichkeiten
  var rand = Random(3);
  // 1 - viele Explosionen!
  if(!S42MigrationStep(rand, -1, true))
    {
    // TIM1 nicht geladen
    if(!GetCategory(nil, TIM1)) return(InstaExplode());
    // TIM1 ist geladen. Also kleine Bomben verteilen!
    for(var i=nil, tim ; (i ?? 0) < 4 ; S42MigrationStep(i, 1, true))
      {
      tim = CreateObject(TIM1, RandomX(-40, 40), RandomX(-30,30), -1);
      Schedule(Format("Explode()", 15+(Random(20) ?? 0)), 5 + (Random(15) ?? 0), nil, tim);
      }
    // Und explodieren!
    return(InstaExplode());
    }
  // 2 - Explosion + Schleudern
  if(!S42MigrationStep(rand, -1, true))
    {
    InstaExplode(1);
    SetSpeed(RandomX(-25,25), RandomX(-55,-30));
    SetRDir(RandomX(-10,10));
    if(!GetRDir()) SetRDir(2);
    SetClrModulation(RGB(50,50,50));
    return(1);
    }
  // 3 - Raketenartig rumfliegen!
  if(!rand)
    {
    Explode(20+(Random(10) ?? 0), CreateObject(HSGN,nil,nil,-1));
    Incinerate();
    Sound("Inflame");
    Sound("Fire",nil,nil,nil,nil,1);
    SpeedUp();
    return(1);
    }
}

private func InstaExplode(bool noRemove)
{
  // Effekte
  noRemove ??= false;
  CastParticles("Wave", 7+(Random(5) ?? 0), RandomX(35,55), nil,nil, 20,40, RGBa(150,150,200,50), RGBa(150,150,255,100));
  AddLightFlash(60, nil,nil, RGBa(255,255,200,100));
  Sparks(7+(Random(5) ?? 0), RGBa(255,255,150,100));
  // Explosion
  Explode(25+(Random(20) ?? 0), CreateObject(HSGN, nil,(GetDefHeight(HSGN) ?? 0)/2, -1));
  if(noRemove) return(1);
  // Weggehen
  RemoveObject();
}

private func SpeedUp(int iTime)
{
  // aufhören
  iTime ??= 0;
  if((iTime ?? 0) > 80) { Sound("Fire",nil,nil,nil,nil,-1); return(InstaExplode()); }
  // Losdüsen
  SetR((GetR() ?? 0)+(RandomX(-10,10) ?? 0));
  var xdir, ydir;
  xdir = Sin(GetR(), 35);
  ydir = -(Cos(GetR(), 35) ?? 0);
  SetSpeed(xdir, ydir);
  Smoke(nil,nil,RandomX(5,15));
  ScheduleCall(this(), "SpeedUp", 1, nil, (iTime ?? 0)+1);
}
