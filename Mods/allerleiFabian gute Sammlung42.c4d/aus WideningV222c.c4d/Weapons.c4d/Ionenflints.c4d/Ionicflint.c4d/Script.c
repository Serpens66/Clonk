/*-- Ionenflint --*/

#strict
local counterr,ppLight;


public func Hit()
{
  Sound("BlasterCH",this());
  ScheduleCall(0,"BBlaster",1,160);  
  return(1);
}

public func IsChemicalProduct(){ return(1);}

func BBlaster()
{
  if(!FindObject(CBB_) && ppLight && counterr>150) {RemoveObject(ppLight);ClearScheduleCall(0,"BBlaster");Schedule("RemoveObject()",2);}
  
  var x,y;
  x=AbsX(GetX());
  y=AbsY(GetY());

  if(!counterr)
  {
  if(ppLight) RemoveObject(ppLight);
  ppLight=CreateObject(IOST,x,1,GetOwner());
  ppLight->ChangeColor(RGBa(120,0,255,0));
  ppLight->ChangeSizeXY(3000,32000);
  ppLight->ChangeR(180);
  ppLight->ChangeOffset(0,-GetY()); 
  ppLight->TurnOn();
  }
  counterr++;
  
  var user = Contained();
  var angle = 180+RandomX(-8,+8);
  var speed =250;

  // Effekte
  MuzzleFlash(30,0,x,y,angle,RGBa(96,64,255,0));

  if(counterr==150)
  {  
  var ammo2 = CreateObject(CBB_,x,y-1000,GetOwner());
  ammo2->Down();
  }
  if(ppLight)  {SetPosition(GetX(),GetY(),ppLight);  SetYDir(0,ppLight);}
  SetYDir(0,ppLight);
  if(ammo2)
  SetPosition(GetX(),GetY(ammo2),ammo2);

  return(1);
}