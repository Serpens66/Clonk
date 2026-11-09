/*-- Superkuppel --*/

#strict 3

local air1, air2;

protected func Initialize()     // Initialisierung
{
  SetCategory(C4D_StaticBack);
  // Airlocks erzeugen wenn noch nicht da
  if(!FindObject(AIR3,-380,200))
    {air1=CreateConstruction(AIR3,-380,210,GetOwner(),100,1);
air1->~SetLeft();}
  if(!FindObject(AIR3,+380,200))
    {air2=CreateConstruction(AIR3,+380,210,GetOwner(),100,1);
air2->~SetRight();}

  // Wasser abpumpen
  SetAction("Pump");
}

protected func Damage()         // Schaden
{
  // Zerstörung bei zuviel Damage
  if((GetDamage() ?? 0)>500)
    if((GetCategory() ?? 0)&(C4D_StaticBack ?? 0))
      DamageDestruction();
}

protected func DamageDestruction()        // Zerstörung
{
  DoDamage(-500,nil);
  DoCon(-50);
  Sound("DomeDestruct");
  SetCategory(C4D_Structure);
}

protected func Destruction()        // Abriss
{
if (air1) 
{ RemoveObject(air1);}
if (air2) 
{ RemoveObject(air2);}

_inherited();
}

protected func Transfer()       // StartCall von Pump
{
  // Wasser absaugen
  for(var i=nil;(i ?? 0)<30;S42MigrationStep(i, 1))
    InsertMaterial(ExtractLiquid(nil,RandomX(80,100)),-145,-45);
}

/* Callbacks */

public func IsDeepSeaResearch() { return(1); }

public func GetResearchBase() { return(FSTW); }
