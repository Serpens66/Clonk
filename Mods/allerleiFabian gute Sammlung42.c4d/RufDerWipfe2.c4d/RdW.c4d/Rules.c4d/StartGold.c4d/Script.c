/*-- StartGold --*/

#strict

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
  if(!iIndex--){ if(!iSymb) return Format("Startgold:%d",100+iGold); if(iSymb==2) return 5; return XSYX;}
  if(!iIndex--){ if(!iSymb) return "+100"; if(iSymb==2) return 3; return XSYX;}
  if(!iIndex--){ if(!iSymb) return "+10 "; if(iSymb==2) return 1; return XSYX;}
  if(!iIndex--){ if(!iSymb) return "-10 "; if(iSymb==2) return 2; return XSYX;}
  if(!iIndex--){ if(!iSymb) return "-100"; if(iSymb==2) return 4; return XSYX;}
}

public func ConfigSettingsCall(iIndex)
{
  if(!iIndex--)
   return 0;
  if(!iIndex--)
   return iGold=BoundBy(iGold+100,-80,900);
  if(!iIndex--)
   return iGold=BoundBy(iGold+10 ,-80,900);
  if(!iIndex--)
   return iGold=BoundBy(iGold-10 ,-80,900);
  if(!iIndex--)
   return iGold=BoundBy(iGold-100,-80,900);
}
