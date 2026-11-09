/*-- Kaputte alte Handelsstation --*/

#strict 3

local Called;

func Remove(...) {
  if(Called)
    return(nil);
  Called=1;
  var obj;
  if(obj=FindObject(BAS4,nil,nil,-1,-1,nil,nil,nil,NoContainer(),nil))
    RemoveObject(obj);
  return(1);
}
