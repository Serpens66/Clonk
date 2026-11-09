/*-- Wasser-Sensor --*/

#strict 3
#include SENS

func Initialize()
{
  _inherited();
  AddState("nass");
}

func Timer()
{
	state = nil;
	if(GBackLiquid())
		SetBit(state,nil,1);
	else
		SetBit(state,1,1);
	TriggerActions();
}

public func GetAdditionalInfo()
{
	return(nil);//noch nix
}