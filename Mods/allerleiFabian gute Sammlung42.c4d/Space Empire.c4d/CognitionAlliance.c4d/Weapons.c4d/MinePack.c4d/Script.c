/*-- Minenpack --*/

#strict 3

local minecount;


/* Initialisierung */

protected func Initialize(...) {
  SetAction("Five");
  minecount = 5;
  return(1);
}

/* In Minen umwandeln */

protected func Activate(...) {
  var mine;
  while (minecount)
  {
    mine = CreateContents(CWMN,Par(nil));
    SetOwner(GetOwner(Par(nil)), mine);
    S42MigrationStep(minecount, -1);
  }
  Sound("Connect");
  return(RemoveObject());
}

/* Minen sollen gepackt werden */

public func PackMines(...) {
  // Pack anfangs leer
  minecount = nil;
  // bis zu fünf Minen hier rein
  var mine;
  while (And((mine = FindContents(CWMN,Contained())), ((minecount ?? 0) < 5)))
  {
    S42MigrationStep(minecount, 1);
    RemoveObject(mine);
  }
  if (!minecount)
    return(RemoveObject());
  // richtige Action & Titelbild setzen
  if (Equal(minecount, 1))  { SetAction("One");   SetPicture(110, 120, 35,  40); }
  if (Equal(minecount, 2))  { SetAction("Two");   SetPicture(110, 80,  35,  40);  }
  if (Equal(minecount, 3))  { SetAction("Three"); SetPicture(110, 40,  35,  40);  }
  if (Equal(minecount, 4))  { SetAction("Four");  SetPicture(110, nil,   35,  40);   }
  if (Equal(minecount, 5))  { SetAction("Five");  SetPicture( 10, nil,  100, 115);  }
  // Sound
  Sound("Connect");
  return(1);
}

/* Wert bei Verkauf */

protected func CalcValue(...) {
  return((minecount ?? 0)*6);
}

/* Eigenschaften */

public func IsRefineryProduct() { return(1); }

