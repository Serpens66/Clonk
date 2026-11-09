/*-- Drehrad --*/

#strict 3

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
  if((GetCon() ?? 0) < 100) return;
  if(And((!Equal(GetOwner(), GetOwner(pClonk))), (!Equal(GetOwner(), NO_OWNER)))) return;
  CreateMenu(GetID(),pClonk);
  var obj= Flaschenzug;
  if(Or(Or(Or(Or(Or(!LocalN("Hlp_1a",obj), !LocalN("Hlp_1b",obj)), !LocalN("Hlp_2",obj)), !LocalN("Hlp_3",obj)), !LocalN("Hlp_4a",obj))
  , !LocalN("Hlp_4b",obj)))
  {
  AddMenuItem("$Repair$","Repair",CXCN,pClonk,nil,pClonk);
  }
  else
  {
  if(And(Tool, ObjectCall(Tool,"CallStop_Menu"))) ObjectCall(Tool,"Stop");
  if(And(Tool, ObjectCall(Tool,"Condition_MenuItem"))) AddMenuItem(Tool->~ GetMenuCaptain(),"UseTool",Tool->~ GetMenuItem(),pClonk);
  if(TurnedLeft())
    {
    if(Or((And((!!SEqual(GetAction(Boden), "EinziehenLinks")), (Equal(GetPhase(Boden), 11)))), (!!SEqual(GetAction(Boden), "Idle"))))
      AddMenuItem("$FloorDrawOut$","MoveFloor",LMM1,pClonk,nil,1);
    if(And((!!SEqual(GetAction(Boden), "AusfahrenLinks")), (Equal(GetPhase(Boden), 11))))
      AddMenuItem("$FloorDrawIn$","MoveFloor",LMM5,pClonk);
    }
  else
    {
    if(Or((And((!!SEqual(GetAction(Boden), "Einziehen")), (Equal(GetPhase(Boden), 11)))), (!!SEqual(GetAction(Boden), "Idle"))))
      AddMenuItem("$FloorDrawOut$","MoveFloor",LMM5,pClonk,nil,1);
    if(And((!!SEqual(GetAction(Boden), "Ausfahren")), (Equal(GetPhase(Boden), 11))))
      AddMenuItem("$FloorDrawIn$","MoveFloor",LMM1,pClonk);
    }
  var i,idDef;
  while(idDef=GetDefinition(S42MigrationStep(i, 1, true),C4D_Vehicle))
   if(DefinitionCall(idDef,"IsTooool"))
    if(!Equal(GetID(Tool), idDef))
     AddMenuItem("$Fasten$","ChangeTool",idDef,pClonk);
  if(!Hilfsclonk) AddMenuItem("$Buy$","Kaufen",KILC,pClonk,nil,pClonk);
	     else AddMenuItem("$Release$","Entlassen",REAC,pClonk);
  }
  if(Equal(GetID(Flaschenzug), GRS3))
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
  if((GetWealth(GetOwner()) ?? 0)< 10) return(S42MigrationFirst(Message("$NotEnoughMoney$",pClonk),Sound("CommandFailure1")));
  SetWealth(GetOwner(),(GetWealth(GetOwner()) ?? 0)-10);
  Sound("Cash");
  Sound("Build*");
  // Schaden beheben
  DoDamage(-(GetDamage(Flaschenzug) ?? 0),Flaschenzug);
  DoDamage(-(GetDamage() ?? 0));
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
    if(And(((GetY(Tool) ?? 0)+10 > (GetY(Boden) ?? 0)), Tool)) return(Message("Seil oder Tool im Weg!",this()));
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
  if((GetWealth(GetOwner()) ?? 0)< 10) return(S42MigrationFirst(Message("$NotEnoughMoney$",pClonk),Sound("CommandFailure1")));
  SetWealth(GetOwner(),(GetWealth(GetOwner()) ?? 0)-10);
  Sound("Cash");
  Hilfsclonk= CreateObject(CLNK,nil,+10,GetOwner());
  CreateContents(ROCK,Hilfsclonk);
  Message("Hilfsclonk gekauft.",Hilfsclonk);
  FadeInClonk(Hilfsclonk,255);
  Hilfsclonk_TimerCall();
}

