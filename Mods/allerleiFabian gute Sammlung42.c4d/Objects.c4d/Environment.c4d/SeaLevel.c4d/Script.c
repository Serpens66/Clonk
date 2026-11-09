/*-- Meeresspiegel --*/

#strict 3


// Kann mittels des neuen Spielzielauswählers
public func IsChooseable2() { return(1); }

protected func Initialize()
{
  // Eigene Position festlegen
  SetPosition();
  // Abflüsse platzieren
  var x = -150;
  while (((S42MigrationAssign(x, "+=", 200)) ?? 0) < (LandscapeWidth() ?? 0))
    CreateDrain(x, GetHorizonHeight(x));
  return(1);    
}
       
private func GetHorizonHeight(x)
{
  var y;
  while (And((y ?? 0) < (LandscapeHeight() ?? 0), !GBackSemiSolid(x, y)))
    S42MigrationAssign(y, "+=", 10);
  while (Or((y ?? 0) > (LandscapeHeight() ?? 0), GBackSemiSolid(x, y)))
    S42MigrationStep(y, -1);
  return(y);
}
    
private func CreateDrain(x, y)
{
  if (GBackLiquid(x, (y ?? 0) + 1))
  {
      // Log(Format("Drai platziert: %v/%v",x,y));
      return(CreateObject(DRAI, x, y, -1));
  }
    
  return(nil);
}

func Destruction()
{
var pAbfluss;
while(pAbfluss=FindObject(DRAI,nil,nil,nil,nil,nil,nil,nil,nil,pAbfluss))
RemoveObject(pAbfluss);
}
