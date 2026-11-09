/*-- VehicleHUD --*/

#strict 3

local clonk,target,
      //alpha,
      flash,
      charge, recharge, modusName,
      fuelID, fuelCount, fuelLoad, masterFuelCount,
      infoID;

public func Set(object pTarget, object pClonk)
{
  target = pTarget;
  clonk = pClonk;
  SetOwner(GetOwner(pClonk));
  Timer();
}
      
protected func Initialize()
{
  // Einstellungen
  SetVisibility(VIS_Owner);
  SetPosition(75, -400);
  // Infos
  SetGraphics("Row", this(), GetID(), 1, 1);
  //alpha = 150;
  infoID = CreateObject(2HUD, nil,nil, GetOwner());
  infoID->SetPosition(150, -400);
  // Update
  Timer();
  return(1);
}

protected func Destruction()
{
  if(infoID) infoID->RemoveObject();
}

/* Timer */

protected func Timer()
{
  // Spieler weg?
  if(Or(Equal(GetOwner(), NO_OWNER), !GetPlayerName(GetOwner()))) return(SetVisibility(VIS_None));
  
  // Clonk falsch?
  SetVisibility(VIS_Owner);
  SetVisibility(VIS_Owner,infoID);
  if(Or(!Equal(GetCursor(GetOwner()), clonk), NoAmmo()))
  {
    SetVisibility(VIS_None);
    SetVisibility(VIS_None,infoID);
  } 
  
  if(!target) return;
  if(!clonk) return;
  
  target->~UpdateHUD(this());
  
  // Faden
  /*if(alpha < 150) alpha++;
  if(alpha > 150 && charge != 0) alpha = 150;
  SetClrModulation(RGBa(255,255,255, alpha));
  SetClrModulation(RGBa(255,255,255, alpha), 0, 1);
  SetClrModulation(RGBa(255,255,255, alpha), infoID);*/
  
  if(flash)
  {
    var mod  = Sin(flash,100);
    SetClrModulation(RGBa(255,255,255,mod));
    S42MigrationAssign(flash, "+=", 10);
  }
  
  // Anzeige
  SetObjDrawTransform((charge ?? 0) * 10, nil, -1000 * (100-(charge ?? 0))/2,nil, 1000, nil, this(), 1);
  if(recharge)
  {
    SetGraphics("Red", this(), GetID(), 2, 2, "Red");
    SetObjDrawTransform(1000, nil, 50000 -1000 * (100-(recharge ?? 0)),nil, 1000, nil, this(), 2);
  }
  else
    SetGraphics(nil,nil,nil, 2);
}

/* Aufrufe */

public func Charge(int iCharge)
{
  iCharge ??= 0;
  if(Equal(iCharge, charge)) return;
  charge = iCharge;
  //alpha = 80;
  //if(!charge) alpha = 200;
  return(1);
}

public func Recharge(int iRecharge)
{
  iRecharge ??= 0;
  if(Equal(iRecharge, recharge)) return;
  recharge = iRecharge;
  return(1);
}

public func Fuel(int iFuelCount, int iFuelLoad, string sModusName)
{
  iFuelCount ??= 0;
  iFuelLoad ??= 0;
  if(And(And(Equal(iFuelCount, fuelCount), Equal(iFuelLoad, fuelLoad)), Equal(sModusName, modusName))) return;
  fuelCount = iFuelCount;
  fuelLoad = iFuelLoad;
  modusName = sModusName;
  var color = "ffff00";
  if(!fuelCount) color = "ff0000";
    Message("@%s|<c %s>%d/%d</c>", this(), modusName, color, fuelCount, fuelLoad);
  return(1);
}

public func MasterFuel(id idFuelID, int iMasterFuelCount)
{
  iMasterFuelCount ??= 0;
  if(!infoID) return; // Panique!
  if(And(Equal(idFuelID, fuelID), Equal(iMasterFuelCount, masterFuelCount))) return;
  fuelID = idFuelID;
  masterFuelCount = iMasterFuelCount;
  SetGraphics(nil, infoID, fuelID, 1, 4);
  //if(ammoID) SetClrModulation(RGBa(255,255,255, 128), infoID, 1);
  if((masterFuelCount ?? 0) > -1) {
    var color = "ffff00";
    if(!masterFuelCount) color = "ff0000";
    Message("@<c %s>%d</c>", infoID, color, masterFuelCount);
  }
  else
    Message("", infoID);
  return(1);
}

public func Clear()
{
  Charge();
  Recharge();
  Fuel();
  MasterFuel(nil, -1);
}

public func SetFlash(bool fFlash)
{
  fFlash ??= false;
  if(fFlash)
  {
   if(!flash)
     flash = 1;
  }
  else
  {
    flash = nil;
    SetClrModulation(RGBa(255,255,255));
  }
}