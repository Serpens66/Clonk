#strict 3
#include SHLK

public func IsWaste(){ return(50);}
private func PackTo(){ return(_CHF);}


public func Hit()
{
  if(!Random(15))
  {
    CastPXS("Acid", 20, 10, nil, nil);
    CastPXS("Acid", 20, 10, nil, nil);
//    globalpollution+=20;
var verbrannt;
verbrannt= CreateObject(VBWS);
verbrannt->Verbrannt(GetID(),IsWaste());
    ChangeDef(SHLK);
  }
  Sound("WoodHit*");
}
