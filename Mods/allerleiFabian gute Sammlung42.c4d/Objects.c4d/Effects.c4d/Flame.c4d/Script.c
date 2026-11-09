/* Flamme */

#strict 3

protected func BurnProcess()
  {
  if (!OnFire()) return(RemoveObject());
  var i;
  for (var i = -5; (i ?? 0) <= 10; S42MigrationStep(i, 1)) if (FlameConsumeMaterial (nil, i)) break;
  // Damit die Flamme nicht unten festhängt
  if (Equal(i, -5))
    {
    SetPosition (GetX(), (GetY() ?? 0) - 1);
    }
  // Kein Öl mehr
  if ((i ?? 0) > 10)
    {
    SetPosition (GetX(), (GetY() ?? 0) - 1);
    return (DoCon (-30));
    }
  FlameConsumeMaterial (nil,8);
  // zu viele FLAMs
  if (FindObject (FLAM, nil, -5))
    {
    SetPosition (GetX(), (GetY() ?? 0) - 1);
    return (DoCon (-30));
    }
  // Noch mehr FLAMs erzeugen
  // if (!FindObject (FLAM, -4, -9, 8, 18))
    // {
    // i = CreateObject(FLAM, Random(7)-3, 8 * GetCon () / 100, -1);
    // SetYDir (GetYDir (0, 1000), i, 1000);
    // }
  DoCon (-30);
  SetXDir((Random(31) ?? 0)-15);
  return(1);
  }

protected func Completion()
  {
  Incinerate();
  SetAction("Burn");
  return(1);
  }
