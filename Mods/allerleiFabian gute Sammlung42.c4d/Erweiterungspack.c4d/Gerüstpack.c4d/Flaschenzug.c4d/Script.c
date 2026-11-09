/*-- Flaschenzug --*/

#strict 3

local Drehrad,Tool,Boden;
local Hlp_1a,Hlp_1b,Hlp_2,Hlp_3,Hlp_4a,Hlp_4b;

public func Ausrichtung_XPos() { return(+19); }
public func Ausrichtung_Boden_XPos() { return(+17); }
public func Ausrichtung_Drehrad_XPos() { return(-36); }
public func Bausatz_XPos() { return(-10); }

protected func Initialize()
{
  SetPosition(GetX(this()),(GetY(this()) ?? 0) -1, this());
  SetCategory(1);
  var pClonk= GetCursor(GetOwner());
  CreateMenu(GetID(),pClonk,this(),nil,"$Direction$",nil,1);
  AddMenuItem("$Right$","Richtung",GRS3,pClonk,nil,pClonk,"");
  AddMenuItem("$Left$","Richtung",GRS2,pClonk,nil,pClonk,"");
}

private func Richtung(idItem,pClonk)
{
  if(And((Equal(GetID(), GRS3)), (Equal(idItem, GRS2)))) SetPosition((GetX() ?? 0) -38, GetY());
  if(And((Equal(GetID(), GRS2)), (Equal(idItem, GRS3)))) SetPosition((GetX() ?? 0) +38, GetY());
  if(!Equal(idItem, GetID())) ChangeDef(idItem);  // changedef auf sich selbst anwenden ist boese!
  var X=-25;
  if(Equal(this()->GetID(), GRS2)) X=13;
  if(!(this()->FindGeruest(X,20))) return(this()->CreateAccessories()); // this-> muss nun gemacht werden, um den changeDef quatsch dennoch funktionstüchtig zu machen
  CreateMenu(GetID(),pClonk,this(),nil,"$Adjustment$",nil,1);
  AddMenuItem("$Down$","Ausrichtung",LMM7,pClonk,nil,FindGeruest(X,20),"");
  AddMenuItem("$NoAdjustment$","Ausrichtung",MCMX,pClonk,nil,pClonk,"");
}

private func Ausrichtung(idItem,obj)
{
  if(Equal(idItem, MCMX)) CloseMenu(obj);
  else { SetPosition((GetX(obj) ?? 0)+(Ausrichtung_XPos() ?? 0),(GetY(obj) ?? 0)-45); Sound("Connect"); }
  CreateAccessories();
}

public func CreateAccessories()
{
  // Zusatzboden
  if(!Boden)
    {
    Boden= CreateObject(FZBD,Ausrichtung_Boden_XPos(),+29,GetOwner());
    SetObjectOrder(this(),Boden);
    SetObjectOrder(FindGeruest(-25,20),Boden);
    }
  // Tool erstellen
  if(!Tool)
    {
    var TurnedLeft= nil;
    if(Equal(GetID(), GRS2)) TurnedLeft= 1;
    Tool= CreateObject(StartTool(),DefinitionCall(StartTool(),"GetXPos",TurnedLeft),DefinitionCall(StartTool(),"GetYPos"),GetOwner());
    }
  // Drehrad erstellen
  if(!Drehrad)
    {
    Drehrad= CreateObject(DRRD,Ausrichtung_Drehrad_XPos(),+26,GetOwner());
    if(Equal(GetID(), GRS2)) LocalN("TurnAround",Drehrad)= 1;
    }
  Tool-> Init(this(),Drehrad);
  Drehrad-> Init(this(),Tool,Boden);
  // Hilfsobjekte erstellen
  if(!Hlp_1a) ObjectSetAction(Hlp_1a= CreateObject(GHLP,nil,nil,GetOwner()),"Attach",this());
  if(!Hlp_1b) ObjectSetAction(Hlp_1b= CreateObject(GHLP,nil,nil,GetOwner()),"Attach",this());
  if(!Hlp_2) ObjectSetAction(Hlp_2= CreateObject(GHLP,nil,nil,GetOwner()),"Attach",Drehrad);
  if(!Hlp_3) ObjectSetAction(Hlp_3= CreateObject(GHLP,nil,nil,GetOwner()),"Attach",Tool);
  // Seile erstellen
  if(!Hlp_4a) ObjectSetAction(Hlp_4a= CreateObject(PWRL,nil,nil,GetOwner()),"Connect",Hlp_1a,Hlp_2);
  if(!Hlp_4b) ObjectSetAction(Hlp_4b= CreateObject(PWRL,nil,nil,GetOwner()),"Connect",Hlp_1b,Hlp_3);
}

