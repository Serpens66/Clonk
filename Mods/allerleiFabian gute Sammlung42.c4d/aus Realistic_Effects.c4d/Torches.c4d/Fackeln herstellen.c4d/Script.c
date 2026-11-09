/*-- Fackeln herstellen --*/

#strict 3

protected func Initialize()
{
  ScheduleCall(this(),"Splitting",2); 
}

private func Splitting()
{
  Enter(Contained(),CreateObject(FK9Z,nil,nil,GetOwner()));
  Enter(Contained(),CreateObject(FK9Z,nil,nil,GetOwner()));
  Enter(Contained(),CreateObject(FK9Z,nil,nil,GetOwner()));
  Enter(Contained(),CreateObject(FK9Z,nil,nil,GetOwner()));
  RemoveObject(nil,1);
}

public func IsAnvilProduct(...) { return(1);
}