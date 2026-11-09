/* Zelt */

#strict

local flag;
local flagOwn;

public func Initialize()
{
  SetEntrance(1);
  return(1);
}


/* Inhalt von Loren übernehmen */

private func DoorClosed()
{
  var num = -1;
  var obj;
  while (obj = Contents(++num))
    if (obj->~IsLorry())
      GrabContents(obj);
}


public func Unpack()
{
  SetEntrance(0);
  SetAction("Unpack");
  Sound("TN_Pack");
  return(1);
}

private func Unpacked()
{
  SetEntrance(1);
  
  var clonk = FindObject2(Find_OCF(OCF_CrewMember()), 
                          Find_Action("Push"),
                          Find_ActionTarget(this()));
  
  AddCommand(clonk, "UnGrab");
  
  SetAction("Idle");
  return(1);
}

private func Packed()
{ 
  CreateObject(TB5B, 0, 15, GetOwner());
  RemoveObject();
  return(1);
}

public func CanPack()
{
  while(Contents())
  {
    if (GetOCF(Contents()) & OCF_Living() && !GetAlive(Contents()))
    {
      // Leichen aus dem Zelt werfen
      Exit(Contents());
    }
    else return(false);
  }
  return(true);
}

protected func ControlDigDouble()
{
  if (GetAction() ne "Idle") return(0);
  
  if (!CanPack())
  {
    Message("$Empty$", this());
    Sound("Error");
    return(0);
  }
  SetEntrance(0);
  
  // Attachte Objekte (Flaggen, etc.) loswerden
  var obj;
  while(obj = FindObject(0, 0,0,0,0, 0, 0, this(), 0, obj) )
    if(obj->GetProcedure() S= "ATTACH")
    {   
      obj->SetAction("Idle");
      flag = GetID(obj);       // Speichern
      flagOwn = GetOwner(obj); // Speichern
      RemoveObject(obj);
    }

  SetAction("Pack");
  Sound("TN_Pack");
  CreateObject(flag, 0, 0, flagOwn);     // und wiederherstellen!
  return(1);
}

protected func SellTo()
{
  return(TB5B);
}