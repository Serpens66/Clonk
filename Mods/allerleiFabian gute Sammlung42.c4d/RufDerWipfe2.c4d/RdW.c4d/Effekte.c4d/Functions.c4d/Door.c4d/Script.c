#strict
#include _ZF9

local Door;

public func Initialize()
{
  _inherited();
  if(!Door)
    Door=CreateDoor(this(),DoorID());
  SetEntrance(0);
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
  if(!pTarget)  return false;
  if(!idDef)    idDef=DO1_;
  if(!GetDefCoreVal("Entrance","DefCore",GetID(pTarget),2)) return false;
  if(!iX)       iX=GetDefCoreVal("Entrance","DefCore",GetID(pTarget),0)+pTarget->~DoorXOffset();
  if(!iY)       iY=GetDefCoreVal("Entrance","DefCore",GetID(pTarget),1)+pTarget->~DoorYOffset();
  var pDoor = CreateObject(idDef,AbsX(GetX(pTarget)+iX),AbsY(GetY(pTarget)+iY),GetOwner(pTarget));
  pDoor->~Set(pTarget,iX,iY);
  return pDoor;
}

public func DoorXOffset(){  return 3;}
public func DoorYOffset(){  return 0;}