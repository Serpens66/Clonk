/*-- Uran aus Plutonium3fertig --*/

#strict

protected func Initialize()
{
  ScheduleCall(this(),"Splitting",2); 
}

private func Splitting()
{
  Enter(Contained(),CreateObject(SURO,0,0,GetOwner()));
  RemoveObject(0,1);
RemoveObject(PL03);
}


public func IsChemicalProduct() { return(1); }
func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(300); }