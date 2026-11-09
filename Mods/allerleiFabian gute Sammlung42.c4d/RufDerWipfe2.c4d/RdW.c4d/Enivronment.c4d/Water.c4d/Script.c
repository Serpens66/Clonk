/*-- Grundwasser --*/
// Meeresspiegelregel umgedreht:
// Abflüsse nur an Land

#strict 3

// Kann mittels des neuen Spielzielauswählers
public func IsVariabelChooseable2() { return(1); }

protected func Initialize()
{
  // Eigene Position festlegen
  SetPosition();
  // Abflüsse platzieren
  // var x = -250;
  // while ((x += 400) < LandscapeWidth())
  var x = nil;
  while (((S42MigrationAssign(x, "+=", 30)) ?? 0) < (LandscapeWidth() ?? 0)) // alle 30 clonkmeter einen abfluss platzieren
    CreateDrain(x, GetHorizonHeight(x));
  return(1);    
}
       
private func GetHorizonHeight(x)
{
  var y;
  while (And((y ?? 0) < (LandscapeHeight() ?? 0), !GBackSolid(x, y)))
    S42MigrationAssign(y, "+=", 10);
  while (And((y ?? 0) > (LandscapeHeight() ?? 0), GBackSolid(x, y)))
    S42MigrationStep(y, -1);
  return(y);
}
    
private func CreateDrain(x, y)
{
  if (!GBackLiquid(x, y))
  {
    CreateObject(DRA2, x, (y ?? 0)-1, -1);
    //CreateObject(DRA2, x, y+100, -1);
    return 1;
  }
  return(nil);
}

func Destruction()
{
var pAbfluss2;
while(pAbfluss2=FindObject(DRA2,nil,nil,nil,nil,nil,nil,nil,nil,pAbfluss2))
RemoveObject(pAbfluss2);
}
