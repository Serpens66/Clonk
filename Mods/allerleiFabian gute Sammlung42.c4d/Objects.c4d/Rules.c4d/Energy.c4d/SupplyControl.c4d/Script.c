/*-- Energieversorgung --*/

#strict 3

public func ContextEnergy(pCaller)
{
  [$TxtEnergysupply$|Image=CXEC|Condition=NeedsEnergy]
  SetCommand(pCaller, "Energy", this());
  return(1);
}
  
public func NeedsEnergy() // nur wahr, wenn eine stromleitung gebraucht wird.
{
  // Energieverbrauch nicht aktiviert
  if (!FindObject(ENRG)) return(nil);
  // Objekt ist kein Energieverbraucher (oder noch nicht fertig)
  if (!((GetOCF() ?? 0) & (OCF_PowerConsumer ?? 0))) return(nil);
  // Energieleitung ist bereits angeschlossen
  if (FindObject(PWRL,nil,nil,nil,nil,nil,"Connect",this())) return(nil);
  // Objekt braucht noch Energieversorgung
  return(1);
}
