/*-- Wasser-Sensor --*/

#strict
#include SENS

func Initialize()
{
  _inherited();
  AddState("nass");
}

func Timer()
{
	state = 0;
	if(GBackLiquid())
		SetBit(state,0,1);
	else
		SetBit(state,1,1);
	TriggerActions();
}

public func GetAdditionalInfo()
{
	return(0);//noch nix
}