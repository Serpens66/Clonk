#strict 3

protected func Activate(object pCaster, object pRealcaster)
{
  var iX = ((GetDir(pCaster) ?? 0) * 2 - 1) * (RandomX(100, 120) ?? 0);
  var iY = RandomX(100, 200);
  var iDY = nil;
  var thickness = RandomX(16, 28);
  var mat = "Lava";
  // Effekt prüfen
  var iResult;
  if (iResult = CheckEffect("VolcanoNSpell", nil, 150)) return(And(!Equal(iResult, -1), RemoveObject()));
  // in der Luft soll kein Vulkan erzeugt werden
  while (And(!GBackSolid(iX,iDY), (iDY ?? 0)+(GetY() ?? 0) <= (LandscapeHeight() ?? 0))) S42MigrationAssign(iDY, "+=", 5);
  // wenn das Landstück unter der Stelle wo der Vulkan
  // erzeugt werden soll nicht hoch genug ist (100px), wie vorher mit Y = LandscapeHeight() zaubern
  if (!GBackSolid(iX,(iY ?? 0)+(iDY ?? 0)))
    if (!GBackSolid(iX,((iY=100) ?? 0)+(iDY ?? 0)))
    {
      iY = AbsY(LandscapeHeight());
      iDY= nil;
    }
    
/*
  // ANDERE MAT-KOMBINATIONEN DEAKTIVIERT

  // Zaubernden Clonk ermitteln
  var clonk = pCaster;
  if (pRealcaster) clonk = pRealcaster;
    
  // Kombinieren?
  var obj;
  if (obj = FindContents(ABRL, clonk)) {
    mat = "Acid"; 
    thickness = RandomX(12, 17);
  } else if (obj = FindContents(OBRL, clonk)) {
    mat = "Oil";
    thickness = RandomX(8, 10);
  } else if (obj = FindContents(WBRL, clonk)) {
    mat = "Water";
  }
  RemoveObject(obj);

*/

  
  // Vulkan ausbrechen lassen
  LaunchVolcano(BoundBy((GetX() ?? 0) + (iX ?? 0),nil,LandscapeWidth()),BoundBy((GetY() ?? 0) + (iY ?? 0) + (iDY ?? 0),nil,LandscapeHeight()), thickness, mat);
  Sound("Lava*");
  RemoveObject();
  return(1);
}

public func GetSpellClass(object pMage) { return(EART); }
public func GetSpellCombo(pMage) { return ("322"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
