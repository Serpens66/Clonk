/*-- Schlacke - Müllgrundobjekt --*/

#strict

// Eher Unschädlich für die Natur
public func IsWaste(){ return(6);}
public func PackTo() { return(_SKA);}

public func Initialize()
{
    if(FindObject(NOWS)) {RemoveObject();return();} // kein Müll, dann sofort wieder entfernen
}

public func Hit()
{
  return Sound("RockHit*");
}

/* Einsammeln */

public func Entrance(pClonk)
{
  // Nicht im Müllpacket selber
  if(GetID(pClonk) == PackTo()) return();

  // Nicht wenn unterdrückt
  if(GetEffect("IntNoPacking", pClonk)) return();

  // Zu einem Pack dazufügen
  return(Pack(pClonk));
}

/* Packen */

protected func Activate(pClonk)
{
  [Packen]
  return (Pack(pClonk));
}

public func Pack(pClonk)
{
  // Andere Einzelobjekte suchen und packen
  for(var obj in FindObjects(Find_Exclude(this()), Find_Container(pClonk), Find_ID(GetID())))
  {
    obj->~JoinPack(pClonk, 1);
  }
  // und selber zu Pack hinzufügen
  JoinPack(pClonk,1);
  return(1);
}

public func JoinPack(pClonk,fForce)
{
  // Nicht wenn unterdrückt
  if(GetEffect("IntNoPacking", pClonk)) return();
  if(GetEffect("IntArrowVanish", this())) return();

  // Müllpakete im Clonk suchen
  for(var obj in FindObjects(Find_Container(pClonk), Find_ID(PackTo())))
  {
    // Noch Platz?
    if( obj->MaxPackCount() > obj->PackCount() )
    {
      // Integrieren
      obj->DoPackCount(1);
      return(RemoveObject());
    }
  }
  // Nur bei passendem Flag Aufnahme erzwingen
  if(!fForce) return();
  // Neues Pack anfangen
  CreateContents(PackTo(), pClonk)->SetPackCount(1);
  return(RemoveObject());
}
