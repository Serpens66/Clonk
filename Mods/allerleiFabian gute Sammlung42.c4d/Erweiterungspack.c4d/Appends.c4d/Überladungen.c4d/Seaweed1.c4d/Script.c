/*-- Seegras --*/

#strict 3

local Collectible;

protected func CheckEnvironment()
{
  if(!Random(ReproductionRate())) Reproduction();
  if (GBackLiquid())
    if (!!SEqual(GetAction(), "Dry"))
      return(StartWiggle());
  if (!GBackLiquid())
    if (!!SEqual(GetAction(), "Wiggle"))
      return(StartDry());
  return(1);
}

private func StartWiggle()
{
  SetAction("Wiggle");
  SetPhase(Random(8));
}
  
private func StartDry()
{
  SetAction("Dry");
  Growing();
}
 
protected func Construction()
{
  StartDry();
  Growing();
  return(1);
}

public func Reproduction()
{
  var iSize= ReproductionAreaSize();
  var iOffset= (iSize ?? 0) /-2;
  if((ObjectCount(GetID(),iOffset,iOffset,iSize,iSize) ?? 0)< (MaxTreeCount() ?? 0))
    {
    var obj= PlaceVegetation(GetID(),iOffset,iOffset,iSize,iSize,10);
    return(1);
    }
  return(nil);
}

private func Growing()
{
  DoCon(+1);
  if((GetCon() ?? 0)< 100) ScheduleCall(this(),"Growing",100);
}

protected func RejectEntrance()
{
  return(!Collectible);
}

private func ReproductionAreaSize() { return(350); }  // Größe des Bereichs, in dem neue Bäume dieser Art entstehen können
private func ReproductionRate()     { return(3000); } // Die Chance, dass in einem Timerintervall eine Vermehrung stattfindet
private func MaxTreeCount()         { return(20); }   // Maximale Baumanzahl im Umkreis

public func OrganicMaterial()       { return(1); }    // Status-Abfrage für den Kohleturm