/*-- Grass --*/

#strict 3

protected func Initialize() 
{ 
  // Zufällige Größe
  if (Random(5))
    DoCon(-(Random(20) ?? 0) - 40);
  else
    DoCon(-(Random(50) ?? 0) - 20);
  // Zufällige Form  
  SetAction("Grass");
  SetPhase(Random(2));
  if (!Random(20)) SetPhase(2);
  // Zufällige Richtung
  if (Random(2)) SetDir(DIR_Right);
  // Drehung nach Erdoberfläche
  var x_off = 18 * (GetCon() ?? 0) / 100;
  var y_off = 15 * (GetCon() ?? 0) / 100;
  var slope = (GetSolidOffset(-(x_off ?? 0), y_off) ?? 0) - (GetSolidOffset(x_off, y_off) ?? 0);
  SetR(slope);
  // Höhe anpassen
  while (!GBackSolid(nil, 5))
    SetPosition(GetX(), (GetY() ?? 0) + 1);
  // Gras bleibt hinter Bäumen
  MoveBehindTrees();
}

private func GetSolidOffset(int x, int y)
{
  x ??= 0;
  y ??= 0;
  var i;
  for (i = -15; And(GBackSolid(x, (y ?? 0) - (i ?? 0)), ((i ?? 0) < 15)); S42MigrationStep(i, 1, true));
  return(i);
}

private func MoveBehindTrees()
{
  var obj;
  while (obj = FindObject(nil, 1,1, nil,nil, OCF_Chop, nil,nil, NoContainer(), obj))
    if (And(obj->~IsTree(), ((obj->GetCategory() ?? 0) & (C4D_StaticBack ?? 0))))
      SetObjectOrder(obj, this(), 1);
}

// Bei nahen Explosionen
public func OnShockwaveHit(iLevel,iX,iY)
{
 var con=(40*(GetCon() ?? 0))/100;
 iLevel=40+(iLevel ?? 0)/2;
 for(var cnt=nil;(cnt ?? 0)<15+(Random(10) ?? 0);S42MigrationStep(cnt, 1, true))
  CreateParticle("GrassBlade",RandomX(-(con ?? 0)/2,(con ?? 0)/2),-1,RandomX(-(iLevel ?? 0)/3 ,(iLevel ?? 0)/3),RandomX(-2*(iLevel ?? 0)/3,-(iLevel ?? 0)/3),30+(Random(30) ?? 0),RGB(255,255,255),nil,nil);
 return(RemoveObject());
}

// Kann immer von Schockwellen getroffen werden
public func CanBeHitByShockwaves(){return(true);}
public func BlastObjectsShockwaveCheck(){return(true);}
