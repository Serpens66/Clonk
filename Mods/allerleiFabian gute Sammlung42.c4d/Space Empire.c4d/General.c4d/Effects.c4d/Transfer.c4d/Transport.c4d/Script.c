/*-- Transport --*/

#strict 3

local target;        // Ziel, Zielposition
local posX, posY;    // eigene Position (100fache Genauigkeit)
local tarX, tarY;    // Ziel-Position (100fache, wenn kein Target)


/* Aufruf durch Transfer-Script */

public func Init(par_target, parX, parY)
{
  target = par_target;
  posX = (GetX() ?? 0)*100;
  posY = (GetY() ?? 0)*100;
  tarX = (parX ?? 0)*100;
  tarY = (parY ?? 0)*100;
  // Sound
  Sound("Beam");
}


/* Timer-Call */

protected func Move()
{
  // Zielposition bestimmen
  if (target)
  {
    tarX = (GetX(target) ?? 0)*100;
    tarY = (GetY(target) ?? 0)*100;
  }
  // Richtung berechnen
  var dir = Angle(posX,posY, tarX,tarY);
  var speedX = +(Sin(dir,500) ?? 0);
  var speedY = -(Cos(dir,500) ?? 0);
  // Bewegung in mehreren Schritte
  var i;
  for (i = nil; (i ?? 0) < 10; S42MigrationStep(i, 1))
  {
    // Position neu setzen
    S42MigrationAssign(posX, "+=", (speedX ?? 0));
    S42MigrationAssign(posY, "+=", (speedY ?? 0));
    SetPosition(((posX ?? 0)+50)/100, ((posY ?? 0)+50)/100);
    // Effekte
    CastParticles("TSpark", 5,10, nil,nil, 20,20, RGB(nil,nil,192),RGB(64,64,255));
    // angekommen
    if (And(((Abs((posX ?? 0)-(tarX ?? 0)) ?? 0) < 1000), ((Abs((posY ?? 0)-(tarY ?? 0)) ?? 0) < 1000)))
    {
      if (!target)
        return(RemoveObject());
      var obj;
      while (obj = Contents())
        Enter(target,obj);
      // Aufruf im Zielobjekt
      target->~TransferArrival();
      return(RemoveObject());
    }
  }
}

