/*-- Gastank --*/

#strict 3

protected func Initialize()
{
  // Standardfärbung
  SetColorDw(RGB(100,255,100));
}

func Damage(a,b,c,d,e,f)
{
if((GetDamage() ?? 0)>100) Incinerate();
_inherited(a,b,c,d,e,f);
}

protected func Incineration()
{
  /* Mächtiger Badabumm! */

  // TIM1 nicht geladen
  if(!GetCategory(nil, TIM1)) return(InstaExplode());
  // TIM1 ist geladen. Also kleine Bomben verteilen!
  for(var i=nil, tim ; (i ?? 0) < 7 ; S42MigrationStep(i, 1, true))
    {
    tim = CreateObject(TIM1, RandomX(-80, 80), RandomX(-20,20), -1);
    Schedule(Format("Explode()", 20+(Random(20) ?? 0)), 25 + (Random(25) ?? 0), nil, tim);
    }
  // Und explodieren!
  return(InstaExplode());
}

private func InstaExplode()
{
  /* Viel Effekt */
  CastParticles("Wave", 7+(Random(5) ?? 0), RandomX(35,55), nil,nil, 30,60, RGBa(nil,nil,200,50), RGBa(20,20,255,100));
  CastParticles("Glas", 15+(Random(5) ?? 0), 100, nil,nil, 60,100, RGBa(200,200,200), RGBa(200,200,200));
  AddLightFlash(120, nil,nil, RGBa(255,255,200,100));
  Sparks(7+(Random(5) ?? 0), RGBa(255,255,150,100));
  /* Rauch und Explosionen! */
  return(Explode(120));
}

public func IsCraneGrabable() { return(1); }
func IsBulletTarget(){return(1);}
