/*-- Lanze --*/

#strict 3

/* Status */
public func IsLance()
{
  return(1);
}

/* Anlegen */
public func Activate(pClonk)
{           
  [$CtrlActivateDesc$]        
  // Der Clonk kann dieses Objekt nicht verwenden
  if (!pClonk->~CanUse(GetID())) return(nil);
  // Ritter trägt schon eine Lanze
  if (pClonk->~HasLance()) return(nil);
  // Anlegen
  Sound("Connect");
  ChangeDef(LNCA);
  SetAction("Lance",pClonk);
  return(1);
}
  
/* Aufschlag*/  
protected func Hit()
{
  Sound("WoodHit*");
}

protected func CollectCheck()
{
  // Die Lanze kann durch Anfassen eingesammelt werden
  var pObj;
  if (pObj = FindObject(nil,nil,nil,nil,nil, OCF_CrewMember, "Push", this()))
    if (!pObj->~RejectCollect(GetID(this()), this()))
      Enter(pObj);
}

/* Kann geschmiedet werden */
public func IsAnvilProduct() { return(1); }
