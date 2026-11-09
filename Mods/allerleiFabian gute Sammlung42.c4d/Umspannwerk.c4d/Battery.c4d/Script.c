/*-- Batterie --*/

#strict 3

/* Aufschlag */

protected func Hit()
{
  Sound("MetalHit*");
}


/* Produkteigenschaften */


public func IsChemicalProduct() { return(1); }

/* Forschung */

// public func GetResearchBase() { return(ACT3); } // ist jetzt unabhängig vom Geschützturm


public func CalcValue(object pInBase, int iForPlayer)
{
  // Je nach Anzahl
  iForPlayer ??= 0;
  return((GetValue(nil, GetID()) ?? 0)+(3*(GetEnergy() ?? 0))/100); // normalen Value wenn leer, plus 3, wenn voll
}

func PrintEnergy()
{
    if(FindObject(ENNN))
        if(!Contained()) // nur nachricht ausgeben, wenn nicht contained, da sonst unübersichtlich
            Message(Format("%v",GetEnergy()),this());
}

func Activate(pClonk)
{
    Message(Format("Batterie enthaelt %v Energie",GetEnergy()),pClonk);
}