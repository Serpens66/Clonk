/* Context Use drone functionality */

#strict 3

// Enables object to use drones (with context menu/double dig)

protected func ContextSupportDrone()
{
  [$CtxDrone$|Image=DRSU|Condition=SupportByDrone]
  var pDrone;
  for( pDrone in FindObjects(Find_Func("IsDrone"), Find_Action("Fly"), Find_Owner(GetOwner())))
  {
    pDrone->~TurnOff();
  }
}

protected func SupportByDrone()
{
  return(FindObject2( Find_Func("IsDrone"), Find_Action("Fly"), Find_Owner(GetOwner())));
}

// Kann Ausrüstung benutzen
public func CanUse(id idObj)
{
  // Das Objekt kann natürlich Ausnahmen vornehmen....
  if(DefinitionCall(idObj, "IsDrone")) return(1);
  return(_inherited(idObj));
}