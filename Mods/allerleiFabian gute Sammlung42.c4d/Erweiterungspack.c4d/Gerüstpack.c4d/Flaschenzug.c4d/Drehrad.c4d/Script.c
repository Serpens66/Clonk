/*-- Drehrad --*/

#strict

local Flaschenzug,Tool,Hilfsclonk,Boden,TurnAround;

protected func Initialize()
{
  SetCategory(4);
  SetAction("Normal");
}

public func Init(pGeruest,pTool,pFloor)
{
  Flaschenzug= pGeruest;
  Tool= pTool;
  Boden= pFloor;
}

protected func ControlUp(pClonk)
{
  [Menü]
  if(GetCon() < 100) return();
  if((GetOwner() != GetOwner(pClonk)) && (GetOwner()!= NO_OWNER())) return();
  CreateMenu(GetID(),pClonk);
  var obj= Flaschenzug;
  if(!LocalN("Hlp_1a",obj) || !LocalN("Hlp_1b",obj) || !LocalN("Hlp_2",obj) || !LocalN("Hlp_3",obj) || !LocalN("Hlp_4a",obj)
  || !LocalN("Hlp_4b",obj))
  {
  AddMenuItem("$Repair$","Repair",CXCN,pClonk,0,pClonk);
  }
  else
  {
  if(Tool && ObjectCall(Tool,"CallStop_Menu")) ObjectCall(Tool,"Stop");
  if(Tool && ObjectCall(Tool,"Condition_MenuItem")) AddMenuItem(Tool->~ GetMenuCaptain(),"UseTool",Tool->~ GetMenuItem(),pClonk);
  if(TurnedLeft())
    {
    if(((GetAction(Boden) eq "EinziehenLinks") && (GetPhase(Boden)== 11)) || (GetAction(Boden) eq "Idle"))
      AddMenuItem("$FloorDrawOut$","MoveFloor",LMM1,pClonk,0,1);
    if((GetAction(Boden) eq "AusfahrenLinks") && (GetPhase(Boden)== 11))
      AddMenuItem("$FloorDrawIn$","MoveFloor",LMM5,pClonk);
    }
  else
    {
    if(((GetAction(Boden) eq "Einziehen") && (GetPhase(Boden)== 11)) || (GetAction(Boden) eq "Idle"))
      AddMenuItem("$FloorDrawOut$","MoveFloor",LMM5,pClonk,0,1);
    if((GetAction(Boden) eq "Ausfahren") && (GetPhase(Boden)== 11))
      AddMenuItem("$FloorDrawIn$","MoveFloor",LMM1,pClonk);
    }
  var i,idDef;
  while(idDef=GetDefinition(i++,C4D_Vehicle()))
   if(DefinitionCall(idDef,"IsTooool"))
    if(GetID(Tool)!= idDef)
     AddMenuItem("$Fasten$","ChangeTool",idDef,pClonk);
  if(!Hilfsclonk) AddMenuItem("$Buy$","Kaufen",KILC,pClonk,0,pClonk);
	     else AddMenuItem("$Release$","Entlassen",REAC,pClonk);
  }
  if(GetID(Flaschenzug) == GRS3)
    {
    AddMenuItem("$Turn$","Drehen",GRS2,pClonk);
    }
  else
    {
    AddMenuItem("$Turn$","Drehen",GRS3,pClonk);
    }  
  if(Flaschenzug) AddMenuItem("$TearOff$","Abreissen",CXCN,pClonk);
}

private func Repair(idItem,pClonk)
{
  if(GetWealth(GetOwner())< 10) return(Message("$NotEnoughMoney$",pClonk),Sound("CommandFailure1"));
  SetWealth(GetOwner(),GetWealth(GetOwner())-10);
  Sound("Cash");
  Sound("Build*");
  // Schaden beheben
  DoDamage(-GetDamage(Flaschenzug),Flaschenzug);
  DoDamage(-GetDamage());
  // Seile etc. evtl. neu spannen.
  Flaschenzug->~ CreateAccessories();
}

private func UseTool()
{
  Tool->~ UseTool();
}

private func MoveFloor(idItem,pOut)
{
  Sound("Click");
  
  if(pOut)
    {
    if((GetY(Tool)+10 > GetY(Boden)) && Tool) return(Message("Seil oder Tool im Weg!",this()));
    if(TurnedLeft()) ObjectSetAction(Boden,"AusfahrenLinks");
		else ObjectSetAction(Boden,"Ausfahren");    
    }
  else
    {
    if(TurnedLeft()) ObjectSetAction(Boden,"EinziehenLinks");
		else ObjectSetAction(Boden,"Einziehen");
    }
}

private func ChangeTool(idNewTool)
{
  Flaschenzug->~ CreateNewTool(idNewTool,this());
}

private func Kaufen(idItem,pClonk)
{
  if(GetWealth(GetOwner())< 10) return(Message("$NotEnoughMoney$",pClonk),Sound("CommandFailure1"));
  SetWealth(GetOwner(),GetWealth(GetOwner())-10);
  Sound("Cash");
  Hilfsclonk= CreateObject(CLNK,0,+10,GetOwner());
  CreateContents(ROCK,Hilfsclonk);
  Message("Hilfsclonk gekauft.",Hilfsclonk);
  FadeInClonk(Hilfsclonk,255);
  Hilfsclonk_TimerCall();
}

