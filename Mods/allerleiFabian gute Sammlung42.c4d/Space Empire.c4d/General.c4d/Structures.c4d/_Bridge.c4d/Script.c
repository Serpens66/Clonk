/*-- Brückensegment --*/

#strict 3

local cons;   // frei oder verankert?


/* Initialisierung */

protected func Initialize(...) {
  // Im Freien erzeugte Brückensegmente sind verankert
  if (!Contained())
  {
    SetSolidMask(nil,nil,72,3);
    cons = 1;
  }
  return(1);
}

protected func Entrance(...) {
  // Im Behälter: lösen
  SetSolidMask();
  cons = nil;
  return(1);
}

public func ControlDigDouble(...) {
  [Verankern/Lösen]
  // Lösen
  if (cons) return(Release());  
  // Verankern
  if (!GBackSolid(nil,+11))
    SetPosition(GetX(),(GetY() ?? 0)+11);
  SetSolidMask(nil,nil,72,3);
  Sound("Connect");
  cons = 1;
  return(1);
}

private func Release(...) {
  SetSolidMask();
  cons = nil;
  Sound("Connect");
  // Aus der Erde lösen
  var i;
  while (And(Stuck(), S42MigrationStep(i, 1) < 12))
    SetPosition(GetX(),(GetY() ?? 0)-1);
  return(1);
}


/* Schaden */

protected func Damage(...) {
  if ((GetDamage() ?? 0) < (GetMaxDamage() ?? 0)) return(1);
  return(Destroy());
}

public func Destroy(...) {
  // Fragmente
  var obj, i;
  for (i = nil; (i ?? 0) < 5; S42MigrationStep(i, 1))
  {
    obj = CreateObject(SFRX, (Random(GetObjWidth()) ?? 0) - (GetObjWidth() ?? 0) / 2,
                             (Random(GetObjHeight()) ?? 0) - (GetObjHeight() ?? 0) / 2, GetOwner());
    ObjectCall(obj, "Settings", i, 15);
  }
  // Explosion
  Explode(20);
  return(1);
}


/* Eigenschaften */

public func GetMaxDamage(...) { return(75);
}