private func Hilfsclonk_TimerCall(iNum)
{
  var c= iNum;
  if(!Hilfsclonk) return(nil);
  if((GetDamage(Hilfsclonk) ?? 0)> 80) return(Entlassen());
  if(And((!!SEqual(GetAction(Hilfsclonk), "Push")), (Equal(GetDir(Hilfsclonk), DIR_Left)))) SetDir(DIR_Right,Hilfsclonk);
  if(!SEqual(GetAction(Hilfsclonk), "Push")) { SetCommand(Hilfsclonk,"Grab",this()); S42MigrationStep(c, 1, true); }
  if((c ?? 0) >= 30) return(Entlassen());
  ScheduleCall(this(),"Hilfsclonk_TimerCall",35,nil,c);
}

private func FadeInClonk(pTarget,iAlpha)
{
  iAlpha= BoundBy((iAlpha ?? 0)-15,nil,255);
  SetClrModulation(RGBa(255,255,255,iAlpha),pTarget);
  // Fertig
  if(Equal(iAlpha, nil)) return(S42MigrationFirst(SetCommand(pTarget,"Grab",this()),SetDir(DIR_Right,pTarget),SetCategory(4,pTarget),SetAlive(nil,pTarget)));
  ScheduleCall(this(),"FadeInClonk",1,nil,pTarget,iAlpha);
}

private func Entlassen(fNoMsg)
{
  if(!Hilfsclonk) return;
  SetCommand(Hilfsclonk,"UnGrab");
  if(!fNoMsg) Message("$Released$",Hilfsclonk); 
  FadeOutClonk(Hilfsclonk);
}

private func FadeOutClonk(pTarget)
{
  if(!GetClrModulation(pTarget)) SetClrModulation(RGB(255,255,255),pTarget);
  SetClrModulation(DoRGBaValue(GetClrModulation(pTarget),Min(15,255-(GetRGBValue(GetClrModulation(pTarget),nil) ?? 0)),nil),pTarget);
  if((GetObjectBlitMode(pTarget) ?? 0) & 4) SetColorDw(DoRGBaValue(GetColorDw(pTarget),Min(15,255-(GetRGBValue(GetColorDw(pTarget),nil) ?? 0)),nil),pTarget);
  if(Or(Equal(GetRGBValue(GetColorDw(pTarget),nil), 255), Equal(GetRGBValue(GetClrModulation(pTarget),nil), 255)))
    RemoveObject(pTarget,(ContentsCount(nil,pTarget) ?? 0)-1);
  else
    ScheduleCall(this(),"FadeOutClonk",1,nil,pTarget);
}

private func Drehen(idItem)
{
  if(!Flaschenzug) return(nil);
  Flaschenzug ->~ RemoveHelpers();
  Flaschenzug ->~ RemoveDrehrad();
  Flaschenzug ->~ RemoveTool();
  Flaschenzug ->~ RemoveBoden();
  SetPosition(GetX(Flaschenzug),(GetY(Flaschenzug) ?? 0) -1, Flaschenzug);
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
  if((GetCon() ?? 0) < 100) return;
  SetAction("Normal");
  if(!Hostile(GetController(pClonk),GetOwner())) SetPlrView(GetOwner(pClonk),Tool);
  Tool->~ DoControlDig(pClonk);
}

protected func ControlLeft(pClonk,pTurned)
{
  [Bewegen]
  if(And(TurnAround, !pTurned)) return(ControlRight(pClonk,1));
  if((GetCon() ?? 0) < 100) return;
  SetAction("Work");
  if(!Hostile(GetController(pClonk),GetOwner())) SetPlrView(GetOwner(pClonk),Tool);
  Tool->~ DoControlUp(pClonk);
}

protected func ControlRight(pClonk,pTurned)
{
  [Bewegen]
  if(And(TurnAround, !pTurned)) return(ControlLeft(pClonk,1));
  if((GetCon() ?? 0) < 100) return;
  SetAction("Work");
  if(!Hostile(GetController(pClonk),GetOwner())) SetPlrView(GetOwner(pClonk),Tool);
  Tool->~ DoControlDown(pClonk);
}

private func Working()
{
  if(!IsOccupied()) return(ObjectCall(Tool,"Stop"));
  if((GetContact(Tool,-1) ?? 0) & (CNAT_Bottom ?? 0)) return(ObjectCall(Tool,"Stop"));
}

/*-- Status --*/

public func IsOccupied()
{
  if(FindObject(nil,nil,nil,nil,nil,nil,"Push",this(),NoContainer())) return(1);
  return(nil);
  //if(IsOccupiedByPlayer() || IsOccupiedByHelper()) return(1);
}

public func TurnedLeft()
{
  return(Equal(GetID(Flaschenzug), GRS2));
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