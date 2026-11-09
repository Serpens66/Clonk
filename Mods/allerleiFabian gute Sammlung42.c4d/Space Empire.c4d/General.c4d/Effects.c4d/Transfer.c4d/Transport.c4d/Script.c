/*-- Transport --*/

#strict

local target;        // Ziel, Zielposition
local posX, posY;    // eigene Position (100fache Genauigkeit)
local tarX, tarY;    // Ziel-Position (100fache, wenn kein Target)


/* Aufruf durch Transfer-Script */

public func Init(par_target, parX, parY)
{
  target = par_target;
  posX = GetX()*100;
  posY = GetY()*100;
  tarX = parX*100;
  tarY = parY*100;
  // Sound
  Sound("Beam");
}


/* Timer-Call */

protected func Move()
{
  // Zielposition bestimmen
  if (target)
  {
    tarX = GetX(target)*100;
    tarY = GetY(target)*100;
  }
  // Richtung berechnen
  var dir = Angle(posX,posY, tarX,tarY);
  var speedX = +Sin(dir,500);
  var speedY = -Cos(dir,500);
  // Bewegung in mehreren Schritte
  var i;
  for (i = 0; i < 10; ++i)
  {
    // Position neu setzen
    posX += speedX;
    posY += speedY;
    SetPosition((posX+50)/100, (posY+50)/100);
    // Effekte
    CastParticles("TSpark", 5,10, 0,0, 20,20, RGB(0,0,192),RGB(64,64,255));
    // angekommen
    if ((Abs(posX-tarX) < 1000) && (Abs(posY-tarY) < 1000))
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

