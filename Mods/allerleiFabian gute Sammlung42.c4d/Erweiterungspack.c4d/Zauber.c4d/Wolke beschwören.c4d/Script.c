#strict 3

local cloud;

public func Activate(pCaster,pRealcaster)
{
  var pClonk= pRealcaster;
  if(!pClonk) pClonk= pCaster;
  var iMat= Material("Water");
  if(Equal(GetID(Contents(nil,pClonk)), ABRL)) iMat= Material("Acid");
  if(Equal(GetID(Contents(nil,pClonk)), LBRL)) iMat= Material("Lava");
  if(Equal(GetID(Contents(nil,pClonk)), OBRL)) iMat= Material("Oil");
  cloud= CreateObject(FXP1,-(GetX(pClonk) ?? 0),-(GetY(pClonk) ?? 0),NO_OWNER);
  if(!cloud) return(nil);
  cloud-> Activate(iMat,LandscapeWidth(),100);
  Sound("Magic1");
  Schedule("RemoveObject()",1000,nil,cloud);
  RemoveObject();
  return(1);
}

public func GetSpellClass(object pMage) { return(WATR); }
public func GetSpellCombo(pMage) { return ("61"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
