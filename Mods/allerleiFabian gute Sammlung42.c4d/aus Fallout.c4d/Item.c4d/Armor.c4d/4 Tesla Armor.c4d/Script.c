/*-- Tesla Armor --*/

#strict
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
fffffff=0;
angelegt=1;
superfertigkeit=0;
}

func MakeGraphic(pClonk)
{
    var iPortraitCount = 2; 
  var strGraphics = "ArmorTA";
  SetGraphics(strGraphics,pClonk);
  SetPortrait(Format("%s%d", strGraphics, 1 + Random(iPortraitCount)), pClonk, S_TA, 0, 0);
}

public func GearEffectReset(object pClonk)
{
if(superfertigkeit==1) {fffffff=1;fffffff2=0;};
angelegt=0;

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
// falls fähigkeit an, deaktivieren
if(superfertigkeit!=0) ConUse2(pClonk);
  if(pNightVision) RemoveObject(pNightVision);

return();
}

/* Damage reduction */

public func OnClonkDmg(int iDamage, int iType, object pClonk)
{
  if(!pClonk) return();
  //Melee
  if(iType == DMG_Melee)
    return(0);     // 20% Melee
  //Projectile
  if(iType == DMG_Projectile)
    return(-30);     // 90% Laser 
  //Flame
  if(iType == DMG_Fire)
    return(0);   // 10% Fire 
  //Explosion
  if(iType == DMG_Explosion)
    return(0);     // 20% Explosion
  //Energy
  if(iType == DMG_Energy)
    return(100);    // 80% Electric 
  //Plazma
  if(iType == DMG_Bio)
    return(10);     // 80% Plasma 
  //Stahlmantelkugeln
  if(iType == DMG_BetterProjectile)
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
  if(superfertigkeit==0) {
if(bereit==1) {
fffffff2=1;
AutoDeactivate(pHazard);
superfertigkeit=1;
  SetPhysical("Fight", GetPhysical("Fight", PHYS_Current, pHazard) +0, PHYS_StackTemporary, pHazard);
  SetPhysical("Throw", GetPhysical("Throw", PHYS_Current, pHazard) +0, PHYS_StackTemporary, pHazard);
  SetPhysical("Jump", GetPhysical("Jump", PHYS_Current, pHazard) +0, PHYS_StackTemporary, pHazard);
  SetPhysical("Walk", GetPhysical("Walk", PHYS_Current, pHazard) +0, PHYS_StackTemporary, pHazard);
  SetPhysical("Scale", GetPhysical("Scale", PHYS_Current, pHazard) +0, PHYS_StackTemporary, pHazard);
  SetPhysical("Hangle", GetPhysical("Hangle", PHYS_Current, pHazard) +0, PHYS_StackTemporary, pHazard);
  SetPhysical("Swim", GetPhysical("Swim", PHYS_Current, pHazard) +0, PHYS_StackTemporary, pHazard);
  SetPhysical("Breath", GetPhysical("Breath", PHYS_Current, pHazard) +0, PHYS_StackTemporary, pHazard);
  SetPhysical("Dig", GetPhysical("Dig", PHYS_Current, pHazard) +0, PHYS_StackTemporary, pHazard);
  SetPhysical("Energy", GetPhysical("Energy", PHYS_Current, pHazard) +0, PHYS_StackTemporary, pHazard);
aufladen=0;
AddEffect("Sparklez", pHazard, 1, 1,pHazard);
  AddEffect("Radiatefffffffz", pHazard, 1, 1,pHazard);
Sound("Energize",0,derclonk);
  };
if (bereit==0) {PlayerMessage(GetOwner(pHazard),Format("Aufladestatus  %v/2000",aufladen),pHazard);};
 }
  // turn off
  else {
superfertigkeit=0;
bereit=0;
fffffff2=0;
fertig=0;
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
Sound("DeEnergize",0,derclonk);
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


func AutoDeactivate()
{

if(fffffff==1)
{
fffffff=0;
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
Sound("DeEnergize",0,derclonk);
};
}






public func Fertigkeit()
{
if(superfertigkeit==1) 
{++fertig;


if (fertig>=500) 
{fffffff=1;fertig=0;bereit=0;superfertigkeit=0;fffffff2=0;AutoDeactivate();};  }

if(superfertigkeit==0 && angelegt==1)   
{++aufladen; 

if (aufladen>=2000) 
{bereit=1;fffffff=0; }; };
return(1);
}


public func IsAnvilProduct() { return(1); }