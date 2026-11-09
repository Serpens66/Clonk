#strict 3
#include _ZF9

local Door;

public func Initialize()
{
  _inherited();
  if(!Door)
    Door=CreateDoor(this(),DoorID());
  SetEntrance(nil);
}

public func Destruction()
{
  _inherited();
  if(Door) RemoveObject(Door);
}

protected func ActivateEntrance(pObj)
{
  if(!Door)                 return SetEntrance(1);     // Keine Tür? Eingang offen
  if(!Door->~IsOpen())      Door->~Open();      // Tür ist zu? Öffnen.
  if(Door->~IsReallyOpen()) Door->~SetState(1); // Tür ist offen? Befehl zu warten geben.
  return(1);
}

public func DoorID()  { return DO1_;}

global func CreateDoor(pTarget,idDef,iX,iY)
{
  if(!pTarget)  pTarget=this();
  if(!pTarget)  return nil;
  if(!idDef)    idDef=DO1_;
  if(!GetDefCoreVal("Entrance","DefCore",GetID(pTarget),2)) return nil;
  if(!iX)       iX=(GetDefCoreVal("Entrance","DefCore",GetID(pTarget),nil) ?? 0)+(pTarget->~DoorXOffset() ?? 0);
  if(!iY)       iY=(GetDefCoreVal("Entrance","DefCore",GetID(pTarget),1) ?? 0)+(pTarget->~DoorYOffset() ?? 0);
  var pDoor = CreateObject(idDef,AbsX((GetX(pTarget) ?? 0)+(iX ?? 0)),AbsY((GetY(pTarget) ?? 0)+(iY ?? 0)),GetOwner(pTarget));
  pDoor->~Set(pTarget,iX,iY);
  return pDoor;
}

public func DoorXOffset(){  return 3;}
public func DoorYOffset(){  return nil;}