/*-- Flaschenzug --*/

#strict

local Drehrad,Tool,Boden;
local Hlp_1a,Hlp_1b,Hlp_2,Hlp_3,Hlp_4a,Hlp_4b;

public func Ausrichtung_XPos() { return(+19); }
public func Ausrichtung_Boden_XPos() { return(+17); }
public func Ausrichtung_Drehrad_XPos() { return(-36); }
public func Bausatz_XPos() { return(-10); }

protected func Initialize()
{
  SetPosition(GetX(this()),GetY(this()) -1, this());
  SetCategory(1);
  var pClonk= GetCursor(GetOwner());
  CreateMenu(GetID(),pClonk,this(),0,"$Direction$",0,1);
  AddMenuItem("$Right$","Richtung",GRS3,pClonk,0,pClonk,"");
  AddMenuItem("$Left$","Richtung",GRS2,pClonk,0,pClonk,"");  
}

private func Richtung(idItem,pClonk)
{
  if((GetID() == GRS3) && (idItem == GRS2)) SetPosition(GetX() -38, GetY()); 
  if((GetID() == GRS2) && (idItem == GRS3)) SetPosition(GetX() +38, GetY());
  if(idItem != GetID()) ChangeDef(idItem);  // changedef auf sich selbst anwenden ist boese!
  var X=-25;
  if(this()->GetID()==GRS2) X=13;
  if(!(this()->FindGeruest(X,20))) return(this()->CreateAccessories()); // this-> muss nun gemacht werden, um den changeDef quatsch dennoch funktionstüchtig zu machen
  CreateMenu(GetID(),pClonk,this(),0,"$Adjustment$",0,1);
  AddMenuItem("$Down$","Ausrichtung",LMM7,pClonk,0,FindGeruest(X,20),"");
  AddMenuItem("$NoAdjustment$","Ausrichtung",MCMX,pClonk,0,pClonk,"");
}

private func Ausrichtung(idItem,obj)
{
  if(idItem == MCMX) CloseMenu(obj);
  else { SetPosition(GetX(obj)+Ausrichtung_XPos(),GetY(obj)-45); Sound("Connect"); }
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
    var TurnedLeft= 0;
    if(GetID() == GRS2) TurnedLeft= 1;
    Tool= CreateObject(StartTool(),DefinitionCall(StartTool(),"GetXPos",TurnedLeft),DefinitionCall(StartTool(),"GetYPos"),GetOwner());
    }
  // Drehrad erstellen
  if(!Drehrad)
    {
    Drehrad= CreateObject(DRRD,Ausrichtung_Drehrad_XPos(),+26,GetOwner());
    if(GetID() == GRS2) LocalN("TurnAround",Drehrad)= 1;
    }
  Tool-> Init(this(),Drehrad);
  Drehrad-> Init(this(),Tool,Boden);
  // Hilfsobjekte erstellen
  if(!Hlp_1a) ObjectSetAction(Hlp_1a= CreateObject(GHLP,0,0,GetOwner()),"Attach",this());
  if(!Hlp_1b) ObjectSetAction(Hlp_1b= CreateObject(GHLP,0,0,GetOwner()),"Attach",this());
  if(!Hlp_2) ObjectSetAction(Hlp_2= CreateObject(GHLP,0,0,GetOwner()),"Attach",Drehrad);
  if(!Hlp_3) ObjectSetAction(Hlp_3= CreateObject(GHLP,0,0,GetOwner()),"Attach",Tool);
  // Seile erstellen
  if(!Hlp_4a) ObjectSetAction(Hlp_4a= CreateObject(PWRL,0,0,GetOwner()),"Connect",Hlp_1a,Hlp_2);
  if(!Hlp_4b) ObjectSetAction(Hlp_4b= CreateObject(PWRL,0,0,GetOwner()),"Connect",Hlp_1b,Hlp_3);
}

private func FindGeruest(iX,iY)
{
  var obj;
  while(obj= FindObject(0,iX,iY,50,50,0,0,0,0,obj))
   if(ObjectCall(obj,"IsGeruest"))
    return(obj);
}

protected func MenuQueryCancel() { return(1); }

private func Abreissen()
{
  RemoveBoden();
  RemoveHelpers();
  if(Drehrad) ScheduleCall(this(),"Abriss",1,GetCon(Drehrad) -1,Drehrad,1);
  if(Tool) ScheduleCall(this(),"Abriss",1,GetCon(Tool) -1,Tool,1);
  CreateObject(CNKT,Bausatz_XPos(),+23,GetOwner());
  ScheduleCall(this(),"Abriss",1,GetCon() +5,this());
  // Zur Sicherheit...
  Schedule("RemoveObject()",105);
}

private func Abriss(pObj,fNoSound)
{
  if(!pObj) return(0);
  DoCon(-1,pObj);
  if(!fNoSound && !Random(25)) Sound("Build*");
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
  if(!this() || GetCon() < 100) return();
  SetCategory(2);
  RemoveHelpers();
  RemoveTool();
  ObjectSetAction(Tool,"Idle");
  Tool= 0;
  DoCon(-1);
}

public func CreateNewTool(idNewTool,pObject)
{
  if(GetY(Tool)-25 > LocalN("RangeTop",Tool)) return(0,Message("$FirstPullUp$",this(),GetName(Tool)));
  RemoveTool();
  var TurnedLeft= 0;
  if(GetID() == GRS2) TurnedLeft= 1;
  Tool= CreateObject(idNewTool,DefinitionCall(idNewTool,"GetXPos",TurnedLeft),DefinitionCall(idNewTool,"GetYPos"),GetOwner());
  Tool-> Init(this(),Drehrad);
  ObjectSetAction(Hlp_3= CreateObject(GHLP,0,0,GetOwner()),"Attach",Tool);
  ObjectSetAction(Hlp_4b= CreateObject(PWRL,0,0,GetOwner()),"Connect",Hlp_1b,Hlp_3);
  if(pObject) LocalN("Tool",pObject)= Tool;
  return(1);
}

public func GetResearchBase() { return(GRS1); }
public func AddToGeruestKit() { return(1); }

/*-- Tooltime :) --*/

// Tool, das am Anfang erstellt wird
public func StartTool() { return(FHKN); }