/*-- Clonk im Turm --*/

#strict

local clonk;     // der Clonk im Turm, der diesen Schatten erzeugt
local pos;       // Position des Schattens


/* Aufruf vom Wachturm */

public func Init(tower, par_clonk)
{
  clonk = par_clonk;
  // am Anfang steht der Clonk erstmal
  SetAction("Stand", tower);
  SetPhase(Random(8));
  SetDir(Random(2));
  // zufällige Startposition
  pos = Random(23)-11;
  SetVertex(0,0, -pos);
  SetVertex(0,1, 8);
  SetPosition(GetX(tower)+GetVertex(0,0,tower)+pos,
              GetY(tower)+GetVertex(0,1,tower)-8);
}

public GetClonk: return(clonk);


/* Action-Calls */

private func Standing()
{
  // nicht bewegen
  var phase = GetPhase()-1;
  SetPhase(phase);
  // vielleicht weiterlaufen
  if (Random(4)) return(1);
  SetAction("Walk");
  SetPhase(phase);
  // umdrehen, wenn schon am Rand, oder zufällig
  if (!Random(2) ||
      (!GetDir() && (pos <= -11)) ||
      (GetDir() && (pos >= 11)))
    SetDir(!GetDir());
}

private func Walking()
{
  // am Rand oder auch zufällig stehen bleiben
  if (!Random(20) ||
      (!GetDir() && (pos <= -11)) ||
      (GetDir() && (pos >= 11)))
  {
    var phase = GetPhase();
    SetAction("Stand");
    SetPhase(phase);
    return(1);
  }
  // je nach Laufrichtung bewegen
  if (GetDir()==DIR_Left()) --pos;
                       else ++pos;
  SetVertex(0,0, -pos);
}

