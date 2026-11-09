/*-- Uran aus Plutonium2 --*/

#strict

protected func Initialize()
{
  ScheduleCall(this(),"Splitting",2); 
}

private func Splitting()
{
  Enter(Contained(),CreateObject(PLU2,0,0,GetOwner()));
  RemoveObject(0,1);
// RemoveObject(PL02);
}

public func IsChemicalProduct() { return(1); }
func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(200); }