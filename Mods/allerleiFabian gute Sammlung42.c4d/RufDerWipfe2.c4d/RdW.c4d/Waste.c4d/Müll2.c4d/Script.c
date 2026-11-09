#strict
#include SHLK

public func IsWaste(){ return(50);}
private func PackTo(){ return(_CHF);}


public func Hit()
{
  if(!Random(15))
  {
    CastPXS("Acid", 20, 10, 0, 0);
    CastPXS("Acid", 20, 10, 0, 0);
//    globalpollution+=20;
var verbrannt;
verbrannt= CreateObject(VBWS);
verbrannt->Verbrannt(GetID(),IsWaste());
    ChangeDef(SHLK);
  }
  Sound("WoodHit*");
}
