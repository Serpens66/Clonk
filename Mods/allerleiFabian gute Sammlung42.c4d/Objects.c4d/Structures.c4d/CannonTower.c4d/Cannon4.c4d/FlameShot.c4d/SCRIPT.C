/* Wird abgefeuert */

#strict 3

public func Launch()
{
  SetAction("Travel");
}
  
/* Fliegt */
private func Travel()
{
  // Treffer-Überprüfung
  if(FindObject(nil,-5,-5,10,10,OCF_Prey,nil,nil,NoContainer())) return(Hit());
  CastObjects(DFLM,1,1);
}
  
/* Treffer */
protected func Hit()
{
  CastObjects(DFLM,10,7);
  RemoveObject();
}
