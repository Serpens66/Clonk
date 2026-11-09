/*-- Hammer --*/

#strict
local Reload;
local ReloadTime;


public func Initialize()
{
  ReloadTime=250;
  return(1);
}

public func Activate(pClonk)
{
  if (Reload>=250)
  {
  var obj,x;
  var obj1 = GetPhysical("Fight",0,pClonk)/10000;  //standardwert wäre dann 8
  var obj2 = 
(GetPhysical("Walk",0,pClonk)+GetPhysical("Jump",0,pClonk)+GetPhysical("Scale",0,pClonk)+GetPhysical("Hangle",0,pClonk)+GetPhysical("Dig",0,pClonk)+GetPhysical("Swim",0,pClonk)+GetPhysical("Throw",0,pClonk))/55000;


  ReloadTime=5+obj2+obj1;	
  if(GetDir(pClonk)== DIR_Left()) x= -20;
			     else x= + 4;
  if(GetAction(pClonk) eq "Walk")
   if(GetActMapVal("Name","Knife",GetID(pClonk)))
    ObjectSetAction(pClonk,"Knife");
   else
    ObjectSetAction(pClonk,"Fight");   
  if(obj= FindObject(0,x,-20/2,16,20,OCF_Alive(),0,0,NoContainer()))
    {
    Sound("BladeHit",0,0,40);
    Punch(obj,25+obj1*2);
    Reload=0;
    return(1);
    }
  Sound("Knife");
  return(1);
  }
}

protected func Ticking()
	{
	if (Reload<250) Reload=Reload+ReloadTime;
	}

//protected func Activate(pClonk) { ControlThrow(pClonk); }

protected func Hit() { Sound("MetalHit*"); }

public func IsAnvilProduct() { return(1); }