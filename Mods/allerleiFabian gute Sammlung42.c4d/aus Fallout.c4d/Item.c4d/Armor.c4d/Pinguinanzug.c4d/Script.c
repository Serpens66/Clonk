/*-- Pinguin Armor --*/

#strict 3
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
  SetPhysical("Fight", (GetPhysical("Fight", PHYS_Current, pClonk) ?? 0) +2000, PHYS_StackTemporary, pClonk);
  SetPhysical("Throw", (GetPhysical("Throw", PHYS_Current, pClonk) ?? 0) +0, PHYS_StackTemporary, pClonk);
  SetPhysical("Jump", (GetPhysical("Jump", PHYS_Current, pClonk) ?? 0) +0, PHYS_StackTemporary, pClonk);
  SetPhysical("Walk", (GetPhysical("Walk", PHYS_Current, pClonk) ?? 0) -15000, PHYS_StackTemporary, pClonk);
  SetPhysical("Scale", (GetPhysical("Scale", PHYS_Current, pClonk) ?? 0) - 8000, PHYS_StackTemporary, pClonk);
  SetPhysical("Hangle", (GetPhysical("Hangle", PHYS_Current, pClonk) ?? 0) - 8000, PHYS_StackTemporary, pClonk);
  SetPhysical("Swim", (GetPhysical("Swim", PHYS_Current, pClonk) ?? 0) + 50000, PHYS_StackTemporary, pClonk);
  SetPhysical("Breath", (GetPhysical("Breath", PHYS_Current, pClonk) ?? 0) +1000000, PHYS_StackTemporary, pClonk);
  SetPhysical("Dig", (GetPhysical("Dig", PHYS_Current, pClonk) ?? 0) - 15000, PHYS_StackTemporary, pClonk);
  SetPhysical("Energy", (GetPhysical("Energy", PHYS_Current, pClonk) ?? 0) +0, PHYS_StackTemporary, pClonk);
  
  if(Equal(pClonk->IsPinguin(), 2))
  {
      PlayerMessage(GetOwner(pClonk),"Trägt bereits einen Taucheranzug",pClonk);
      return(pClonk->~TakeOffGear(nil, GEAR_Armor)); // wenn bereits der Taucheranzug angezogen ist, dann diese pinguinrüsstung gleich wieder ausziehen, weil beide die pinguin variable benutzen.
  }
  
  pClonk->Pinguinan();
}

func MakeGraphic(pClonk)
{
  var iPortraitCount = 1; 
  var strGraphics = "Pinguin";
  SetGraphics(strGraphics,pClonk);
  SetPortrait("Pinguin", pClonk, PNGU, nil, nil);
}

public func GearEffectReset(object pClonk)
{

// Grafik je nach Stufe zurücksetzen
 var stuufe=pClonk->~AktuelleStufe();
if(stuufe)
{

if(And((stuufe ?? 0)>=1, (stuufe ?? 0)<=9))
{SetGraphics("Normal",pClonk);  //Hazardclonk mit Clonkgrafik
  var iPortraitCount = 5; 
  var strGraphics = "";
SetPortrait(Format("%s%d", strGraphics, 1 + (Random(iPortraitCount) ?? 0)), pClonk, CLNK, nil, nil);}

if(And(And((stuufe ?? 0)>=10, (stuufe ?? 0)<=14), !(pClonk->~IstBauarbeiter())))
{SetPortrait("random", pClonk, S_XX); SetGraphics("SG1",pClonk);}  //Hazardclonk mit SG1 Grafik

// Bisher nur eine Grafik für alle Stufen ab 10
if(And(And((stuufe ?? 0)>=10, (stuufe ?? 0)<=20), pClonk->~IstBauarbeiter()))
{ SetPortrait("random", pClonk, X_12); 
SetGraphics("Bauer",pClonk);}  //Hazardclonk mit Arbeiter Grafik

if(And(And((stuufe ?? 0)>=15, (stuufe ?? 0)<=19), !(pClonk->~IstBauarbeiter())))
{SetGraphics("",pClonk);
  SetPortrait(GetPortrait(pClonk, nil, 1), pClonk, GetPortrait(pClonk, 1, 1), nil, nil);}  //Hazardclonk Grafik

if(And(Equal(stuufe, 20), !(pClonk->~IstBauarbeiter())))
{SetGraphics("Future",pClonk);
SetPortrait("FutureP", pClonk, HZCK);} //Hazardclonk Future Grafik, Future Portrait
}
  // reset graphics and portrait
else{  SetGraphics("",pClonk);
  SetPortrait(GetPortrait(pClonk, nil, 1), pClonk, GetPortrait(pClonk, 1, 1), nil, nil);}
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
  if(Equal(pClonk -> IsPinguin(), 1)) pClonk->Pinguinaus();
  if(pNightVision) RemoveObject(pNightVision);
}

/* Damage reduction */

public func OnClonkDmg(int iDamage, int iType, object pClonk)
{
  iDamage ??= 0;
  iType ??= 0;
  if(!pClonk) return;
  //Melee
  if(Equal(iType, DMG_Melee))
    return(nil);     // 40% Melee
  //Projectile
  if(Equal(iType, DMG_Projectile))
    return(nil);     // 70% Laser
  //Flame
  if(Equal(iType, DMG_Fire))
    return(nil);   // 50% Fire
  //Plazma
  if(Equal(iType, DMG_Bio))
    return(nil);     // 60% Plasma
  //Explosion
  if(Equal(iType, DMG_Explosion))
    return(nil);     // 40% Explosion
  //Energy
  if(Equal(iType, DMG_Energy))
    return(nil);    // 60% Electric
  //Stahlmantelkugeln
  if(Equal(iType, DMG_BetterProjectile))
    return(nil);     // Stahlmantelkugeln durchschlagen Rüstungen einfach
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
      return;
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