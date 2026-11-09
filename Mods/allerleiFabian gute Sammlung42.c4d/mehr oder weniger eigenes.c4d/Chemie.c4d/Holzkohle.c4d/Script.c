/*-- Holzkohle --*/

#strict

protected func Initialize()
{
  ScheduleCall(this(),"Splitting",2); 
}

private func Splitting()
{
  Enter(Contained(),CreateObject(COAL,0,0,GetOwner()));
  RemoveObject(0,1);
}
