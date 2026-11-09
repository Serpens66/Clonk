/*-- Combat Armor --*/

#strict
#include GEAR

local pNightVision, superfertigkeit;	// saves the nightvision-light
local eeeeee, eeeeee2;
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
fertig=1;
eeeeee=0;
angelegt=1;
superfertigkeit=0;
  SetPhysical("Fight", GetPhysical("Fight", PHYS_Current, pClonk) +10000, PHYS_StackTemporary, pClonk);
  SetPhysical("Throw", GetPhysical("Throw", PHYS_Current, pClonk) +0, PHYS_StackTemporary, pClonk);
  SetPhysical("Jump", GetPhysical("Jump", PHYS_Current, pClonk) - 2000, PHYS_StackTemporary, pClonk);
  SetPhysical("Walk", GetPhysical("Walk", PHYS_Current, pClonk) - 7000, PHYS_StackTemporary, pClonk);
  SetPhysical("Scale", GetPhysical("Scale", PHYS_Current, pClonk) - 4000, PHYS_StackTemporary, pClonk);
  SetPhysical("Hangle", GetPhysical("Hangle", PHYS_Current, pClonk) - 4000, PHYS_StackTemporary, pClonk);
  SetPhysical("Swim", GetPhysical("Swim", PHYS_Current, pClonk) - 5000, PHYS_StackTemporary, pClonk);
  SetPhysical("Breath", GetPhysical("Breath", PHYS_Current, pClonk) - 5000, PHYS_StackTemporary, pClonk);
  SetPhysical("Dig", GetPhysical("Dig", PHYS_Current, pClonk) - 5000, PHYS_StackTemporary, pClonk);
  SetPhysical("Energy", GetPhysical("Energy", PHYS_Current, pClonk) +10000, PHYS_StackTemporary, pClonk);
DoEnergy(10000,pClonk,true);
}

func MakeGraphic(pClonk)
{
  var iPortraitCount = 3; 
  var strGraphics = "Spezial";
  var a ="a";
  SetGraphics(strGraphics,pClonk);
  SetPortrait("Kampf", pClonk, S_CA, 0, 0);
}

public func GearEffectReset(object pClonk)
{
if(superfertigkeit==1) {eeeeee=1;eeeeee2=0;};
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
else {  SetGraphics("",pClonk);
  SetPortrait(GetPortrait(pClonk, 0, 1), pClonk, GetPortrait(pClonk, 1, 1), 0, 0);}
// falls fähigkeit an, deaktivieren
if(superfertigkeit!=0) ConUse2(pClonk);
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
DoEnergy(-10000,pClonk,true);
  if(pNightVision) RemoveObject(pNightVision);
}

/* Damage reduction */

public func OnClonkDmg(int iDamage, int iType, object pClonk)
{
  if(!pClonk) return();
  //Melee
  if(iType == DMG_Melee)
    return(40);     // 40% Melee
  //Projectile
  if(iType == DMG_Projectile)
    return(60);     // 60% Laser 
  //Flame
  if(iType == DMG_Fire)
    return(30);   // 30% Fire 
  //Plazma
  if(iType == DMG_Bio)
    return(50);     // 50% Plasma 
  //Explosion
  if(iType == DMG_Explosion)
    return(40);     // 40% Explosion
  //Energy
  if(iType == DMG_Energy)
    return(50);    // 50% Electric 
  //Stahlmantelkugeln
  if(iType == DMG_BetterProjectile)
    return(20);     // Stahlmantelkugeln durchschlagen Rüstungen einfach
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



public func ConDesc2() {
  if(superfertigkeit) return("$CtxFertigkeitOff$");
  return("$CtxFertigkeitOn$");
}

public func ConUse2(object pHazard) {
derclonk=pHazard;
  // turn on

  if(superfertigkeit==0) {
if(bereit==1) {
eeeeee2=1;
superfertigkeit=1;
  SetPhysical("Fight", GetPhysical("Fight", PHYS_Current, pHazard) +0, PHYS_StackTemporary, pHazard);
  SetPhysical("Throw", GetPhysical("Throw", PHYS_Current, pHazard) +5000, PHYS_StackTemporary, pHazard);
  SetPhysical("Jump", GetPhysical("Jump", PHYS_Current, pHazard) +12000, PHYS_StackTemporary, pHazard);
  SetPhysical("Walk", GetPhysical("Walk", PHYS_Current, pHazard) +17000, PHYS_StackTemporary, pHazard);
  SetPhysical("Scale", GetPhysical("Scale", PHYS_Current, pHazard) +12000, PHYS_StackTemporary, pHazard);
  SetPhysical("Hangle", GetPhysical("Hangle", PHYS_Current, pHazard) +12000, PHYS_StackTemporary, pHazard);
  SetPhysical("Swim", GetPhysical("Swim", PHYS_Current, pHazard) +20000, PHYS_StackTemporary, pHazard);
  SetPhysical("Breath", GetPhysical("Breath", PHYS_Current, pHazard) +20000, PHYS_StackTemporary, pHazard);
  SetPhysical("Dig", GetPhysical("Dig", PHYS_Current, pHazard) +17000, PHYS_StackTemporary, pHazard);
  SetPhysical("Energy", GetPhysical("Energy", PHYS_Current, pHazard) +0, PHYS_StackTemporary, pHazard);
aufladen=0;
Sound("Energize",0,derclonk);
  };
if (bereit==0) {PlayerMessage(GetOwner(pHazard),Format("Aufladestatus  %v/1500",aufladen),pHazard);};
 }
  // turn off
  else {

superfertigkeit=0;
eeeeee2=0;
bereit=0;
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
Sound("DeEnergize",0,derclonk);
  };
  // Attentioooon: Here comes the effect!
  // ..
  // .....!
  Sound("Click"); // Ba-bum.
return(1);
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


func autodeactivate()
{
if(eeeeee==1)
{

eeeeee=0;
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
Sound("DeEnergize",0,derclonk);
};
}



public func Fertigkeit()
{

if(superfertigkeit==1) 
{++fertig;


if (fertig>=1000) 
{eeeeee=1;fertig=0;bereit=0;superfertigkeit=0;eeeeee2=0;autodeactivate();};  }

if(superfertigkeit==0 && angelegt==1)   
{++aufladen; 

if (aufladen>=1500) 
{bereit=1; eeeeee=0; }; };
return(1);
}


public func IsAnvilProduct() { return(1); }