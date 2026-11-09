/*-- Gerüst --*/

#strict

local Aussenwand,Verschluss,MQC;

protected func Initialize()
{
  MQC=1;
  SetAction("Exist");
  if(!FindGeruest(-25,20) && !FindGeruest(-70,-25) && !FindGeruest(20,-25) && !FindGeruest(-25,-70))
    {
    MQC=0;
    return(1);
    }
  var pClonk= GetCursor(GetOwner());
  CreateMenu(GetID(),pClonk,this(),0,"$Adjustment$",0,1);
  if(FindGeruest(-70,-25)) AddMenuItem("$Left$","Ausrichtung",LMM1,pClonk,0,FindGeruest(-70,-25),"");
  if(FindGeruest(20,-25))  AddMenuItem("$Right$","Ausrichtung",LMM5,pClonk,0,FindGeruest(20,-25),"");
  if(FindGeruest(-25,20))  AddMenuItem("$Down$","Ausrichtung",LMM7,pClonk,0,FindGeruest(-25,20),"");
  if(FindGeruest(-25,-70)) AddMenuItem("$Up","Ausrichtung",LMM3,pClonk,0,FindGeruest(-25,-70),"");
  AddMenuItem("$NoAdjustment$","Ausrichtung",MCMX,pClonk,0,pClonk,"");
}

private func Ausrichtung(idItem,obj)
{
  if(idItem == MCMX) CloseMenu(obj);
  if(idItem == LMM1) { SetPosition(GetX(obj) +38,GetY(obj)); Sound("Connect"); }
  if(idItem == LMM3) { SetPosition(GetX(obj),GetY(obj) +40); Sound("Connect"); }
  if(idItem == LMM5) { SetPosition(GetX(obj) -38,GetY(obj)); Sound("Connect"); }
  if(idItem == LMM7) { SetPosition(GetX(obj),GetY(obj) -40); Sound("Connect"); }
  MQC=0;
}

private func FindGeruest(iX,iY)
{
  var obj;
  while(obj= FindObject(0,iX,iY,50,50,0,0,0,0,obj))
   if(ObjectCall(obj,"IsGeruest"))
    return(obj);
}

protected func MenuQueryCancel() { return(MQC); }

protected func ControlUp(pClonk)
{
  [Menü]
  if(GetCon() < 100) return();
  if((GetOwner() != GetOwner(pClonk)) && (GetOwner()!= NO_OWNER())) return();
  var phase= GetPhase();
  CreateMenu(GetID(),pClonk);
  if(phase != 0) AddMenuItem("$Normal$","Change",GetID(),pClonk);
  if(phase != 1) AddMenuItem("$Fence$","Change",GetID(),pClonk,0,1,0,2,1);
  if(phase != 2) AddMenuItem("$ExternalWallLeft$","Change",GetID(),pClonk,0,2,0,2,2);
  if(phase != 3) AddMenuItem("$ExternalWallRight$","Change",GetID(),pClonk,0,3,0,2,3);
  if(phase != 4) AddMenuItem("$ExternalWalls$","Change",GetID(),pClonk,0,4,0,2,4);
  if(Verschluss) AddMenuItem("$OpenTrapdoor$","Open",DCKL,pClonk,0,0,0,2,1);
	       else AddMenuItem("$CloseTrapdoor$","Close",DCKL,pClonk);
  AddMenuItem("$TearOff$","Abreissen",CXCN,pClonk);
}

private func Change(idItem,iPhase)
{
  Sound("Build*");
  if(GetAction() ne "Exist") SetAction("Exist");
  SetPhase(iPhase);
  SetSolidMask(iPhase*50,40,50,40);
  if(Inside(iPhase,3,4))
    {
    if(!Aussenwand) Aussenwand= CreateObject(ASNW,+19,+20,GetOwner());
    }
  else RemoveAussenwand();
}

private func Open()
{
  Sound("Lever1");
  RemoveVerschluss();
}

private func Close()
{
  Sound("Lever1");
  Verschluss= CreateObject(DCKL,-4,-16,GetOwner());
}

private func Abreissen()
{
  RemoveAussenwand();
  RemoveVerschluss();
  CreateObject(CNKT,+5,+15,GetOwner());
  ScheduleCall(this(),"Abriss",1,GetCon() +5);
  // Zur Sicherheit...
  Schedule("RemoveObject()",105);
}

private func Abriss()
{
  if(!this()) return(0);
  DoCon(-1);
  if(!Random(25)) Sound("Build*");
}

protected func Destruction()	{ RemoveAussenwand(); RemoveVerschluss(); }
protected func Incineration()	{ RemoveAussenwand(); RemoveVerschluss(); }

private func RemoveAussenwand()
{
  if(Aussenwand) RemoveObject(Aussenwand);  
}

private func RemoveVerschluss()
{
  if(Verschluss) RemoveObject(Verschluss);  
}

public func IsGeruest() { return(1); }
public func AddToGeruestKit() { return(1); }