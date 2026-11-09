/*-- LZD --*/

#strict

local on;
local target;
local upgraded;
local starget;
local next;
local user;
local jumper;
local jumpermark;

func Initialize() 
{
  ScheduleCall(0,"JumperSet",2);
  on = 0;
  upgraded = 0;
  return(1);
}

func JumperSet()
{
  if(GetID(Contained()) == PUD_)
  {
   jumper = Contained();
  }
  return(1);
}

func Entf()
{
  if(target)
  {
   target->RemoveObject();
  }
  Explode(5);
  return(1);
}

func Activate()
{
  SetOwner(GetOwner(Contained()));
  CreateMenu(A55B,Contained(),0,0,0,3);
  AddMenuItem("An/Aus","Switch",LZDS,Contained());
  
  if(upgraded)
  {
   AddMenuItem("Object Markieren","Select",ARRW,Contained());
  }
  
  if(starget)
  {
   AddMenuItem("Markierung löschen","DeSelect",ARRW,Contained());
  }
  
  if(jumper)
  {
   if(LocalN("kindersicherung",jumper))
   {
    AddMenuItem("Jumper Tarnen","Cloak",PUD2,Contained());
   }
   AddMenuItem("Jumperortung","JumperMark",LZD5,Contained());
   
   AddMenuItem("Jumper Tarnen","Cloak2",PUD2,Contained());
 
   AddMenuItem("Jumper Heckluke","Luke",PUD7,Contained());
  }
  return(1);
}

func JumperMark()
{
  if(jumpermark)
  {
   jumpermark = 0;
   Message("Jumperortung ist aus!",this());
   if(target)
   {
    LocalN("jumper",target)=0;
   }
   return(1);
  }
  
  if(!jumpermark)
  {
   jumpermark = 1;
   Message("Jumperortung ist an!",this());
   if(target)
   {
    LocalN("jumper",target)=jumper;
   }
   return(1);
  }
  return(1);
}
  
func Luke()
{
  jumper->Luke();
  return(1);
}

func Cloak()
{
  if(GetAction(jumper) eq "Open")
  {
   jumper->SetAction("Stand");
  }
  jumper->Cloak();
  LocalN("pUser",jumper) = Contained();
  jumper->SetOwner(GetOwner());
  ScheduleCall(0,"CloseJumper",21);
  return(1);
}

func Cloak2()
{
  if(GetAction(jumper) eq "Open")
  {
   jumper->Luke();
   ScheduleCall(0,"Cloak2",22);
   return(1);
  }
  jumper->Cloak();
  LocalN("pUser",jumper) = Contained();
  jumper->SetOwner(GetOwner());
  return(1);
}

func CloseJumper()
{
  LocalN("open",jumper) = 0;
  return(1);
}

func DeSelect()
{
  if(starget)
  {
   starget=0;
  }
  return(1);
}

func Select()
{
  CreateMenu(ARRW,Contained(),0,0,0,3);
  for(next in FindObjects(Find_Or(Find_Category(C4D_Vehicle),Find_OCF(OCF_Collectible()),Find_OCF(OCF_Alive())),Find_Exclude(Contained()),Find_Exclude(this()),Find_Distance(20)))
  {
   AddMenuItem("%s Auswählen","Mark",GetID(next),Contained(),0,next);
  }
  return(1);
}

func Mark(trash,ziel)
{
  starget = ziel;
  Message("Zielobjekt: %v",this(),GetName(starget));
  return(1);
}

func Switch()
{
  SetOwner(GetOwner(Contained()));

  if(on == 0)
  {
   on = 1;
   
   if(upgraded == 1)
   {
    target=CreateObject(LZD1);
    target->SetOwner(GetOwner());
    LocalN("jumper",target)=jumper;
    
    if(!jumpermark)
    {
     LocalN("jumper",target)=0;
    }
    return(1);
   }
   
   target=CreateObject(LZDS);
   target->SetOwner(GetOwner());
   LocalN("jumper",target)=jumper;
   
   if(!jumpermark)
   {
    LocalN("jumper",target)=0;
   }
   return(1);
  }
  
  if(on == 1)
  {
   on = 0;
   target->RemoveObject();
   return(1);
  }
}

func Check()
{
  if(starget)
  {
   if(upgraded)
   {
    if(on)
    {
     LocalN("starget",target) = starget;
    }
   }
  }
  
  if(!Contained())
  {
   if(on == 1)
   {
    Switch();
   }
  }

  if(Contained() == GetCursor(GetOwner()))
  {
   return(1);
  }

  if(on == 1)
  {
   Switch();
   return(1);
  }
  return(1);
}

/* Upgrade */

public func IsUpgradeable(id uid) 
{
  if(upgraded == 1)
  {
   return();
  }

  if(uid == KRFL)
  {
   return("Aufrüsten");
  }
}

public func Upgrade(id uid) 
{
  if(on)
  {
   Switch();
  }

  if(upgraded == 1) return();

  if(uid == KRFL) 
  {
    upgraded = 1;
    SetPicture(10,100,130,100);
    if(on == 1)
    {
     Switch();
    }
    return(true);
  }
}