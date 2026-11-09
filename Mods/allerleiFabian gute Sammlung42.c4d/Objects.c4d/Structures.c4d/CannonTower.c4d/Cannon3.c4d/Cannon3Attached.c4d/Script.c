/*--- Luftabwehrgesch¸tz (angeschraubt) ---*/

#strict 3

#include CTW2

protected func RotationSpeed() { return(6); }
protected func CannonMobileID() { return(CTW7); }

/* Munition aufnehmen */

public func ComEnter(object pAmmo)
{
  // Gepackte Muni
  if(Or(pAmmo->~IsArrowPack(), Equal(GetID(pAmmo), ADM1)))
  {
    // Aufnehmen
    Sound("Connect");
    if (Equal(GetID(pAmmo), ADM1)) pAmmo->~Unpack(this()); else Enter(this(), pAmmo);
  }
  // Einzelne Munitionskomponenten
  else if(Or(pAmmo->~IsArrow(), Equal(GetID(pAmmo), ADB1)))
  {
    Sound("Click");
    Enter(this(), pAmmo);
  }
}

public func ComFire(object pClonk)
{
  Sound("Click");
  SetAction("Attaching", GetActionTarget() );
  SetRDir(nil);
    
  // Bis zu 2 Projektile verschieﬂen
  for(var i = nil; (i ?? 0) < 2; S42MigrationStep( i, 1))
  {
    // Projektil suchen
    var c = (ContentsCount() ?? 0) - 1, obj, ammo;
    while(obj = Contents(S42MigrationStep(c, -1, true)) )
    {
      if (obj->~IsArrowPack())
      {
        ammo = obj->~GetItem();
        if (!ammo)
          {
          // Alter Pfeilpaket: Erstmal nur aufteilen; sp‰ter wieder versuchen (Rekursionsschutz gegen neues Zusammenpacken)
          Sound("Connect");
          Split2Components(obj);
          break;
          }
      }
      else if(Or(obj->~IsArrow(), Equal(GetID(obj), ADB1)))
      {
        ammo = obj;
        break;
      }
    }
  
    // Keins da? :(
    if(!ammo)
    {
      if(!i) Sound("Error");
      else Sound("Click");
      return(nil);
    }

    // Sound    
    if(!i) Sound("Arrow");
    
    
    var speed = 15;
    if(Equal(GetID(ammo), ADB1)) speed = 22;
    
    // Rotationsgeschwindigkeit
    var rdir = 4;
    if((GetR() ?? 0) < 0) rdir = -4;
    if(Inside(GetR(), -10, 10) ) rdir = nil;
    
    // Controller setzen
    ammo->SetController(pClonk->GetController());
    
    // Ausstoﬂ
    Exit(ammo, Sin((GetR() ?? 0) - 45 + 90 * (i ?? 0), 9), -(Cos((GetR() ?? 0) - 45 + 90 * (i ?? 0), 9) ?? 0), GetR(), Sin(GetR(), speed), -(Cos(GetR(), speed) ?? 0), rdir);
    SetPlrView(GetOwner(pClonk), ammo);
    ammo->~Launch();
  }
}

