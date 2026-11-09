/*-- Fass --*/

#strict
#include WBRL
#appendto WBRL

protected func CalcValue(a,b,c)
{
  if(GetID()== WBRL)
    {
    var iValueEmpty= GetValue(0,EmptyBarrelID());
    var iValueFull= GetValue(0,GetID())+ 1;
    return(iValueEmpty- 1+ ((iValueFull- iValueEmpty)* iFillLevel)/ BarrelMaxFill());
    }
}