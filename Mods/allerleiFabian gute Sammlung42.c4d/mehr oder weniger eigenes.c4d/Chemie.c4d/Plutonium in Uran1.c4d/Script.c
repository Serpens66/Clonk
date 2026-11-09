/*-- Uran aus Plutonium1 --*/

#strict 3

protected func Initialize()
{
  ScheduleCall(this(),"Splitting",2); 
}

private func Splitting()
{
  Enter(Contained(),CreateObject(PLU1,nil,nil,GetOwner()));
  RemoveObject(nil,1);
// RemoveObject(PL01);
}

public func IsChemicalProduct() { return(1); }


func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(100); }