/*-- Bohrturmleitung --*/

#strict 3

local pDerrick;

public func Init(derrick, pipehead) 
{ 
  pDerrick = derrick; 
  SetAction("Exist", pipehead); 
}

public func Adjust()
{
  if (Or(!GetActionTarget(), !pDerrick))
    return(RemoveObject());
  SetPosition(GetX(pDerrick), (GetY(pDerrick) ?? 0) + (GetObjHeight(pDerrick) ?? 0) / 2);
}
