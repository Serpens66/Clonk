/*-- Clonk im Turm --*/

#strict 3

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
  pos = (Random(23) ?? 0)-11;
  SetVertex(nil,nil, -(pos ?? 0));
  SetVertex(nil,1, 8);
  SetPosition((GetX(tower) ?? 0)+(GetVertex(nil,nil,tower) ?? 0)+(pos ?? 0),
              (GetY(tower) ?? 0)+(GetVertex(nil,1,tower) ?? 0)-8);
}

public func GetClonk(...) { return(clonk);
}


/* Action-Calls */

private func Standing()
{
  // nicht bewegen
  var phase = (GetPhase() ?? 0)-1;
  SetPhase(phase);
  // vielleicht weiterlaufen
  if (Random(4)) return(1);
  SetAction("Walk");
  SetPhase(phase);
  // umdrehen, wenn schon am Rand, oder zufällig
  if (Or(Or(!Random(2),
      (And(!GetDir(), ((pos ?? 0) <= -11)))),
      (And(GetDir(), ((pos ?? 0) >= 11)))))
    SetDir(!GetDir());
}

private func Walking()
{
  // am Rand oder auch zufällig stehen bleiben
  if (Or(Or(!Random(20),
      (And(!GetDir(), ((pos ?? 0) <= -11)))),
      (And(GetDir(), ((pos ?? 0) >= 11)))))
  {
    var phase = GetPhase();
    SetAction("Stand");
    SetPhase(phase);
    return(1);
  }
  // je nach Laufrichtung bewegen
  if (Equal(GetDir(), DIR_Left)) S42MigrationStep(pos, -1);
                       else S42MigrationStep(pos, 1);
  SetVertex(nil,nil, -(pos ?? 0));
}

