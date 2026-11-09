/*-- Tesla Armor --*/

#strict 3
#include GEAR

local pNightVision, superfertigkeit;	// saves the nightvision-light
local fffffff;
local fffffff2;
local fertig, bereit, aufladen, angelegt,obbject,derclonk,test;

/* Status */

public func GetGearType()	{ return(GEAR_Armor); }
protected func Hit()		{ Sound("ClonkHit*"); }
public func IsEquipment()	{ return(true); }

/* Callbacks */

public func GearEffect(object pClonk)
{
derclonk=pClonk;
  // Change graphics and portrait
  MakeGraphic(pClonk);
  Sound("Connect");
fertig=1;
fffffff=nil;
angelegt=1;
superfertigkeit=nil;
}

func MakeGraphic(pClonk)
{
    var iPortraitCount = 2; 
  var strGraphics = "ArmorTA";
  SetGraphics(strGraphics,pClonk);
  SetPortrait(Format("%s%d", strGraphics, 1 + (Random(iPortraitCount) ?? 0)), pClonk, S_TA, nil, nil);
}

public func GearEffectReset(object pClonk)
{
if(Equal(superfertigkeit, 1)) {fffffff=1;fffffff2=nil;};
angelegt=nil;

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
// falls fähigkeit an, deaktivieren
if(!Equal(superfertigkeit, nil)) ConUse2(pClonk);
  if(pNightVision) RemoveObject(pNightVision);

return;
}

/* Damage reduction */

public func OnClonkDmg(int iDamage, int iType, object pClonk)
{
  iDamage ??= 0;
  iType ??= 0;
  if(!pClonk) return;
  //Melee
  if(Equal(iType, DMG_Melee))
    return(nil);     // 20% Melee
  //Projectile
  if(Equal(iType, DMG_Projectile))
    return(-30);     // 90% Laser 
  //Flame
  if(Equal(iType, DMG_Fire))
    return(nil);   // 10% Fire
  //Explosion
  if(Equal(iType, DMG_Explosion))
    return(nil);     // 20% Explosion
  //Energy
  if(Equal(iType, DMG_Energy))
    return(100);    // 80% Electric 
  //Plazma
  if(Equal(iType, DMG_Bio))
    return(10);     // 80% Plasma 
  //Stahlmantelkugeln
  if(Equal(iType, DMG_BetterProjectile))
    return(-40);     // Stahlmantelkugeln durchschlagen Rüstungen einfach
}

/* Use per context menu of the hazard clonk */

public func ConDesc() {
  if(pNightVision) return("$CtxNightVisionOff$");
  return("$CtxNightVisionOn$");
}

public func ConDesc2() {
  if(superfertigkeit) return("$CtxFertigkeitOff$");
  return("$CtxFertigkeitOn$");
}

public func ConUse2(object pHazard) {

  // turn on
  if(Equal(superfertigkeit, nil)) {
if(Equal(bereit, 1)) {
fffffff2=1;
AutoDeactivate(pHazard);
superfertigkeit=1;
  SetPhysical("Fight", (GetPhysical("Fight", PHYS_Current, pHazard) ?? 0) +0, PHYS_StackTemporary, pHazard);
  SetPhysical("Throw", (GetPhysical("Throw", PHYS_Current, pHazard) ?? 0) +0, PHYS_StackTemporary, pHazard);
  SetPhysical("Jump", (GetPhysical("Jump", PHYS_Current, pHazard) ?? 0) +0, PHYS_StackTemporary, pHazard);
  SetPhysical("Walk", (GetPhysical("Walk", PHYS_Current, pHazard) ?? 0) +0, PHYS_StackTemporary, pHazard);
  SetPhysical("Scale", (GetPhysical("Scale", PHYS_Current, pHazard) ?? 0) +0, PHYS_StackTemporary, pHazard);
  SetPhysical("Hangle", (GetPhysical("Hangle", PHYS_Current, pHazard) ?? 0) +0, PHYS_StackTemporary, pHazard);
  SetPhysical("Swim", (GetPhysical("Swim", PHYS_Current, pHazard) ?? 0) +0, PHYS_StackTemporary, pHazard);
  SetPhysical("Breath", (GetPhysical("Breath", PHYS_Current, pHazard) ?? 0) +0, PHYS_StackTemporary, pHazard);
  SetPhysical("Dig", (GetPhysical("Dig", PHYS_Current, pHazard) ?? 0) +0, PHYS_StackTemporary, pHazard);
  SetPhysical("Energy", (GetPhysical("Energy", PHYS_Current, pHazard) ?? 0) +0, PHYS_StackTemporary, pHazard);
aufladen=nil;
AddEffect("Sparklez", pHazard, 1, 1,pHazard);
  AddEffect("Radiatefffffffz", pHazard, 1, 1,pHazard);
Sound("Energize",nil,derclonk);
  };
if (Equal(bereit, nil)) {PlayerMessage(GetOwner(pHazard),Format("Aufladestatus  %v/2000",aufladen),pHazard);};
 }
  // turn off
  else {
superfertigkeit=nil;
bereit=nil;
fffffff2=nil;
fertig=nil;
  ResetPhysical(pHazard, "Fight");
  ResetPhysical(pHazard, "Throw");
  ResetPhysical(pHazard, "Walk");
  ResetPhysical(pHazard, "Scale");
  ResetPhysical(pHazard, "Hangle");
  ResetPhysical(pHazard, "Jump");
  ResetPhysical(pHazard, "Swim");
  ResetPhysical(pHazard, "Breath");
  ResetPhysical(pHazard, "Dig");
  ResetPhysical(pHazard, "Energy");
RemoveEffect("Sparklez", pHazard);
RemoveEffect("Radiatefffffffz", pHazard);
Sound("DeEnergize",nil,derclonk);
  };
  // Attentioooon: Here comes the effect!
  // ..
  // .....!
  Sound("Click"); // Ba-bum.
return(1);
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


func AutoDeactivate()
{

if(Equal(fffffff, 1))
{
fffffff=nil;
  ResetPhysical(derclonk, "Fight");
  ResetPhysical(derclonk, "Throw");
  ResetPhysical(derclonk, "Walk");
  ResetPhysical(derclonk, "Scale");
  ResetPhysical(derclonk, "Hangle");
  ResetPhysical(derclonk, "Jump");
  ResetPhysical(derclonk, "Swim");
  ResetPhysical(derclonk, "Breath");
  ResetPhysical(derclonk, "Dig");
  ResetPhysical(derclonk, "Energy");
RemoveEffect("Sparklez", derclonk);
RemoveEffect("Radiatefffffffz", derclonk);
Sound("DeEnergize",nil,derclonk);
};
}






public func Fertigkeit()
{
if(Equal(superfertigkeit, 1))
{S42MigrationStep(fertig, 1);


if ((fertig ?? 0)>=500)
{fffffff=1;fertig=nil;bereit=nil;superfertigkeit=nil;fffffff2=nil;AutoDeactivate();};  }

if(And(Equal(superfertigkeit, nil), Equal(angelegt, 1)))
{S42MigrationStep(aufladen, 1);

if ((aufladen ?? 0)>=2000)
{bereit=1;fffffff=nil; }; };
return(1);
}


public func IsAnvilProduct() { return(1); }