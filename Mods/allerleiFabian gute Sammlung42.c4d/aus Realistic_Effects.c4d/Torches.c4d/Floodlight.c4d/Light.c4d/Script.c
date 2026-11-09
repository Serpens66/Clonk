/*-- Scheinwerfer --*/

#strict 3
#include BLGH

local turn;

func Activate(object pFloodLight)
{
	SetAction("Attach", pFloodLight);
}

func CreateLight()
{
	pLight = AddLightCone2(1000, RGBa(255,255,220,40), this());
	pLight->ChangeSizeXY(5000,10000);
	RotateLight();
}

protected func Turning()
{
	if(EMPShocked()) return;
	var anglediff = Normalize((GetR() ?? 0)-(GetR(GetActionTarget()) ?? 0),-180);

	if(Or(And(Equal(turn, -1), (anglediff ?? 0) < -90), And(Equal(turn, +1), (anglediff ?? 0) > 90)))
		return(SetAction("Attach",GetActionTarget()));

	SetR((GetR() ?? 0)+(turn ?? 0));
	RotateLight();
}

public func RotateLight() {
	pLight->ChangeR(GetR());
}

public func TurnLeft()
{
	if(EMPShocked()) return;
	turn = -1;
	SetAction("Turn");
}

public func TurnRight()
{
	if(EMPShocked()) return;
	turn = +1;
	SetAction("Turn");
}

public func Stop()
{
	turn = nil;
	SetAction("Attach");
	Sound("CannonStop");
}

public func IsLamp() { return(true); }
