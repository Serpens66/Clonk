/*-- wurfAxt --*/

#strict 3
local iXDir,iYDir;

/* Ist eine Handwaffe */
public func IsFightWeapon()
{
  return(1);
}

/* Kampfaktion */
public func GetFightAction()
{
  return("AxeFight");
}

/* Schlagaktion */
public func GetStrikeAction()
{
  return("AxeStrike");
}

/* Kampfgeschwindigkeit */
public func GetStrikeTime()
{
  return(3);
}

/* Kampfschaden */
public func GetStrikeDamage()
{
  return(12);
}

/* Schlag */
public func StrikeSound()
{
  Sound("AxeHit*");
}

/* Aufschlag */
protected func Hit()
{
  Sound("MetalHit*");
}

/* Kann geschmiedet werden */
public func IsAnvilProduct() { return(1); }




//// WurfAXT


/*-- Wurfaxt --*/

protected func Hit()
{
SetAction("Idle");
  Sound("MetalHit*");
    CastObjects(AXE7,1,50); 
      RemoveObject(this());
        Sound("AxeHit*");
  CreateParticle("FSpark", nil, GetVertex(1,nil), -5, -5, RandomX(20,40), RGBa(120,120,120,120));
  if (FindObject2(Find_Distance(10),Find_OCF(OCF_Alive))) return(Opfer());
}

protected func Opfer()
{
  var Opfer = FindObject(nil,-20, -20, 40, 40, OCF_Alive,nil,nil,nil,Opfer);
  Sound("AxeHit*");
  if(Random(3))Fling(Opfer,(GetXDir() ?? 0)/180,(GetYDir() ?? 0)/180-2);
    var Opfer1 = FindObject(TRE1,-20, -20, 40, 40,Opfer1);
    var Opfer3 = FindObject(TRE3,-20, -20, 40, 40,Opfer3);
    var Opfer2 = FindObject(TRE2,-20, -20, 40, 40,Opfer2);
  if(Random(20))DoDamage(80,Opfer1); 
  if(Random(18))DoDamage(80,Opfer2); 
  if(Random(16))DoDamage(80,Opfer3); 
  RemoveObject(this());
}

protected func Departure(pObj) {
  if(GetXDir())
    if((GetOCF(pObj) ?? 0)&(OCF_Living ?? 0))
      SetAction("Depart");
}

protected func Departure2() {
 if(Contained()) return(SetAction("Idle"));
  SetAction("Fly");
  Sound("Catapult");
  SetXDir((GetXDir() ?? 0)*3);
  SetYDir(-20);
  SetRDir((GetXDir() ?? 0)*2);
  iXDir=(((GetXDir() ?? 0)>0) ?? 0)*2-1;
  iYDir=20;
}

protected func Flug() {

  CreateParticle("PSpark", nil, GetVertex(1,nil), -5, nil, RandomX(20,40), RGBa(120,120,120,120));

  if (FindObject2(Find_Distance(10),Find_OCF(OCF_Alive))) return(Opfer());
}

public func CarryLimit()
{
  // Nur eines tragbar
  return(1);
}

