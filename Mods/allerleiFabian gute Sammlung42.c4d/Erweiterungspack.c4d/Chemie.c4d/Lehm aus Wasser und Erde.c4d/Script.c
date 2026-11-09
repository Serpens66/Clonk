/*-- Lehm aus Erde und Wasser --*/

#strict

protected func Initialize()
{
  ScheduleCall(this(),"Splitting",2); 
}

private func Splitting()
{
  Enter(Contained(),CreateObject(LOAM,0,0,GetOwner()));
  Enter(Contained(),CreateObject(LOAM,0,0,GetOwner()));
  Enter(Contained(),CreateObject(BARL,0,0,GetOwner()));
  Enter(Contained(),CreateObject(LOAM,0,0,GetOwner()));
  Enter(Contained(),CreateObject(LOAM,0,0,GetOwner()));
  Enter(Contained(),CreateObject(LOAM,0,0,GetOwner()));
  Enter(Contained(),CreateObject(LOAM,0,0,GetOwner()));
  Enter(Contained(),CreateObject(LOAM,0,0,GetOwner()));
  Enter(Contained(),CreateObject(LOAM,0,0,GetOwner()));
  Enter(Contained(),CreateObject(LOAM,0,0,GetOwner()));
  RemoveObject(0,1);
}

public func IsChemicalProduct() { return(1); }