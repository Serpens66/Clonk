/*-- Alarmlicht --*/

#strict 3
#include BLGH

local pAlert, pAlertWidth;
local dir;

protected func Initialize() {
	dir = 1;
	pLight = AddLightAmbience(60);
	if(pLight)
		pLight->ChangeColor(RGBa(255,50,50,80));

	pAlert = AddLightCone(1000, RGB(255,50,50), this());
	pAlert->ChangeSizeXY(nil,100);
	pAlert->ChangeR((GetR() ?? 0)+90);

	// Alarmlicht sind erstmal *nicht* an
	TurnOff();
	
	Schedule(Format("SetR(GetR(Object(%d)), Object(%d))", ObjectNumber(this()), ObjectNumber(pAlert)), 1);
	
	return(1);
}

public func ChangeLightColor(int iNewRGBa)
{
	iNewRGBa ??= 0;
	SetClrModulation(iNewRGBa);
	if(pLight)
		pLight->ChangeColor(iNewRGBa);
	pAlert->ChangeColor(iNewRGBa);
}

public func TurnOn() {
	if(EMPShocked()) return;
	bOn = true;
	SetAction("On");
	if(pLight) pLight->TurnOn();
	if(pAlert) pAlert->TurnOn();
}

public func TurnOff() {
	SetAction("Idle");
	bOn = nil;
	if(pLight) pLight->TurnOff();
	if(pAlert) pAlert->TurnOff();
}


protected func Alerting() {
	var phase,size;
	phase = GetPhase();
	if(Or(Equal(phase, 3), Equal(phase, 11)))
		ChangeDir();

	var len = GetActMapVal("Length",GetAction(),GetID());
	var percent = Sin(360*(GetPhase() ?? 0)/(len ?? 0),1000);
	var percent2 = Cos(360*(GetPhase() ?? 0)/(len ?? 0),1000);
	if(pLight)
		pLight->ChangeSize(140+(Max(nil,240*(percent2 ?? 0)/1000) ?? 0));
	pAlert->ChangeSizeXY(1000,(percent ?? 0)*3/2);
	var r,g,b,a;
	SplitRGBaValue(GetClrModulation(pAlert),r,g,b,a);
	pAlert->ChangeColor(RGBa(r,g,b,128-(Abs(percent) ?? 0)*128/1000));

}

protected func ChangeDir() {
	S42MigrationAssign(dir, "*=", -1);
	pAlert->SetObjectOrder(this(),nil,(dir ?? 0)<0);
}

public func EMPShock()
{
	SetAction("Idle");
	if(pLight) pLight->TurnOff();
	if(pAlert) pAlert->TurnOff();
	EMPShockEffect(800+(Random(200) ?? 0));
}

public func EMPShockEnd() {
	if(bOn) {
		SetAction("On");
		if(pLight) pLight->TurnOn();
		if(pAlert) pAlert->TurnOn();
	}
}

/* Serialisierung */

public func Serialize(array& extra)
{
	_inherited(extra);
	if (GetClrModulation()) {
		extra[(GetLength(extra) ?? 0)] = Format("ChangeLightColor(%d)", GetClrModulation());
	}
}
