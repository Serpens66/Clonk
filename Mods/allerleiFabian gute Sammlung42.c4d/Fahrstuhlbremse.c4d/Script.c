/*-- Elevator Lock --*/

#strict
#appendto ELEC // Fahrstuhl, Bunkerfahrstuhl und Transportfahrtuhl
// #appendto ELC2 // Seilbahnkorb, fkt so nicht, muss in skript selbst gemacht werden
#appendto ELE_ // Metallfahrstuhl

local bLock;

public func DoElevatorLock(pObj) {
  // Ein Geräusch machen
  Sound("Click");
  
  if(!bLock) {Message("$TxtElevatorLockOn$",this()); this()->Halt(); return(bLock=1);}
  if( bLock) {Message("$TxtElevatorLockOff$",this()); return(bLock=0);}
}

public func ContextElevatorLockTurnOn(pObj) 
{
  [$TxtElevatorLockTurnOn$|Image=ELEC|Condition=IsNotInElevatorLock|Desc=$TxtElevatorLockDesc$]
  DoElevatorLock(pObj);
}
 
public func ContextElevatorLockTurnOff(pObj) 
{
  [$TxtElevatorLockTurnOff$|Image=ELEC|Condition=IsInElevatorLock|Desc=$TxtElevatorLockDesc$]
  DoElevatorLock(pObj);
}

public func IsNotInElevatorLock() {if(!FindObject(_ELL)) return(0); return(!bLock);}
public func IsInElevatorLock() {if(!FindObject(_ELL)) return(0); return(bLock);}

private func MoveToWaitingClonk()
{
  // Bremse an? 
  if(bLock&&FindObject(_ELL)) return(0);

  _inherited();            
}

public func ControlDig(pObj)
{
  if(bLock&&FindObject(_ELL)) return(0);
  _inherited();
}

public func ControlDigDouble(pObj)  
{
  [$TxtDrillshaft$|Image=ECS1:2|Method=Classic|Condition=NoContext]
  if(bLock&&FindObject(_ELL)) return(0);
  if(!GetPlrCoreJumpAndRunControl(GetController(pObj)))
    this()->DoControlDig(pObj);

  return(1); 
}

public func ControlDigReleased(pObj)
{
  if(bLock&&FindObject(_ELL)) return(0);
  _inherited();
}

public func ControlDownSingle(pObj) 
{ 
  if(bLock&&FindObject(_ELL)) return(0);
  _inherited();
}

public func ControlDown(pObj)
{
  if(bLock&&FindObject(_ELL)) return(0);
  _inherited();
}

public func ControlDownReleased(pObj)
{
  if(bLock&&FindObject(_ELL)) return(0);
  _inherited();
}

public func ControlUp(pObj)         
{ 
  if(bLock&&FindObject(_ELL)) return(0);
  _inherited();
}

public func ControlUpReleased(pObj)
{
  if(bLock&&FindObject(_ELL)) return(0);
  _inherited();
}

private func SelfMovement()
{
  if(bLock&&FindObject(_ELL)) return(0);
  _inherited();
}
