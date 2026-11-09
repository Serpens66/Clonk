/*-- Kohle und Schwefel aus Öl --*/

#strict

protected func Initialize()
{
  ScheduleCall(this(),"Splitting",2); 
}

private func Splitting()
{
  var i= 2;
  while(i--) Enter(Contained(),CreateObject(COAL,0,0,GetOwner()));
  i= 2;
  while(i--) Enter(Contained(),CreateObject(SPHR,0,0,GetOwner()));
  Enter(Contained(),CreateObject(BARL,0,0,GetOwner()));
  RemoveObject(0,1);
}

public func IsChemicalProduct() { return(1); }