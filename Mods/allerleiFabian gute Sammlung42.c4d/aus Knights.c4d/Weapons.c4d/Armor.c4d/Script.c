/*-- Rüstung --*/

#strict 3

/* Status */
public func IsArmor()
{
  return(1);
}

/* Schutz */
public func ShieldFactor()
{
  return(50);
}

/* Grafik */
public func GetGraphics()
{
  return("Armored");
}

/* Anlegen */
public func Activate(pClonk)
{                  
  [$CtrlActivateDesc$]
  // Der Clonk kann dieses Objekt nicht verwenden
  if (!pClonk->~CanUse(GetID())) {return(nil);}
  // Ritter trägt bereits eine Rüstung?
  if (pClonk->KNIG::HasArmor()) {return(nil);}
  // Anlegen  
  return(pClonk->WearArmor(this()));
}
  
/* Aufschlag */  
protected func Hit()
{
  Sound("ClonkHit*");
}

/* Kann geschmiedet werden */
protected func IsAnvilProduct() { return(1); }
