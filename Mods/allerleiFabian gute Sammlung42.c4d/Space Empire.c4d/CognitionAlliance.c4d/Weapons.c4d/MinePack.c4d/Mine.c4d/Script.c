/*-- Mine --*/

#strict 3


protected func Initialize()
{
  return(1);
}

protected func Hit()
{
  Sound("Bip");
  SetAction("Ready");
  return(1);
}

private func CheckTarget()
{
  if (Contained()) SetAction("Idle");
  if ((GetDamage() ?? 0) > (MaxDamage() ?? 0)) return(Explode(35));
  if (FindObject(nil,-10,-10,20,15,OCF_Alive,nil,nil,NoContainer())) return(Explode(35));
  return(1);
}

/* Minen packen */

protected func Activate()
{
  [Minen packen|Image=CWMP]
  // Minenpack erzeugen
  var pack = CreateContents(CWMP,Par(nil));
  ObjectCall(pack,"PackMines");
  return(1);
}
  
public func EMP_Reaction()
{
  if ((Random(35) ?? 0) < (Par(nil) ?? 0)) SetAction("Idle");
  return(nil);
}
 
func MaxDamage() {return(30);}
public func IsBulletTarget(pObj) { return(true); }
// Status
public func IsEquipment() { return(1); }
public func IsMine() { return(1); }
