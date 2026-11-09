/*--- Saw ---*/

#strict

protected func Hit()
{
  Sound("MetalHit*");
	return(1);
}

public func Activate(caller) 
{

  SetComDir(COMD_Stop(), caller);

  CreateMenu(DK8Z, caller, this);
  AddMenuItem("Up", "Up", DK8Z, caller, 0, caller);
  AddMenuItem("Diagonal", "Diagonal", DK8Z, caller, 0,caller);
  AddMenuItem("Straight", "Straight", DK8Z, caller, 0,caller);
  return(1);
}

protected func Up(idWEG, pObj) {
  DoJumpUp(pObj);
  Sound("BOING");
  return(Remover());  
}

protected func Diagonal(idOOOOO, pObj) {
  DoJumpDiagonal(pObj);
  Sound("BOING");
  return(Remover());  
}

protected func Straight(idRGR, pObj) {
  DoJumpStraight(pObj);
  Sound("BOING");
  return(Remover());  
}

protected func Remover()
{
	RemoveObject();
}

protected func DoJumpUp(target) {
  SetPosition(GetX(target), GetY(target) - 1, target);
  ObjectSetAction(target, "Jump"); 
  var dir = -1;
  if (GetDir(target) == 1) dir = +1;

  SetSpeed(10 * dir, -120, target); 

  AddEffect("IntHasNoFallDamage",target,1,0,target);
}

protected func DoJumpDiagonal(target) {
  SetPosition(GetX(target), GetY(target) - 1, target);

  ObjectSetAction(target, "Jump");
  
  var dir = -1;
  if (GetDir(target) == 1) dir = +1;

  SetSpeed(35 * dir, -70, target);

  AddEffect("IntHasNoFallDamage",target,1,0,target);
}

protected func DoJumpStraight(target) {
  SetPosition(GetX(target), GetY(target) - 1, target);

  ObjectSetAction(target, "Jump");
  
  var dir = -1;
  if (GetDir(target) == 1) dir = +1;

  SetSpeed(70 * dir, -50, target);

  AddEffect("IntHasNoFallDamage",target,1,0,target);
}


public func IsAnvilProduct() { return(1); }
