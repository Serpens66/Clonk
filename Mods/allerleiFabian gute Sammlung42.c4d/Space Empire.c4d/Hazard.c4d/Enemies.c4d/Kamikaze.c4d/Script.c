/*--Kamikaze --*/

#strict 3
#include ALN2

local hurt;

/* Aktionen */

func StartLuring()
{
  if(hurt)
    return;
  inherited();
}

// Beiﬂen
private func Bite(object pPrey)
{
  // Beiﬂen!11
  DoDmg(3+(Random(10) ?? 0)*(GetCon() ?? 0)/100, DMG_Explosion, pPrey);
  // Beiﬂsperre, damit man nicht dauerh¸pft
  AddEffect("NoBite", this(), 99, 50, this());
  
  //alien "scharf"? :D
  if(hurt)
  {
    if(glob) //extradmg mit Glob
    {
      DoDmg(20, DMG_Bio, pPrey);
    }
    //Kaboom. :D 
    Detonate();
  }
}

// Schleim schleudern
private func GlobTo(object pPrey)
{
  return; //kann er nicht :(
}

public func Birth()
{
  // Miniwinzig!
  SetCon(50);
}

protected func Death()
{
  GameCallEx("Zaehler9");
  // Zersplattern!
  Sparks(10, RGBa(255, 205, 25, 20));
  CastPXS("Lava", 100, 170);
  // Quieken!
  Sound("JumperDeath");
  // Mit Knall Verschwinden!
  if(hurt)
    Detonate();
  else
    RemoveObject();
}

func CheckLiquid()
{
  return; // kein Tod in wasser mehr
  if(!GBackLiquid(nil,-3))
    return;
  if(Equal(GetMaterial(nil,-3), Material("Water")))
  {
    ExtractMaterialAmount(nil,nil, Material("Water"), GetMass());
    CastPXS("Lava",GetMass(),GetMass());
    Death();
  }
}

public func OnDmg(int iDmg, int iType) {
  //Flame
  iDmg ??= 0;
  iType ??= 0;
  if(Equal(iType, DMG_Fire)) {
    return(25);
  }
  else
    return(inherited(iDmg, iType));
}

//Treffer
func OnHit() {
  hurt = true;
  AddEffect("Hurt",this(),1,4,this(),GetID());
}

private func Detonate() {
  var r = 25;
  if(glob)
    S42MigrationAssign(r, "+=", 10);
 if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))  Schedule(Format("Explode(%d)",r),1); 
 else Schedule(Format("FakeExplode(%d,0,0,%d)",r,r),1);
}

func FxHurtTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  var x,y;
  if(!!SEqual(GetAction(), "Hangle"))
    y = 2;
  if(!!SEqual(GetAction(), "Scale"))
    x = ((GetDir() ?? 0)*2-1) * 5;
    CreateParticle("Thrust",(RandomX(-4,4) ?? 0)+(x ?? 0),(RandomX(-4,4) ?? 0)+(y ?? 0),(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,RandomX(100,300),RGBa(255,200,200,100),nil,nil);
  CreateParticle("Fire",(RandomX(-2,2) ?? 0)+(x ?? 0),(Random(2) ?? 0)+(y ?? 0),nil,RandomX(-6,-12),RandomX(100,180),RGBa(255,RandomX(224,255),nil,160));
  CreateParticle("Fire2",(RandomX(-2,2) ?? 0)+(x ?? 0),(Random(2) ?? 0)+(y ?? 0),nil,RandomX(-8,-14),RandomX(100,180),RGBa(255,255,255,128));
}

func FxHurtEffect (string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
	iEffectNumber ??= 0;
	iNewEffectNumber ??= 0;
	if(!!SEqual(szNewEffectName, "Hurt")) return(-1);
}
