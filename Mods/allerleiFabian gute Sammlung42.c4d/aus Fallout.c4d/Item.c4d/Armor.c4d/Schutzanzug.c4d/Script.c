/*-- Schutzanzug --*/

#strict
#include GEAR

local derclonk;  // magical physics


/* Status */

public func GetGearType()	{ return(GEAR_Armor); }
protected func Hit()		{ Sound("Munch1"); }
public func IsEquipment()	{ return(true); }

/* Callbacks */

public func GearEffect(object pClonk)
{
derclonk=pClonk;
  // Change graphics and portrait
  MakeGraphic(pClonk);
  Sound("Connect");
  SetPhysical("CorrosionResist", 1, PHYS_StackTemporary, pClonk);
pClonk->Schutzanzugan();
}

func MakeGraphic(pClonk)
{
  var iPortraitCount = 2; 
  var strGraphics = "Schutzanzug";
  SetGraphics(strGraphics,pClonk);
  SetPortrait(Format("%s%d", strGraphics, 1 + Random(iPortraitCount)), pClonk, _Y4C, 0, 0);
}

public func GearEffectReset(object pClonk)
{

ResetPhysical(pClonk, "CorrosionResist");

// Grafik je nach Stufe zurücksetzen... geht ab jetzt auch direkt in hazardclonk mit UpdatePortrait()
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
 else{ SetGraphics("",pClonk);
  SetPortrait(GetPortrait(pClonk, 0, 1), pClonk, GetPortrait(pClonk, 1, 1), 0, 0);}
  Sound("Connect");
pClonk->Schutzanzugaus();
}

/* Damage reduction */

public func OnClonkDmg(int iDamage, int iType, object pClonk)
{
  if(!pClonk) return();
  //Melee
  if(iType == DMG_Melee)
    return(1);     // 
  //Projectile
  if(iType == DMG_Projectile)
    return(1);     // 
  //Flame
  if(iType == DMG_Fire)
    return(1);   // 
  //Explosion
  if(iType == DMG_Explosion)
    return(1);     // 
  //Energy
  if(iType == DMG_Energy)
    return(1);    // 
  //Plazma
  if(iType == DMG_Bio)
    return(1);     // 
  //Stahlmantelkugeln
  if(iType == DMG_BetterProjectile)
    return(1);     // Stahlmantelkugeln durchschlagen Rüstungen einfach
}


public func IsAnvilProduct() { return(1); }