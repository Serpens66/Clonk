#strict

/* Schuss */

#include SHTG


// Extern für Überladung
private func CreateTrail(int iSize, int iTrail) {
  pTrail = CreateObject(TRA2,0,0,GetOwner());
  if(pTrail) {
    pTrail->Set(iSize-2,iTrail,this());
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