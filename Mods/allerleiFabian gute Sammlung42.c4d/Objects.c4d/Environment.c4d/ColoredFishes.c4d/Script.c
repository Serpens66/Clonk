/*-- Bunte Fische --*/

#strict 3

local standardfarbe; // sollte bei jedem Fisch ja dieselbe Farbe sein, oder?

protected func Initialize() 
{
  // Alle Fische anfangs umfärben
  var pFish;
  while(pFish=FindObject(FISH,nil,nil,nil,nil,nil,nil,nil,nil,pFish))
    {standardfarbe=GetColorDw(pFish);Colorize(pFish);}
}

public func Colorize(pObj) 
{
	SetColorDw(HSL(Random(256),255,100+(Random(60) ?? 0)),pObj);
}

func Destruction()
{
var pFish;
while(pFish=FindObject(FISH,nil,nil,nil,nil,nil,nil,nil,nil,pFish))
SetColorDw(standardfarbe,pFish);
}


// Kann mittels des neuen Spielzielauswählers 
public func IsChooseable2() { return(1); }