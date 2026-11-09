/*-- Lehm3 --*/

#strict

protected func Initialize()
{
  ScheduleCall(this(),"Splitting",2); 
}

private func Splitting()
{
  Enter(Contained(),CreateObject(LOA2,0,0,GetOwner()));
  RemoveObject(0,1);
}

public func IsChemicalProduct() { return(1); }