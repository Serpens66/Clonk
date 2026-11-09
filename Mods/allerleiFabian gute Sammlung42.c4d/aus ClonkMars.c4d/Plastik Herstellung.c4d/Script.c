/*-- Öl herstellen --*/

#strict

protected func Initialize()
{
  ScheduleCall(this(),"Splitting",2); 
}

private func Splitting()
{
  Enter(Contained(),CreateObject(PSTC,0,0,GetOwner()));
  Enter(Contained(),CreateObject(PSTC,0,0,GetOwner()));
  Enter(Contained(),CreateObject(PSTC,0,0,GetOwner()));
  Enter(Contained(),CreateObject(PSTC,0,0,GetOwner()));
  Enter(Contained(),CreateObject(PSTC,0,0,GetOwner()));
  Enter(Contained(),CreateObject(BARL,0,0,GetOwner()));
  RemoveObject(0,1);
}

