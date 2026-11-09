/*-- Schutzanzug --*/

#strict 3
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
  SetPortrait(Format("%s%d", strGraphics, 1 + (Random(iPortraitCount) ?? 0)), pClonk, _Y4C, nil, nil);
}

public func GearEffectReset(object pClonk)
{

ResetPhysical(pClonk, "CorrosionResist");

// Grafik je nach Stufe zurücksetzen... geht ab jetzt auch direkt in hazardclonk mit UpdatePortrait()
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
 else{ SetGraphics("",pClonk);
  SetPortrait(GetPortrait(pClonk, nil, 1), pClonk, GetPortrait(pClonk, 1, 1), nil, nil);}
  Sound("Connect");
pClonk->Schutzanzugaus();
}

/* Damage reduction */

public func OnClonkDmg(int iDamage, int iType, object pClonk)
{
  iDamage ??= 0;
  iType ??= 0;
  if(!pClonk) return;
  //Melee
  if(Equal(iType, DMG_Melee))
    return(1);     // 
  //Projectile
  if(Equal(iType, DMG_Projectile))
    return(1);     // 
  //Flame
  if(Equal(iType, DMG_Fire))
    return(1);   // 
  //Explosion
  if(Equal(iType, DMG_Explosion))
    return(1);     // 
  //Energy
  if(Equal(iType, DMG_Energy))
    return(1);    // 
  //Plazma
  if(Equal(iType, DMG_Bio))
    return(1);     // 
  //Stahlmantelkugeln
  if(Equal(iType, DMG_BetterProjectile))
    return(1);     // Stahlmantelkugeln durchschlagen Rüstungen einfach
}


public func IsAnvilProduct() { return(1); }