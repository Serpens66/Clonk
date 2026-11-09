/*-- Öl herstellen --*/

#strict 3

protected func Initialize()
{
  ScheduleCall(this(),"Splitting",2); 
}

private func Splitting()
{
  Enter(Contained(),CreateObject(PSTC,nil,nil,GetOwner()));
  Enter(Contained(),CreateObject(PSTC,nil,nil,GetOwner()));
  Enter(Contained(),CreateObject(PSTC,nil,nil,GetOwner()));
  Enter(Contained(),CreateObject(PSTC,nil,nil,GetOwner()));
  Enter(Contained(),CreateObject(PSTC,nil,nil,GetOwner()));
  Enter(Contained(),CreateObject(BARL,nil,nil,GetOwner()));
  RemoveObject(nil,1);
}

