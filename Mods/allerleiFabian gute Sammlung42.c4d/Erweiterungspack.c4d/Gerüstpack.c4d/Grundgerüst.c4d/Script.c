/*-- Gerüst --*/

#strict 3

local Aussenwand,Verschluss,MQC;

protected func Initialize()
{
  MQC=1;
  SetAction("Exist");
  if(And(And(And(!FindGeruest(-25,20), !FindGeruest(-70,-25)), !FindGeruest(20,-25)), !FindGeruest(-25,-70)))
    {
    MQC=nil;
    return(1);
    }
  var pClonk= GetCursor(GetOwner());
  CreateMenu(GetID(),pClonk,this(),nil,"$Adjustment$",nil,1);
  if(FindGeruest(-70,-25)) AddMenuItem("$Left$","Ausrichtung",LMM1,pClonk,nil,FindGeruest(-70,-25),"");
  if(FindGeruest(20,-25))  AddMenuItem("$Right$","Ausrichtung",LMM5,pClonk,nil,FindGeruest(20,-25),"");
  if(FindGeruest(-25,20))  AddMenuItem("$Down$","Ausrichtung",LMM7,pClonk,nil,FindGeruest(-25,20),"");
  if(FindGeruest(-25,-70)) AddMenuItem("$Up","Ausrichtung",LMM3,pClonk,nil,FindGeruest(-25,-70),"");
  AddMenuItem("$NoAdjustment$","Ausrichtung",MCMX,pClonk,nil,pClonk,"");
}

private func Ausrichtung(idItem,obj)
{
  if(Equal(idItem, MCMX)) CloseMenu(obj);
  if(Equal(idItem, LMM1)) { SetPosition((GetX(obj) ?? 0) +38,GetY(obj)); Sound("Connect"); }
  if(Equal(idItem, LMM3)) { SetPosition(GetX(obj),(GetY(obj) ?? 0) +40); Sound("Connect"); }
  if(Equal(idItem, LMM5)) { SetPosition((GetX(obj) ?? 0) -38,GetY(obj)); Sound("Connect"); }
  if(Equal(idItem, LMM7)) { SetPosition(GetX(obj),(GetY(obj) ?? 0) -40); Sound("Connect"); }
  MQC=nil;
}

private func FindGeruest(iX,iY)
{
  var obj;
  while(obj= FindObject(nil,iX,iY,50,50,nil,nil,nil,nil,obj))
   if(ObjectCall(obj,"IsGeruest"))
    return(obj);
}

protected func MenuQueryCancel() { return(MQC); }

protected func ControlUp(pClonk)
{
  [Menü]
  if((GetCon() ?? 0) < 100) return;
  if(And((!Equal(GetOwner(), GetOwner(pClonk))), (!Equal(GetOwner(), NO_OWNER)))) return;
  var phase= GetPhase();
  CreateMenu(GetID(),pClonk);
  if(!Equal(phase, nil)) AddMenuItem("$Normal$","Change",GetID(),pClonk);
  if(!Equal(phase, 1)) AddMenuItem("$Fence$","Change",GetID(),pClonk,nil,1,nil,2,1);
  if(!Equal(phase, 2)) AddMenuItem("$ExternalWallLeft$","Change",GetID(),pClonk,nil,2,nil,2,2);
  if(!Equal(phase, 3)) AddMenuItem("$ExternalWallRight$","Change",GetID(),pClonk,nil,3,nil,2,3);
  if(!Equal(phase, 4)) AddMenuItem("$ExternalWalls$","Change",GetID(),pClonk,nil,4,nil,2,4);
  if(Verschluss) AddMenuItem("$OpenTrapdoor$","Open",DCKL,pClonk,nil,nil,nil,2,1);
	       else AddMenuItem("$CloseTrapdoor$","Close",DCKL,pClonk);
  AddMenuItem("$TearOff$","Abreissen",CXCN,pClonk);
}

private func Change(idItem,iPhase)
{
  Sound("Build*");
  if(!SEqual(GetAction(), "Exist")) SetAction("Exist");
  SetPhase(iPhase);
  SetSolidMask((iPhase ?? 0)*50,40,50,40);
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
  ScheduleCall(this(),"Abriss",1,(GetCon() ?? 0) +5);
  // Zur Sicherheit...
  Schedule("RemoveObject()",105);
}

private func Abriss()
{
  if(!this()) return(nil);
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