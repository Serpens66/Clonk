#strict

local cloud;

public func Activate(pCaster,pRealcaster)
{
  var pClonk= pRealcaster;
  if(!pClonk) pClonk= pCaster;
  var iMat= Material("Water");
  if(GetID(Contents(0,pClonk))== ABRL) iMat= Material("Acid");
  if(GetID(Contents(0,pClonk))== LBRL) iMat= Material("Lava");
  if(GetID(Contents(0,pClonk))== OBRL) iMat= Material("Oil");
  cloud= CreateObject(FXP1,-GetX(pClonk),-GetY(pClonk),NO_OWNER());
  if(!cloud) return(0);
  cloud-> Activate(iMat,LandscapeWidth(),100);
  Sound("Magic1");
  Schedule("RemoveObject()",1000,0,cloud);
  RemoveObject();
  return(1);
}

public func GetSpellClass(object pMage) { return(WATR); }
public func GetSpellCombo(pMage) { return ("61"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
