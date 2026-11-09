/*-- Uran aus Plutonium3fertig --*/

#strict 3

protected func Initialize()
{
  ScheduleCall(this(),"Splitting",2); 
}

private func Splitting()
{
  Enter(Contained(),CreateObject(SURO,nil,nil,GetOwner()));
  RemoveObject(nil,1);
RemoveObject(PL03);
}


public func IsChemicalProduct() { return(1); }
func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(300); }