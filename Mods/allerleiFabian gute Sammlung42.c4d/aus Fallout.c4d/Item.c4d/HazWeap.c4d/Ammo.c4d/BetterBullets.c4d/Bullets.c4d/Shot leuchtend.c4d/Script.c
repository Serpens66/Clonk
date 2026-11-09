#strict 3

/* Schuss */

#include SHT_


// Extern für Überladung
private func CreateTrail(int iSize, int iTrail) {
  iSize ??= 0;
  iTrail ??= 0;
  pTrail = CreateObject(TRA2,nil,nil,GetOwner());
  if(pTrail) {
    pTrail->Set((iSize ?? 0)-2,iTrail,this());
    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }
}

/* Timer */

private func Traveling()
{
return(_inherited());
}


public func BulletStrike(object pObj) {
  if(pObj)
    DoDmg(iDamage,DMG_BetterProjectile,pObj,iPrec);
}