private func Hilfsclonk_TimerCall(iNum)
{
  var c= iNum;
  if(!Hilfsclonk) return(0);
  if(GetDamage(Hilfsclonk)> 80) return(Entlassen());
  if((GetAction(Hilfsclonk) eq "Push") && (GetDir(Hilfsclonk)== DIR_Left())) SetDir(DIR_Right(),Hilfsclonk);
  if(GetAction(Hilfsclonk) ne "Push") { SetCommand(Hilfsclonk,"Grab",this()); c++; }
  if(c >= 30) return(Entlassen());
  ScheduleCall(this(),"Hilfsclonk_TimerCall",35,0,c);
}

private func FadeInClonk(pTarget,iAlpha)
{
  iAlpha= BoundBy(iAlpha-15,0,255);
  SetClrModulation(RGBa(255,255,255,iAlpha),pTarget);
  // Fertig
  if(iAlpha== 0) return(SetCommand(pTarget,"Grab",this()),SetDir(DIR_Right(),pTarget),SetCategory(4,pTarget),SetAlive(0,pTarget));
  ScheduleCall(this(),"FadeInClonk",1,0,pTarget,iAlpha);
}

private func Entlassen(fNoMsg)
{
  if(!Hilfsclonk) return();
  SetCommand(Hilfsclonk,"UnGrab");
  if(!fNoMsg) Message("$Released$",Hilfsclonk); 
  FadeOutClonk(Hilfsclonk);
}

private func FadeOutClonk(pTarget)
{
  if(!GetClrModulation(pTarget)) SetClrModulation(RGB(255,255,255),pTarget);
  SetClrModulation(DoRGBaValue(GetClrModulation(pTarget),Min(15,255-GetRGBValue(GetClrModulation(pTarget),0)),0),pTarget);
  if(GetObjectBlitMode(pTarget) & 4) SetColorDw(DoRGBaValue(GetColorDw(pTarget),Min(15,255-GetRGBValue(GetColorDw(pTarget),0)),0),pTarget);
  if(GetRGBValue(GetColorDw(pTarget),0)== 255 || GetRGBValue(GetClrModulation(pTarget),0)== 255)
    RemoveObject(pTarget,ContentsCount(0,pTarget)-1);
  else
    ScheduleCall(this(),"FadeOutClonk",1,0,pTarget);
}

private func Drehen(idItem)
{
  if(!Flaschenzug) return(0);
  Flaschenzug ->~ RemoveHelpers();
  Flaschenzug ->~ RemoveDrehrad();
  Flaschenzug ->~ RemoveTool();
  Flaschenzug ->~ RemoveBoden();
  SetPosition(GetX(Flaschenzug),GetY(Flaschenzug) -1, Flaschenzug);
  DoCon(-1, Flaschenzug);
}

private func Abreissen()
{
  Entlassen(1);
  if(Flaschenzug) PrivateCall(Flaschenzug,"Abreissen");
}

protected func ControlDig(pClonk)
{
  [Anhalten]
  if(GetCon() < 100) return();
  SetAction("Normal");
  if(!Hostile(GetController(pClonk),GetOwner())) SetPlrView(GetOwner(pClonk),Tool);
  Tool->~ DoControlDig(pClonk);
}

protected func ControlLeft(pClonk,pTurned)
{
  [Bewegen]
  if(TurnAround && !pTurned) return(ControlRight(pClonk,1));
  if(GetCon() < 100) return();
  SetAction("Work");
  if(!Hostile(GetController(pClonk),GetOwner())) SetPlrView(GetOwner(pClonk),Tool);
  Tool->~ DoControlUp(pClonk);
}

protected func ControlRight(pClonk,pTurned)
{
  [Bewegen]
  if(TurnAround && !pTurned) return(ControlLeft(pClonk,1));
  if(GetCon() < 100) return();
  SetAction("Work");
  if(!Hostile(GetController(pClonk),GetOwner())) SetPlrView(GetOwner(pClonk),Tool);
  Tool->~ DoControlDown(pClonk);
}

private func Working()
{
  if(!IsOccupied()) return(ObjectCall(Tool,"Stop"));
  if(GetContact(Tool,-1) & CNAT_Bottom) return(ObjectCall(Tool,"Stop"));
}

/*-- Status --*/

public func IsOccupied()
{
  if(FindObject(0,0,0,0,0,0,"Push",this(),NoContainer())) return(1);
  return(0);
  //if(IsOccupiedByPlayer() || IsOccupiedByHelper()) return(1);
}

public func TurnedLeft()
{
  return(GetID(Flaschenzug) == GRS2);
}

/*--------
public func IsOccupiedByPlayer()
{
  var clonk;
  while(clonk= FindObject(0,0,0,0,0,OCF_CrewMember(),"Push",this(),NoContainer(),clonk)) 
   if(clonk!= Hilfsclonk)
    return(1);
}

public func IsOccupiedByHelper()
{
  var clonk;
  while(clonk= FindObject(0,0,0,0,0,OCF_Living(),"Push",this(),NoContainer(),clonk)) 
   if(clonk== Hilfsclonk)
    return(1);
}
--------*/

protected func Destruction()
{
  if(Hilfsclonk) RemoveObject(Hilfsclonk);
  if(Flaschenzug) ObjectCall(Flaschenzug,"KeyDestroyed");
}

protected func Incineration()
{
  Entlassen(1);
  if(Flaschenzug) ObjectCall(Flaschenzug,"KeyDestroyed");
}

protected func IncinerationEx()
{
  Entlassen(1);
  if(Flaschenzug) ObjectCall(Flaschenzug,"KeyDestroyed");
}