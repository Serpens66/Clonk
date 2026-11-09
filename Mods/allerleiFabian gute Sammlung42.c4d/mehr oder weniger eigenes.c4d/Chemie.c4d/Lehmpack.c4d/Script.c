/*-- Lehm3 --*/

#strict 3

protected func Initialize()
{
  ScheduleCall(this(),"Splitting",2); 
}

private func Splitting()
{
  Enter(Contained(),CreateObject(LOA2,nil,nil,GetOwner()));
  RemoveObject(nil,1);
}

public func IsChemicalProduct() { return(1); }