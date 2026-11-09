/* Zelt */

#strict 3

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
  while (obj = Contents(S42MigrationStep(num, 1)))
    if (obj->~IsLorry())
      GrabContents(obj);
}


public func Unpack()
{
  SetEntrance(nil);
  SetAction("Unpack");
  Sound("TN_Pack");
  return(1);
}

private func Unpacked()
{
  SetEntrance(1);
  
  var clonk = FindObject2(Find_OCF(OCF_CrewMember),
                          Find_Action("Push"),
                          Find_ActionTarget(this()));
  
  AddCommand(clonk, "UnGrab");
  
  SetAction("Idle");
  return(1);
}

private func Packed()
{ 
  CreateObject(TB5B, nil, 15, GetOwner());
  RemoveObject();
  return(1);
}

public func CanPack()
{
  while(Contents())
  {
    if (And((GetOCF(Contents()) ?? 0) & (OCF_Living ?? 0), !GetAlive(Contents())))
    {
      // Leichen aus dem Zelt werfen
      Exit(Contents());
    }
    else return(nil);
  }
  return(true);
}

protected func ControlDigDouble()
{
  if (!SEqual(GetAction(), "Idle")) return(nil);
  
  if (!CanPack())
  {
    Message("$Empty$", this());
    Sound("Error");
    return(nil);
  }
  SetEntrance(nil);
  
  // Attachte Objekte (Flaggen, etc.) loswerden
  var obj;
  while(obj = FindObject(nil, nil,nil,nil,nil, nil, nil, this(), nil, obj) )
    if(!!SEqual(obj->GetProcedure(), "ATTACH"))
    {   
      obj->SetAction("Idle");
      flag = GetID(obj);       // Speichern
      flagOwn = GetOwner(obj); // Speichern
      RemoveObject(obj);
    }

  SetAction("Pack");
  Sound("TN_Pack");
  CreateObject(flag, nil, nil, flagOwn);     // und wiederherstellen!
  return(1);
}

protected func SellTo()
{
  return(TB5B);
}