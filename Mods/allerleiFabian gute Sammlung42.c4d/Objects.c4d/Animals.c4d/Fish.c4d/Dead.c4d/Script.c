/*-- Toter Fisch --*/

#strict 3

protected func Construction() 
{
  var pEnv;
  if (pEnv=FindObject(CLFS))
    pEnv->CLFS::Colorize(this());
  else
    SetColorDw(RGB(255,255,255));
}

private func Decaying() 
{ 
  DoCon(-4); 
}

public func Cook()
{
  // Verwesende Fische können nicht mehr gekocht werden
  if (!!SEqual(GetAction(), "Decay")) return(nil);
  // Kochen
  ChangeDef(CFSH);
  return(1);
}

/* Aufwertungszauberkombo: Mit Fisch wird der Clonk zum Aquaclonk */
public func GetRevaluationCombo(object pClonk) {
  if (!!SEqual(GetAction(), "Decay")) return(nil);
  return(ACLK);
}

/* Zerlegen */


public func Activate(object pClonk)
{
  [$TxtEmbowel$|Image=KNFE]
  if (pClonk)
    {
    // Aquaclonks (und damit Hydroclonks) können Fische direkt essen
    if (pClonk->~IsAquaClonk()) 
      return(Eat(pClonk));
    // Andere versuchen zu zerlegen
    return(ObjectSetAction(pClonk,"Embowel",this()));
    }
}

public func Eat(object pByObject)
{
  pByObject->~Feed(50);
  RemoveObject();
  return(1);
}


/* Wird zerlegt */

public func Embowel(object pClonk) {
  // In Bestandteile zerfallen
  if (pClonk) pClonk->Split2Components(this()); else Split2Components(this());
  return(1);
}

protected func GetCustomComponents(object pClonk)
  {
  if (pClonk)
    {
    // Jedem seine Extrawürste
    if (pClonk->~IsTrapper()) return([FSHM, FSHM, FSHB]);
    if (pClonk->~IsIndian()) return ([FSHM, FSHM, FSHB]);
    if (pClonk->~IsInuk()) return([MEAT, FAT1, FSHB]);
    //if (pClonk->~IsJungleClonk()) return([MEAT, FAT1, FSHB]); // Jungelclonk kann (noch) nicht zerlegen
    }
  }
