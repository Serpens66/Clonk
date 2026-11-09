/*-- Symbol --*/

#strict

local pTracingObject;

func Initialize() {
  SetVisibility(VIS_Owner(),this());
  SetAction("Floating"); 
  return(1);
}

public func TracingObject(pObject) {
  AddEffect("Tracing",this(),1,1,this(),0,pObject);
  pTracingObject=pObject;
}



public func FxTracingTimer() {
  SetPosition(GetX(pTracingObject),GetY(pTracingObject)-40,this());
}
