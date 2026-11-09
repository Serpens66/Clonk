/*-- Minenpack --*/

#strict

local minecount;


/* Initialisierung */

protected Initialize:
  SetAction("Five");
  minecount = 5;
  return(1);

/* In Minen umwandeln */

protected Activate:
  var mine;
  while (minecount)
  {
    mine = CreateContents(CWMN,Par(0));
    SetOwner(GetOwner(Par(0)), mine);
    --minecount;
  }
  Sound("Connect");
  return(RemoveObject());

/* Minen sollen gepackt werden */

public PackMines:
  // Pack anfangs leer
  minecount = 0;
  // bis zu fünf Minen hier rein
  var mine;
  while ((mine = FindContents(CWMN,Contained())) && (minecount < 5))
  {
    ++minecount;
    RemoveObject(mine);
  }
  if (!minecount)
    return(RemoveObject());
  // richtige Action & Titelbild setzen
  if (minecount==1)  { SetAction("One");   SetPicture(110, 120, 35,  40); }
  if (minecount==2)  { SetAction("Two");   SetPicture(110, 80,  35,  40);  }
  if (minecount==3)  { SetAction("Three"); SetPicture(110, 40,  35,  40);  }
  if (minecount==4)  { SetAction("Four");  SetPicture(110, 0,   35,  40);   }
  if (minecount==5)  { SetAction("Five");  SetPicture( 10, 0,  100, 115);  }
  // Sound
  Sound("Connect");
  return(1);

/* Wert bei Verkauf */

protected CalcValue:
  return(minecount*6);

/* Eigenschaften */

public func IsRefineryProduct() { return(1); }

