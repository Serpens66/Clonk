/*--- Holz ---*/

#strict

local type;

protected func Hit(xDir, yDir)
{
  Sound("WoodHit*");
  ChangeDef(EI8Z);
  SetXDir(0-(xDir/10)/5);
  SetYDir(0-(yDir/10)/5);
  CastObjects(EF8Z,3,5);
  return(1);
}

protected func Activate() {
  Exit(0,0,10);
  return(Hit());
}

func Incineration()
{
  AddEffect("IntBecomePot",this,1,300,this);
}

public func FxIntBecomePotStart(iEffectTarget,iEffectNumber,iTemp,healFactor)  {
  return(1);
}

public func FxIntBecomePotTimer(pTarget,iEffectNumber,iEffectTime) {

  if(OnFire()) {
    ChangeDef(EJ8Z); 
    Extinguish();
  }
  RemoveEffect("IntBecomePot", this);
  return(1);
}

public func Check() {
  if(MaterialName(GetMaterial()) eq "Oil" && !Random(2)) {
    type = "oil";
    ExtractMaterialAmount(0,5,Material("Oil"),20);
    ChangeDef(EK8Z);
    return();
  }
}