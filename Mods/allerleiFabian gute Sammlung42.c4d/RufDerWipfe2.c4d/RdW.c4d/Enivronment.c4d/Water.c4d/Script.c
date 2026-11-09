/*-- Grundwasser --*/
// Meeresspiegelregel umgedreht:
// Abflüsse nur an Land

#strict

// Kann mittels des neuen Spielzielauswählers
public func IsVariabelChooseable2() { return(1); }

protected func Initialize()
{
  // Eigene Position festlegen
  SetPosition();
  // Abflüsse platzieren
  // var x = -250;
  // while ((x += 400) < LandscapeWidth())
  var x = 0;
  while ((x += 30) < LandscapeWidth()) // alle 30 clonkmeter einen abfluss platzieren
    CreateDrain(x, GetHorizonHeight(x));
  return(1);    
}
       
private func GetHorizonHeight(x)
{
  var y;
  while (y < LandscapeHeight() && !GBackSolid(x, y))
    y += 10;
  while (y > LandscapeHeight() && GBackSolid(x, y))
    --y;
  return(y);
}
    
private func CreateDrain(x, y)
{
  if (!GBackLiquid(x, y))
  {
    CreateObject(DRA2, x, y-1, -1);
    //CreateObject(DRA2, x, y+100, -1);
    return 1;
  }
  return(0);
}

func Destruction()
{
var pAbfluss2;
while(pAbfluss2=FindObject(DRA2,0,0,0,0,0,0,0,0,pAbfluss2))
RemoveObject(pAbfluss2);
}
