/*-- Lore --*/

#strict 3

#include LORY

/* Füllmengenkontrolle */

private func MaxContents() { return 10; }

protected func RejectCollect(id idObj,object pObj)
{
  if((ContentsCount() ?? 0) < (MaxContents() ?? 0))
  {
    Sound("WoodHit*");
    return nil;
  }
  if(Contained(pObj)) return Message("$TxtLorryisfull$", this);
  if((Abs(GetXDir(pObj)) ?? 0)>6) SetYDir(-5,pObj);
  Sound("WoodHit*");
  return 1;
}

/* Radkontrolle */
protected func CheckWheel()
{
  // Aktion der Geschwindigkeit anpassen
  if (Abs(GetXDir()))
    SetAction("WheelTurn");
  else SetAction("Idle");
  // Drehung der Richtung anpassen
  if((GetXDir() ?? 0)>0) SetDir(1);
  else if((GetXDir() ?? 0)<0) SetDir(nil);
}