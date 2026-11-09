/*-- Pinguin Armor --*/

#strict
#include GEAR

local pNightVision, superfertigkeit;	// saves the nightvision-light
local cccccc, cccccc2;  // zum autom. deaktivieren der Fähigkeit, bzw. zum Laden des Effekts während Fähigkeit aktiv ist im Hazardclonkscript
local fertig, bereit, aufladen, angelegt,derclonk;

/* Status */

public func GetGearType()	{ return(GEAR_Armor); }
protected func Hit()		{ Sound("ClonkHit*"); }
public func IsEquipment()	{ return(true); }

/* Callbacks */

public func GearEffect(object pClonk)
{
  // Change graphics and portrait
  MakeGraphic(pClonk);
  Sound("Connect");
  SetPhysical("Fight", GetPhysical("Fight", PHYS_Current, pClonk) +2000, PHYS_StackTemporary, pClonk);
  SetPhysical("Throw", GetPhysical("Throw", PHYS_Current, pClonk) +0, PHYS_StackTemporary, pClonk);
  SetPhysical("Jump", GetPhysical("Jump", PHYS_Current, pClonk) +0, PHYS_StackTemporary, pClonk);
  SetPhysical("Walk", GetPhysical("Walk", PHYS_Current, pClonk) -15000, PHYS_StackTemporary, pClonk);
  SetPhysical("Scale", GetPhysical("Scale", PHYS_Current, pClonk) - 8000, PHYS_StackTemporary, pClonk);
  SetPhysical("Hangle", GetPhysical("Hangle", PHYS_Current, pClonk) - 8000, PHYS_StackTemporary, pClonk);
  SetPhysical("Swim", GetPhysical("Swim", PHYS_Current, pClonk) + 50000, PHYS_StackTemporary, pClonk);
  SetPhysical("Breath", GetPhysical("Breath", PHYS_Current, pClonk) +1000000, PHYS_StackTemporary, pClonk);
  SetPhysical("Dig", GetPhysical("Dig", PHYS_Current, pClonk) - 15000, PHYS_StackTemporary, pClonk);
  SetPhysical("Energy", GetPhysical("Energy", PHYS_Current, pClonk) +0, PHYS_StackTemporary, pClonk);
  
  if(pClonk->IsPinguin()==2)
  {
      PlayerMessage(GetOwner(pClonk),"Trägt bereits einen Taucheranzug",pClonk);
      return(pClonk->~TakeOffGear(0, GEAR_Armor)); // wenn bereits der Taucheranzug angezogen ist, dann diese pinguinrüsstung gleich wieder ausziehen, weil beide die pinguin variable benutzen.
  }
  
  pClonk->Pinguinan();
}

func MakeGraphic(pClonk)
{
  var iPortraitCount = 1; 
  var strGraphics = "Pinguin";
  SetGraphics(strGraphics,pClonk);
  SetPortrait("Pinguin", pClonk, PNGU, 0, 0);
}

public func GearEffectReset(object pClonk)
{

// Grafik je nach Stufe zurücksetzen
 var stuufe=pClonk->~AktuelleStufe();
if(stuufe)
{

if(stuufe>=1 && stuufe<=9)
{SetGraphics("Normal",pClonk);  //Hazardclonk mit Clonkgrafik
  var iPortraitCount = 5; 
  var strGraphics = "";
SetPortrait(Format("%s%d", strGraphics, 1 + Random(iPortraitCount)), pClonk, CLNK, 0, 0);}

if(stuufe>=10 && stuufe<=14 && !(pClonk->~IstBauarbeiter()))
{SetPortrait("random", pClonk, S_XX); SetGraphics("SG1",pClonk);}  //Hazardclonk mit SG1 Grafik

// Bisher nur eine Grafik für alle Stufen ab 10
if(stuufe>=10 && stuufe<=20 && pClonk->~IstBauarbeiter())
{ SetPortrait("random", pClonk, X_12); 
SetGraphics("Bauer",pClonk);}  //Hazardclonk mit Arbeiter Grafik

if(stuufe>=15 && stuufe<=19 && !(pClonk->~IstBauarbeiter()))
{SetGraphics("",pClonk);
  SetPortrait(GetPortrait(pClonk, 0, 1), pClonk, GetPortrait(pClonk, 1, 1), 0, 0);}  //Hazardclonk Grafik

if(stuufe==20 && !(pClonk->~IstBauarbeiter()))
{SetGraphics("Future",pClonk);
SetPortrait("FutureP", pClonk, HZCK);} //Hazardclonk Future Grafik, Future Portrait
}
  // reset graphics and portrait
else{  SetGraphics("",pClonk);
  SetPortrait(GetPortrait(pClonk, 0, 1), pClonk, GetPortrait(pClonk, 1, 1), 0, 0);}
  Sound("Connect");
  ResetPhysical(pClonk, "Fight");
  ResetPhysical(pClonk, "Throw");
  ResetPhysical(pClonk, "Walk");
  ResetPhysical(pClonk, "Scale");
  ResetPhysical(pClonk, "Hangle");
  ResetPhysical(pClonk, "Jump");
  ResetPhysical(pClonk, "Swim");
  ResetPhysical(pClonk, "Breath");
  ResetPhysical(pClonk, "Dig");
  ResetPhysical(pClonk, "Energy");
  if(pClonk -> IsPinguin()==1) pClonk->Pinguinaus();
  if(pNightVision) RemoveObject(pNightVision);
}

/* Damage reduction */

public func OnClonkDmg(int iDamage, int iType, object pClonk)
{
  if(!pClonk) return();
  //Melee
  if(iType == DMG_Melee)
    return(0);     // 40% Melee
  //Projectile
  if(iType == DMG_Projectile)
    return(0);     // 70% Laser 
  //Flame
  if(iType == DMG_Fire)
    return(0);   // 50% Fire 
  //Plazma
  if(iType == DMG_Bio)
    return(0);     // 60% Plasma 
  //Explosion
  if(iType == DMG_Explosion)
    return(0);     // 40% Explosion
  //Energy
  if(iType == DMG_Energy)
    return(0);    // 60% Electric 
  //Stahlmantelkugeln
  if(iType == DMG_BetterProjectile)
    return(0);     // Stahlmantelkugeln durchschlagen Rüstungen einfach
}

/* Use per context menu of the hazard clonk */

public func ConDesc() {
  if(pNightVision) return("$CtxNightVisionOff$");
  return("$CtxNightVisionOn$");
}

public func ConUseable() {
  // need for nightvision
    return(true);
}

public func ConUse(object pHazard) {

  // turn on
  if(!pNightVision) {
    pNightVision = AddLight2(2200,RGBa(50,255,50,50),pHazard);
    if(!pNightVision)
      return();
    pNightVision->SetVisibility(VIS_Owner);
  }

  // turn off
  else {
    RemoveObject(pNightVision);
  }
  // Attentioooon: Here comes the effect!
  // ..
  // .....!
  Sound("Click"); // Ba-bum.
}


public func IsAnvilProduct() { return(1); }