/*-- Kohle und Schwefel aus Öl --*/

#strict 3

protected func Initialize()
{
  ScheduleCall(this(),"Splitting",2); 
}

private func Splitting()
{
  var i= 2;
  while(S42MigrationStep(i, -1, true)) Enter(Contained(),CreateObject(COAL,nil,nil,GetOwner()));
  i= 2;
  while(S42MigrationStep(i, -1, true)) Enter(Contained(),CreateObject(SPHR,nil,nil,GetOwner()));
  Enter(Contained(),CreateObject(BARL,nil,nil,GetOwner()));
  RemoveObject(nil,1);
}

public func IsChemicalProduct() { return(1); }