#strict

local verbraucht, value, restmunition, faktor;

public func IsAmmoPacket() { return(true); }
public func AmmoID()       { return(STAM); }
public func AmmoCount()    { return(50); }
public func AmmoCountRest() {return(AmmoCount()-verbraucht); }

// Damit eine variable Menge ausgegeben wird:
/* Initialisierung */

protected func Initialize()
{
  restmunition=AmmoCount();
  value = GetDefCoreVal("Value", "DefCore");
  faktor = (value*100)/restmunition;
}


/* Wert & Verkauf */

protected func CalcValue()
{
  var lastvalue = value;
  value = (restmunition*faktor)/100;
  return(lastvalue);
}

public HasAdditionalDesc: return(1);

public GiveAdditionalDesc:
  return(Format("Inhalt: %d Munition",restmunition));


func RestAmmoreduzieren(zahl)
{
verbraucht+=zahl;
//Log("Ammo reduziert");
restmunition=AmmoCountRest();
if(restmunition<=0) RemoveObject();
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
  if(!pObj) return(false);
  var MaxAmmo;
  MaxAmmo = AmmoID()->~MaxAmmo(pObj);
  if(MaxAmmo)
// geändert, weil durch variable werte immer das maximum erreicht werden kann
    if(GetAmmo(AmmoID(),pObj) == MaxAmmo)
      return(false);
  return(true);
}

public func TransferAmmo(object pObj)		// Ammo dem Clonk geben
{
if(GetID(pObj)!=HZCK) {PlayerMessage(GetOwner(pObj),"Nur Hazardclonks können Munition aufnehmen",pObj); return(0);}

  var MaxAmmo;
  MaxAmmo = AmmoID()->~MaxAmmo(pObj);

  if(!pObj) return(false);
  if(NoAmmo()) return(false);
  
  // nicht wenn er schon zu viel hat
  if(!MayTransfer(pObj)) {
    PlayerMessage(GetOwner(pObj),"$NotMoreAmmo$",pObj,AmmoID());
    return();
  }

    if(GetAmmo(AmmoID(),pObj) + AmmoCountRest() <= MaxAmmo)
{
  // Nachricht ausgeben
  HelpMessage(GetOwner(pObj),"$Collected$",pObj,AmmoCountRest(),AmmoID());
  DoAmmo(AmmoID(),AmmoCountRest(),pObj);
  pObj->~AmmoTransferred();
  if(!OnTransfer()) RemoveObject();
  return(true);}

// wird auf MaxAmmo aufgestockt und die Restmunition im Pack reduziert
    if(GetAmmo(AmmoID(),pObj) + AmmoCountRest() > MaxAmmo)
{
var zahl=(MaxAmmo-GetAmmo(AmmoID(),pObj));
  // Nachricht ausgeben
  HelpMessage(GetOwner(pObj),"$Collected$",pObj,(MaxAmmo-GetAmmo(AmmoID(),pObj)),AmmoID());
  DoAmmo(AmmoID(),(MaxAmmo-GetAmmo(AmmoID(),pObj)),pObj);
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
      return(0);
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