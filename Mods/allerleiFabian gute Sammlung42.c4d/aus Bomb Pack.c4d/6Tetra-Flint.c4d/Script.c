/*-- Nitro-Flint --*/

#strict

#include FLNT
#include EFL0

public func ExplodeSize() 
{ 
  return(100); 
}

/* Kann in der Chemiefabrik hergestellt werden */
public func IsChemicalProduct() { return(1); }

func IsAlchemContainer() { return(true); }

public func GetResearchBase() { return(EFL2); }

