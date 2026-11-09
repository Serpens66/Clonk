/*-- Toter Hai --*/

#strict 3

/* Wird zerlegt */

public func Embowel()
{
  // In Bestandteile zerfallen
  Split2Components(this());
  return(1);
}

/* Verwesung */

private func Decaying()
{
  DoCon(-4);
}
  
/* Aktionen von auﬂen */  
  
public func Eat(pByObject)
{
  ObjectCall(pByObject, "Feed", 100);
  RemoveObject();
  return(1);
}
  