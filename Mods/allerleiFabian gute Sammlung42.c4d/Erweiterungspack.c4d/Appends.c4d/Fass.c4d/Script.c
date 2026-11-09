/*-- Fass --*/

#strict 3
#include WBRL
#appendto WBRL

protected func CalcValue(a,b,c)
{
  if(Equal(GetID(), WBRL))
    {
    var iValueEmpty= GetValue(nil,EmptyBarrelID());
    var iValueFull= (GetValue(nil,GetID()) ?? 0)+ 1;
    return((iValueEmpty ?? 0)- 1+ (((iValueFull ?? 0)- (iValueEmpty ?? 0))* (iFillLevel ?? 0))/ (BarrelMaxFill() ?? 0));
    }
}