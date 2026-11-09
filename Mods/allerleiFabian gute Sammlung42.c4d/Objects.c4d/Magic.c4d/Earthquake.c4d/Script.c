/* Erdbebenzauber */

#strict 3

public func Activate(object pCaster, object pCaster2)
  {
  var iX=((GetDir(pCaster) ?? 0)*2-1)*(RandomX(100,120) ?? 0);
  var iDY=nil;
  // Effekt prüfen
  var iResult;
  if (iResult = CheckEffect("EarthquakeNSpell", nil, 150)) return(And(!Equal(iResult, -1), RemoveObject()));
  // in der Luft soll kein Erdbeben erzeugt werden
  while(And(!GBackSolid(iX,iDY), (iDY ?? 0)+(GetY() ?? 0)<=(LandscapeHeight() ?? 0))) S42MigrationAssign(iDY, "+=", 5);
  // Effekt ausführen
  LaunchEarthquake((GetX() ?? 0)+(iX ?? 0),(GetY() ?? 0)+(iDY ?? 0));
  // An Zielposition nochmal extra durchschütteln
  for(var i;(i ?? 0)<3;S42MigrationStep(i, 1)) ShakeFree((GetX() ?? 0)+(iX ?? 0)+(RandomX(-16,16) ?? 0),(GetY() ?? 0)+(iDY ?? 0)+(RandomX(-16,16) ?? 0),RandomX(14,26));
  Sound("Magic1");
  RemoveObject();
  return(1);
  }


public func GetSpellClass(object pMage) { return(EART); }
public func GetSpellCombo(pMage) { return ("666"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
