/*-- Meeresspiegel --*/

#strict


// Kann mittels des neuen Spielzielauswählers
public func IsChooseable2() { return(1); }

protected func Initialize()
{
  // Eigene Position festlegen
  SetPosition();
  // Abflüsse platzieren
  var x = -150;
  while ((x += 200) < LandscapeWidth())
    CreateDrain(x, GetHorizonHeight(x));
  return(1);    
}
       
private func GetHorizonHeight(x)
{
  var y;
  while (y < LandscapeHeight() && !GBackSemiSolid(x, y))
    y += 10;
  while (y > LandscapeHeight() || GBackSemiSolid(x, y))
    --y;
  return(y);
}
    
private func CreateDrain(x, y)
{
  if (GBackLiquid(x, y + 1))
  {
      // Log(Format("Drai platziert: %v/%v",x,y));
      return(CreateObject(DRAI, x, y, -1));
  }
    
  return(0);
}

func Destruction()
{
var pAbfluss;
while(pAbfluss=FindObject(DRAI,0,0,0,0,0,0,0,0,pAbfluss))
RemoveObject(pAbfluss);
}
