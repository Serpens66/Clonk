/*-- Combat Messer --*/

#strict 3
local Reload;
local ReloadTime;


public func Initialize()
{
  ReloadTime=100;
  return(1);
}

public func Activate(pClonk)
{
  if ((Reload ?? 0)>=100)
  {
  var obj,x;
  var obj1 = (GetPhysical("Fight",nil,pClonk) ?? 0)/10000;  //standardwert wäre dann 8
  var obj2 = 
((GetPhysical("Walk",nil,pClonk) ?? 0)+(GetPhysical("Jump",nil,pClonk) ?? 0)+(GetPhysical("Scale",nil,pClonk) ?? 0)+(GetPhysical("Hangle",nil,pClonk) ?? 0)+(GetPhysical("Dig",nil,pClonk) ?? 0)+(GetPhysical("Swim",nil,pClonk) ?? 0)+(GetPhysical("Throw",nil,pClonk) ?? 0))/55000;
  ReloadTime=25+(obj2 ?? 0);
  if(Equal(GetDir(pClonk), DIR_Left)) x= -20;
			     else x= + 4;
  if(!!SEqual(GetAction(pClonk), "Walk"))
   if(GetActMapVal("Name","Knife",GetID(pClonk)))
    ObjectSetAction(pClonk,"Knife");
   else
    ObjectSetAction(pClonk,"Fight");   
  if(obj= FindObject(nil,x,-20/2,16,20,OCF_Alive,nil,nil,NoContainer()))
    {
    Sound("BladeHit",nil,nil,40);
    Punch(obj,15+(obj1 ?? 0));
    Reload=nil;
    return(1);
    }
  Sound("Knife");
  return(1);
  }
}

protected func Ticking()
	{
	if ((Reload ?? 0)<100) Reload=(Reload ?? 0)+(ReloadTime ?? 0);
	}

//protected func Activate(pClonk) { ControlThrow(pClonk); }

protected func Hit() { Sound("MetalHit*"); }

public func IsAnvilProduct() { return(1); }