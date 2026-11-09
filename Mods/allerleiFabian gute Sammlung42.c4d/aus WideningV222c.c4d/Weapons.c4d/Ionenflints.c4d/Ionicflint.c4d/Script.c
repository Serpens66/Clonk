/*-- Ionenflint --*/

#strict 3
local counterr,ppLight;


public func Hit()
{
  Sound("BlasterCH",this());
  ScheduleCall(nil,"BBlaster",1,160);
  return(1);
}

public func IsChemicalProduct(){ return(1);}

func BBlaster()
{
  if(And(And(!FindObject(CBB_), ppLight), (counterr ?? 0)>150)) {RemoveObject(ppLight);ClearScheduleCall(nil,"BBlaster");Schedule("RemoveObject()",2);}
  
  var x,y;
  x=AbsX(GetX());
  y=AbsY(GetY());

  if(!counterr)
  {
  if(ppLight) RemoveObject(ppLight);
  ppLight=CreateObject(IOST,x,1,GetOwner());
  ppLight->ChangeColor(RGBa(120,nil,255,nil));
  ppLight->ChangeSizeXY(3000,32000);
  ppLight->ChangeR(180);
  ppLight->ChangeOffset(nil,-(GetY() ?? 0));
  ppLight->TurnOn();
  }
  S42MigrationStep(counterr, 1, true);
  
  var user = Contained();
  var angle = 180+(RandomX(-8,+8) ?? 0);
  var speed =250;

  // Effekte
  MuzzleFlash(30,nil,x,y,angle,RGBa(96,64,255,nil));

  if(Equal(counterr, 150))
  {  
  var ammo2 = CreateObject(CBB_,x,(y ?? 0)-1000,GetOwner());
  ammo2->Down();
  }
  if(ppLight)  {SetPosition(GetX(),GetY(),ppLight);  SetYDir(nil,ppLight);}
  SetYDir(nil,ppLight);
  if(ammo2)
  SetPosition(GetX(),GetY(ammo2),ammo2);

  return(1);
}