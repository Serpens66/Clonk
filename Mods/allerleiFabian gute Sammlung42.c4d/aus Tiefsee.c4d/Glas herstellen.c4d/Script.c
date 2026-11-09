/*-- Glas herstellen --*/

#strict 3

protected func Initialize()
{
  ScheduleCall(this(),"Splitting",2); 
}

private func Splitting()
{
  Enter(Contained(),CreateObject(GLAS,nil,nil,GetOwner()));
  Enter(Contained(),CreateObject(GLAS,nil,nil,GetOwner()));
  RemoveObject(nil,1);
}

public func IsChemicalProduct() { return(1); }