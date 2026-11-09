/*-- Flagge --*/

#strict
local pseudoflag;

func Initialize()
{
    SetPlrViewRange(FogViewRange(),0,1);
    return(_inherited());
}

func FogViewRange() // wird vom Teamview objekt aufgerufen, um anderen nicht-feindlichen Spielern ebenfalls diesen sichtradius mithilfe eines hilfsobjekts zu geben
{
    return(130); 
}

private func Flying()
{
  SetDir(BoundBy(GetWind()/10+4,0,8));
  if (!GetActTime())
  {
    SetCategory(31 & GetCategory(GetActionTarget()));
    SetObjectOrder (GetActionTarget());
  }
  return(1);
}

func Werdepseudoflagge() // für das uplinksystem
{
pseudoflag = 1;
}

func IstPseudoflagge() {return(pseudoflag);}

func ResetCategory()
{
  SetCategory(2576);
  return 1;
}

func OnOwnerChanged(int new_owner, int old_owner)
{
  // Flagge hat den Besitzer verloren? Dann runterfallen!
  if (new_owner == NO_OWNER) SetAction("Idle");
}

/* Produkteigenschaften */

public func IsAnvilProduct() { return(1); }