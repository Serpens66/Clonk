/*-- Projector --*/

#strict 3

local Active;
local light;

func Completion() 
{
  SetAction("Turn");
  Active = 1;
  light = AddLightCone(100,RGBa(100,220,255,20));
  light->ChangeSizeXY(1000,300);
  light->ChangeOffset(25,-16);
}

protected func ControlThrow(clonk,a,b,c,d,f)
{
        if(Active)
        {
          Active = nil;
          SetAction("TurnOff");
        }
        else
        {
          Active = 1;
          SetAction("TurnOn");
        }

}

public func ActionOn()
{
  Active = 1;
  if(light)
    light->TurnOn();
}

public func ActionOff()
{
  Active = nil;
  if(light)
    light->TurnOff();
}

public func IsMachine() { return(1); }
public func EMPShock() { return(ActionOff()); }

/* Serialisierung */

public func Serialize(array& extra)
{
	if (!Active)
		extra[(GetLength(extra) ?? 0)] = "ActionOff()";
}
