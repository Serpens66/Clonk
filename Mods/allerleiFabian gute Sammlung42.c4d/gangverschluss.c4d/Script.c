/*-- Gangverschluss --*/

#strict

Activate:
  // Aufbauen
  if(Equal(GetDir(Par(0)),DIR_Right())) CreateObject(G15Y, 0, 10, GetOwner());
  if(Equal(GetDir(Par(0)),DIR_Left())) CreateObject(G25Y, 0, 10, GetOwner());
  Sound("Connect");
  RemoveObject();
return(1);

Hit:
  Sound("RockHit*");
  return(1);

/* Produkteigenschaften */

public IsAnvilProduct: return(1);