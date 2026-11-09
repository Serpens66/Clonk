/*-- Schleimklumpen --*/

#strict 3

local target, // Klebezielobjekt
      damage; // Evtl. Schaden

func Initialize() {
  SetR(Random(360));
  AddAlienFlareEffect(this(),100);
  //AddFireEffect(this(),0,RGB(20,255,30),0,-20);
}

/* Kleben am Clonk */

public func AttachTo(pClonk)    // An einem Clonk heften
{
  SetVertexXY(nil, (GetX() ?? 0)-(GetX(pClonk) ?? 0)+(GetVertex(nil, nil, pClonk) ?? 0), (GetY() ?? 0)-(GetY(pClonk) ?? 0)+(GetVertex(nil, 1, pClonk) ?? 0));
  SetAction("AGlobbing", pClonk);

  if(!DecreasePhysical(pClonk)) return(RemoveObject());
}

private func DecreasePhysical(object pClonk)
{
  // Maximal 30% des Gesamtwertes von Walk ~!/$&)=%/
  if((GetPhysical("Walk", 2, pClonk) ?? 0) <= (GetPhysical("Walk", 1, nil, GetID(pClonk)) ?? 0)*7/10)
    return(nil);
  // Effekt starten
  AddEffect("Slimed", pClonk, 50, 800, nil, GetID());
  return(1);
}

/* Kleben am Boden */

public func Glob2Ground(iDamage)
{
  damage = iDamage;
  SetAction("Globbing");
}

protected func Check()
{
  var living;
  while(living = FindObject(nil, -5,-5, 10,10, OCF_Living, nil,nil, NoContainer(), living))
    if(And(GetAlive(living), !(living)->~IsAlien()))
      return(HitLiving(living));
  while(living = FindObject(nil, 1,1, nil,nil, OCF_Living, nil,nil, NoContainer(), living))
    if(And(GetAlive(living), !(living)->~IsAlien()))
      return(HitLiving(living));

  if((GetActTime() ?? 0) >= 600) return(Boom());

}

private func HitLiving(target)
{
  // Maximal 30% des Gesamtwertes von Walk ~!/$&)=%/
  if(!((GetPhysical("Walk", 2, target) ?? 0) <= (GetPhysical("Walk", 1, nil, GetID(target)) ?? 0)*7/10))
    // Effekt starten
    AddEffect("Slimed", target, 101, 1500, nil, GetID());

  // Schaden roflol
  DoDmg(damage, DMG_Bio, target);
  // Weg
  Boom();
}

/* Schleimdingsis */

public func Boom()
{
  // Weg
  RemoveObject();
}

protected func AttachTargetLost()
{
  RemoveObject();
}

/* Schleim-Effekt */

public func FxSlimedStart(target, no, temp,int iStrength)
{
  iStrength ??= 0;
  if(temp)
    if((GetPhysical("Walk", 2, target) ?? 0) <= (GetPhysical("Walk", 1, nil, GetID(target)) ?? 0)*7/10)
      return(nil);

  // Variablen
  // 0: Walk
  // 1: Jump
  // 2: Scale
  // 3: Hangle
  EffectVar(nil, target, no) = (GetPhysical("Walk", 1, nil, GetID(target)) ?? 0)*1/10;
  EffectVar(1, target, no) = (GetPhysical("Jump", 1, nil, GetID(target)) ?? 0)*1/10;
  EffectVar(2, target, no) = (GetPhysical("Scale", 1, nil, GetID(target)) ?? 0)*1/10;
  EffectVar(3, target, no) = (GetPhysical("Hangle", 1, nil, GetID(target)) ?? 0)*1/10;

  SetPhysical("Walk", (GetPhysical("Walk", nil, target) ?? 0)-(EffectVar(nil, target, no) ?? 0), 2, target);
  SetPhysical("Jump", (GetPhysical("Jump", nil, target) ?? 0)-(EffectVar(1, target, no) ?? 0), 2, target);
  SetPhysical("Scale", (GetPhysical("Scale", nil, target) ?? 0)-(EffectVar(2, target, no) ?? 0), 2, target);
  SetPhysical("Hangle", (GetPhysical("Hangle", nil, target) ?? 0)-(EffectVar(3, target, no) ?? 0), 2, target);

  // blar
  SetClrModulation(RGB(200,255,200), target);
}

public func FxSlimedTimer()
{
  return(-1);
}

public func FxSlimedStop(target, no, reason, temp)
{
  SetPhysical("Walk", (GetPhysical("Walk", nil, target) ?? 0)+(EffectVar(nil, target, no) ?? 0), 2, target);
  SetPhysical("Jump", (GetPhysical("Jump", nil, target) ?? 0)+(EffectVar(1, target, no) ?? 0), 2, target);
  SetPhysical("Scale", (GetPhysical("Scale", nil, target) ?? 0)+(EffectVar(2, target, no) ?? 0), 2, target);
  SetPhysical("Hangle", (GetPhysical("Hangle", nil, target) ?? 0)+(EffectVar(3, target, no) ?? 0), 2, target);

  // blar
  if(!GetEffect("*Slimed*", target)) SetClrModulation(RGB(nil,nil,nil), target);

  if(temp) return(nil);

  // Schleimklumpen entfernen
  var glob = FindObject(GLOB, nil,nil,nil,nil,nil, "AGlobbing", target);
  if(glob) glob->~Boom();
}

// Feuer! :o
protected func Incineration() { Boom(); }
