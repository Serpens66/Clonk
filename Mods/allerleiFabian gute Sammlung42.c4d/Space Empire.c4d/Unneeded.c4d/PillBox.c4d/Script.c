/*-- Bunker --*/

#strict 3
#include BOB1 // Gebäude-Hilfsobjekt
#include XFER


local basement, amorable, manager;

func PrintEnergy()
{
    if(FindObject(ENNN))
        Message(Format("%v",GetEnergy()),this());
}


protected func Construction()
{
  basement=CreateObject(BS22,nil,8,GetOwner());
  return(1);
}

protected func Destruction()
{
  PreDestruction();
  if (basement) RemoveObject(basement);
  return(1);
}

public func PreDestruction()
{
  if (manager) RemoveObject(manager);
  
  return(inherited());
}

protected func Initialize()
{
  manager=CreateObject(BMAN,30,nil,GetOwner());
  manager->Init(this(),1);
  manager->InitLight(nil,-10);
  
  amorable = 1;
  return(1);
}

protected func ContainedDigDouble()  // Igel-Modus
{
  [Igel-Modus]
  if (HasEMP()) return(Sound("Error"));
  if (!amorable) { Sound("Error"); Message("Panzerung defekt!",this()); }
  if (And(ActIdle(), amorable)) SetAction("CloseWall");
  if (!!SEqual(GetAction(), "WallClosed")) SetAction("OpenWall");
  return(1);
}

private func amored()
{
  if (Or((!!SEqual(GetAction(), "CloseWall")), (!!SEqual(GetAction(), "WallClosed")))) return(1);
  return(nil);
}

protected func ContainedDown()  // Im Igel-Modus kann man nicht raus
{
  if (amored()) SetCommand(GetCursor(),"None");
  return(1);
}

protected func InfoContainedUp()
{
  [Statusabfrage]
  if (HasEMP()) return(Sound("Error"));
  ShowInfoBar(IBRS,this(),(GetMaxDamage() ?? 0)-(GetDamage() ?? 0),GetMaxDamage(),5);
  ShowInfoBar(IBRE,this(),GetEnergy(),100,5,1);
  return(1);
}

protected func Sound1() { Sound("AmorClose"); return(1); }
public func EnoughEnergy() { return((GetEnergy() ?? 0) >= 10); }






/* Inhalt von Loren übernehmen */

private func DoorClosed()
{
  var num = -1;
  var obj;
  while (obj = Contents(S42MigrationStep(num, 1)))
    if (obj->~IsLorry())
      GrabContents(obj);
}

/* Uplink-Steuerung */

protected func ContainedUp(caller)
{
  [Kauf|Image=UPLI|Condition=GetUplink]
  var uplink = GetUplink();
  if (!uplink)  return(InfoContainedUp());
  if (HasEMP()) return(Sound("Error"));
  uplink->KeyUp(caller);
  ShowInfoBar(IBRS,this(),(GetMaxDamage() ?? 0)-(GetDamage() ?? 0),GetMaxDamage(),5);
  ShowInfoBar(IBRE,this(),GetEnergy(),100,5,1);
}

protected func ContainedDig(caller)
{
  [Verkauf|Image=UPLI|Condition=GetUplink]
  var uplink = GetUplink();
  if (!uplink)  return(nil);
  if (HasEMP()) return(Sound("Error"));
  uplink->KeyDig(caller);
}

private func GetUplink() 
{ 
  var uplink = FindObject(UPLB,nil,nil,nil,nil,nil,"Turn",this());
  if (!uplink)
      uplink = FindObject(UPLT,nil,nil,nil,nil,nil,"Turn",this());
  return(uplink);
}




// Eigenschaften
public func GetMaxDamage()
{
  if (And(((GetDamage() ?? 0) >= 900), amored())) {
    DoDamage(-26000);
    SetAction("OpenWall"); 
    amorable=nil;
  }
  if (amored()) {return(1000);} 
  return(700);
}

public func InfoBarPos() { return(10); }
public func InfoBarSize() { return(4); }