private func FindGeruest(iX,iY)
{
  var obj;
  while(obj= FindObject(nil,iX,iY,50,50,nil,nil,nil,nil,obj))
   if(ObjectCall(obj,"IsGeruest"))
    return(obj);
}

protected func MenuQueryCancel() { return(1); }

private func Abreissen()
{
  RemoveBoden();
  RemoveHelpers();
  if(Drehrad) ScheduleCall(this(),"Abriss",1,(GetCon(Drehrad) ?? 0) -1,Drehrad,1);
  if(Tool) ScheduleCall(this(),"Abriss",1,(GetCon(Tool) ?? 0) -1,Tool,1);
  CreateObject(CNKT,Bausatz_XPos(),+23,GetOwner());
  ScheduleCall(this(),"Abriss",1,(GetCon() ?? 0) +5,this());
  // Zur Sicherheit...
  Schedule("RemoveObject()",105);
}

private func Abriss(pObj,fNoSound)
{
  if(!pObj) return(nil);
  DoCon(-1,pObj);
  if(And(!fNoSound, !Random(25))) Sound("Build*");
}

protected func Destruction() { RemoveDrehrad();RemoveTool(); RemoveBoden(); RemoveHelpers(); }

protected func Incineration()
{
  SetCategory(2);
  if(Drehrad) Incinerate(Drehrad);
  RemoveHelpers();
  RemoveTool(1);
  RemoveBoden();
}

private func RemoveDrehrad()
{
  if(Drehrad) RemoveObject(Drehrad);
}

private func RemoveHelpers()
{
  if(Hlp_1a) RemoveObject(Hlp_1a);
  if(Hlp_1b) RemoveObject(Hlp_1b);
  if(Hlp_2) RemoveObject(Hlp_2);
  if(Hlp_3) RemoveObject(Hlp_3);
  if(Hlp_4a) RemoveObject(Hlp_4a);
  if(Hlp_4b) RemoveObject(Hlp_4b);
}

private func RemoveTool(fDestroy)
{
  if(Hlp_3) RemoveObject(Hlp_3);
  if(Hlp_4b) RemoveObject(Hlp_4b);
  if(fDestroy)
    {
    ObjectSetAction(Tool,"Idle");
    Incinerate(Tool);
    return(1);
    }
  if(Tool) RemoveObject(Tool);
}

private func RemoveBoden()
{
  if(Boden) RemoveObject(Boden);
}

public func KeyDestroyed()
{
  if(Or(!this(), (GetCon() ?? 0) < 100)) return;
  SetCategory(2);
  RemoveHelpers();
  RemoveTool();
  ObjectSetAction(Tool,"Idle");
  Tool= nil;
  DoCon(-1);
}

public func CreateNewTool(idNewTool,pObject)
{
  if((GetY(Tool) ?? 0)-25 > (LocalN("RangeTop",Tool) ?? 0)) return(S42MigrationFirst(nil,Message("$FirstPullUp$",this(),GetName(Tool))));
  RemoveTool();
  var TurnedLeft= nil;
  if(Equal(GetID(), GRS2)) TurnedLeft= 1;
  Tool= CreateObject(idNewTool,DefinitionCall(idNewTool,"GetXPos",TurnedLeft),DefinitionCall(idNewTool,"GetYPos"),GetOwner());
  Tool-> Init(this(),Drehrad);
  ObjectSetAction(Hlp_3= CreateObject(GHLP,nil,nil,GetOwner()),"Attach",Tool);
  ObjectSetAction(Hlp_4b= CreateObject(PWRL,nil,nil,GetOwner()),"Connect",Hlp_1b,Hlp_3);
  if(pObject) LocalN("Tool",pObject)= Tool;
  return(1);
}

public func GetResearchBase() { return(GRS1); }
public func AddToGeruestKit() { return(1); }

/*-- Tooltime :) --*/

// Tool, das am Anfang erstellt wird
public func StartTool() { return(FHKN); }