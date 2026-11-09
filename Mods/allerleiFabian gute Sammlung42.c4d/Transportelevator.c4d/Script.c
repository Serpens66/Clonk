/*-- Fahrstuhl --*/

#strict

#include CXEC
#include ELEF

protected func Initialize()
{
if(FindObject2(Find_ID(ELBS),Find_Distance(100))) RemoveObject(FindObject2(Find_ID(ELBS),Find_Distance(100)));
if(FindObject2(Find_ID(ELBS),Find_Distance(100))) RemoveObject(FindObject2(Find_ID(ELBS),Find_Distance(100)));
  pCase = CreateObject(BELC,+0,+27,GetOwner());
  SetObjectOrder(this(), pCase);
  SetAction("LiftCase",pCase);
  return(1);
}
