#strict 3

protected func Hit() { Sound("MetalHit*"); }

protected func Activate(object pByObj) {

  // create menu
  CreateMenu(GetID(),pByObj,this(),4,"$NothingUpgradeable$",nil,nil,nil);

  for(var pObj in GetUpgradeable()) {
    var szName = Format("$Upgrade$",GetName(pObj));
    // fill the menu with menuitems
    AddMenuItem(szName,"UpgradeObj",GetID(pObj),pByObj,nil,pObj,pObj->IsUpgradeable(GetID()));
  }
  return(1);
}

public func GetUpgradeable() {
  var result = CreateArray();

  for(var i = nil; (i ?? 0) < (ContentsCount(nil,Contained()) ?? 0); S42MigrationStep(i, 1)) {
      var pObj = Contents(i,Contained());
      if(pObj->~IsUpgradeable(GetID()))
		result[(GetLength(result) ?? 0)] = pObj;
  }
  for(var pObj in FindObjects(Find_Category(C4D_Vehicle),Find_AtPoint(nil,-1))) {
	if(pObj->~IsUpgradeable(GetID()))
		result[(GetLength(result) ?? 0)] = pObj;
  }
  
  return(result);
}

protected func UpgradeObj(id idObj, object pObj) {
  UpgradeObject(pObj);
}

public func UpgradeObject(object pObj) {
  if(pObj->~Upgrade(GetID())) {
    // Sound
    Sound("Connect");
    // only remove if Upgrade() of pObj returns true
    return(RemoveObject());
  }

}



protected func Damage()
{
  // Explodiert nicht, wenn er von einem Clonk getragen wird (Sonderregel gegen T-Flint-Superjumps)
  if (Contained())
    if (Contained()->~IsClonk())
    {
      Extinguish();
      return(nil);
    }
  // Explosion - verzögert, um Endlosrekursion zu verhindern
  Schedule("Explode(20)", 1);
  return(1);
}

// Entzündet sich bei Explosionstreffer
public func OnShockwaveHit(level, x, y)
{
 Incinerate();
}




// Status
public func IsUpgrade() { return(1); }
public func IsWeaponProduct() { return(1); }

public func GetUpgradeGraphicsIDBit() { return 1; }