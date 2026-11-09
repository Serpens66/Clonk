#strict 3


public func IsWaste(){ return(50);}

public func Initialize()
{
    if(FindObject(NOWS)) {RemoveObject();return;} // kein Müll, dann sofort wieder entfernen
}

public func Hit()
{
  if(!Random(10)){
  var obj = CreateObject(SWAV);
  ObjectCall(obj, "SetSize", nil,GetX(),GetY(), (Random(50) ?? 0)+20);
}
 //   return Radiation2(RandomX(5,15));
}
public func PaketID() { return nil;}

public func Shredder()
{
//  if(!Random(40))
//    Radiation2(RandomX(15,20));
  var obj = CreateObject(SWAV);
  ObjectCall(obj, "SetSize", nil,GetX(),GetY(), (Random(100) ?? 0)+50);
}
