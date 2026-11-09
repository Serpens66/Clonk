/*-- StartGold --*/

#strict 3

local iGold;

/* Initialisierung */

protected func Initialize()
{
  iGold=50;
}

public func BannerPlaced(iPlr)
{
  DoWealth(iPlr,iGold);
}

public func CreateOnChoose()
{
  return true;
}

public func IsChooserConfig(){return true;}

public func ConfigSettings(iIndex,iSymb)
{
  if(!S42MigrationStep(iIndex, -1, true)){ if(!iSymb) return Format("Startgold:%d",100+(iGold ?? 0)); if(Equal(iSymb, 2)) return 5; return XSYX;}
  if(!S42MigrationStep(iIndex, -1, true)){ if(!iSymb) return "+100"; if(Equal(iSymb, 2)) return 3; return XSYX;}
  if(!S42MigrationStep(iIndex, -1, true)){ if(!iSymb) return "+10 "; if(Equal(iSymb, 2)) return 1; return XSYX;}
  if(!S42MigrationStep(iIndex, -1, true)){ if(!iSymb) return "-10 "; if(Equal(iSymb, 2)) return 2; return XSYX;}
  if(!S42MigrationStep(iIndex, -1, true)){ if(!iSymb) return "-100"; if(Equal(iSymb, 2)) return 4; return XSYX;}
}

public func ConfigSettingsCall(iIndex)
{
  if(!S42MigrationStep(iIndex, -1, true))
   return nil;
  if(!S42MigrationStep(iIndex, -1, true))
   return iGold=BoundBy((iGold ?? 0)+100,-80,900);
  if(!S42MigrationStep(iIndex, -1, true))
   return iGold=BoundBy((iGold ?? 0)+10 ,-80,900);
  if(!S42MigrationStep(iIndex, -1, true))
   return iGold=BoundBy((iGold ?? 0)-10 ,-80,900);
  if(!S42MigrationStep(iIndex, -1, true))
   return iGold=BoundBy((iGold ?? 0)-100,-80,900);
}
