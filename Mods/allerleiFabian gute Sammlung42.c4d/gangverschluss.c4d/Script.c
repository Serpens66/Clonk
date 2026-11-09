/*-- Gangverschluss --*/

#strict 3

func Activate(...) {
  // Aufbauen
  if(Equal(GetDir(Par(nil)),DIR_Right)) CreateObject(G15Y, nil, 10, GetOwner());
  if(Equal(GetDir(Par(nil)),DIR_Left)) CreateObject(G25Y, nil, 10, GetOwner());
  Sound("Connect");
  RemoveObject();
return(1);
}

func Hit(...) {
  Sound("RockHit*");
  return(1);
}

/* Produkteigenschaften */

public func IsAnvilProduct(...) { return(1);
}