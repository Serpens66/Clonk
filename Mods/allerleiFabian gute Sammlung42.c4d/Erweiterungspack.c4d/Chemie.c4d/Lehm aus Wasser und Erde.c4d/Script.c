/*-- Lehm aus Erde und Wasser --*/

#strict 3

protected func Initialize()
{
  ScheduleCall(this(),"Splitting",2); 
}

private func Splitting()
{
  Enter(Contained(),CreateObject(LOAM,nil,nil,GetOwner()));
  Enter(Contained(),CreateObject(LOAM,nil,nil,GetOwner()));
  Enter(Contained(),CreateObject(BARL,nil,nil,GetOwner()));
  Enter(Contained(),CreateObject(LOAM,nil,nil,GetOwner()));
  Enter(Contained(),CreateObject(LOAM,nil,nil,GetOwner()));
  Enter(Contained(),CreateObject(LOAM,nil,nil,GetOwner()));
  Enter(Contained(),CreateObject(LOAM,nil,nil,GetOwner()));
  Enter(Contained(),CreateObject(LOAM,nil,nil,GetOwner()));
  Enter(Contained(),CreateObject(LOAM,nil,nil,GetOwner()));
  Enter(Contained(),CreateObject(LOAM,nil,nil,GetOwner()));
  RemoveObject(nil,1);
}

public func IsChemicalProduct() { return(1); }