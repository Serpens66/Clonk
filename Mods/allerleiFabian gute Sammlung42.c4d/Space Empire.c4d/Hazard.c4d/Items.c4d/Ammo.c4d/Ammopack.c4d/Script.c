#strict 3

local verbraucht, value, restmunition, faktor;

public func IsAmmoPacket() { return(true); }
public func AmmoID()       { return(STAM); }
public func AmmoCount()    { return(50); }
public func AmmoCountRest() {return((AmmoCount() ?? 0)-(verbraucht ?? 0)); }

// Damit eine variable Menge ausgegeben wird:
/* Initialisierung */

protected func Initialize()
{
  restmunition=AmmoCount();
  value = GetDefCoreVal("Value", "DefCore");
  faktor = ((value ?? 0)*100)/(restmunition ?? 0);
}


/* Wert & Verkauf */

protected func CalcValue()
{
  var lastvalue = value;
  value = ((restmunition ?? 0)*(faktor ?? 0))/100;
  return(lastvalue);
}

public func HasAdditionalDesc(...) { return(1);
}

public func GiveAdditionalDesc(...) {
  return(Format("Inhalt: %d Munition",restmunition));
}


func RestAmmoreduzieren(zahl)
{
S42MigrationAssign(verbraucht, "+=", (zahl ?? 0));
//Log("Ammo reduziert");
restmunition=AmmoCountRest();
if((restmunition ?? 0)<=0) RemoveObject();
return(1);
}
















protected func Hit()
{
Sound("RockHit*");
return (1);
}

private func OnTransfer() {}

protected func Activate(object pObj) {		// Doppelgraben
  return(TransferAmmo(pObj));
}

public func MayTransfer(object pObj)
{
  if(!pObj) return(nil);
  var MaxAmmo;
  MaxAmmo = AmmoID()->~MaxAmmo(pObj);
  if(MaxAmmo)
// geändert, weil durch variable werte immer das maximum erreicht werden kann
    if(Equal(GetAmmo(AmmoID(),pObj), MaxAmmo))
      return(nil);
  return(true);
}

public func TransferAmmo(object pObj)		// Ammo dem Clonk geben
{
if(!Equal(GetID(pObj), HZCK)) {PlayerMessage(GetOwner(pObj),"Nur Hazardclonks können Munition aufnehmen",pObj); return(nil);}

  var MaxAmmo;
  MaxAmmo = AmmoID()->~MaxAmmo(pObj);

  if(!pObj) return(nil);
  if(NoAmmo()) return(nil);
  
  // nicht wenn er schon zu viel hat
  if(!MayTransfer(pObj)) {
    PlayerMessage(GetOwner(pObj),"$NotMoreAmmo$",pObj,AmmoID());
    return;
  }

    if((GetAmmo(AmmoID(),pObj) ?? 0) + (AmmoCountRest() ?? 0) <= (MaxAmmo ?? 0))
{
  // Nachricht ausgeben
  HelpMessage(GetOwner(pObj),"$Collected$",pObj,AmmoCountRest(),AmmoID());
  DoAmmo(AmmoID(),AmmoCountRest(),pObj);
  pObj->~AmmoTransferred();
  if(!OnTransfer()) RemoveObject();
  return(true);}

// wird auf MaxAmmo aufgestockt und die Restmunition im Pack reduziert
    if((GetAmmo(AmmoID(),pObj) ?? 0) + (AmmoCountRest() ?? 0) > (MaxAmmo ?? 0))
{
var zahl=((MaxAmmo ?? 0)-(GetAmmo(AmmoID(),pObj) ?? 0));
  // Nachricht ausgeben
  HelpMessage(GetOwner(pObj),"$Collected$",pObj,((MaxAmmo ?? 0)-(GetAmmo(AmmoID(),pObj) ?? 0)),AmmoID());
  DoAmmo(AmmoID(),((MaxAmmo ?? 0)-(GetAmmo(AmmoID(),pObj) ?? 0)),pObj);
  pObj->~AmmoTransferred();
  if(!OnTransfer()) {RestAmmoreduzieren(zahl);}

  return(true);}

}


protected func Damage()
{
  // Explodiert nicht, wenn er von einem Clonk getragen wird (Sonderregel gegen T-Flint-Superjumps)
  if (Contained())
    if (Contained()->~IsClonk())
    {
      Extinguish();
      return(nil);
    }
  // Explosion - verzögert, um Endlosrekursion zu verhindern
  Schedule("Explode(20)", 1);
  return(1);
}

// Entzündet sich bei Explosionstreffer
public func OnShockwaveHit(level, x, y)
{
 Incinerate();
}


// public func IsRefineryProduct() { return(1); }