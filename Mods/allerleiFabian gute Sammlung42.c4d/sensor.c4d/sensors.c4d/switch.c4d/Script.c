/*-- Schalter --*/

#strict 3
#include SENS

// Zielobjekte
local last_user;

/* Steuerung */

func Initialize()
{
  _inherited();
  AddState("$ACTIVE$");
}

/*
public func ActivateEntrance(pObj)
{
  FinishCommand (pObj,true,0); 
  ControlThrow(pObj);
  return(1);
}
*/


public func ControlThrow(pClonk)
{
	last_user = pClonk;
	if(Equal(GetState(state,nil), 1))
	{
		SetGraphics ("OFF");
		SetState(state,nil,2);//Aus
	}
	else
	{
		SetGraphics ();
		SetState(state,nil,1);//An
	}
	Sound("Grab");
	TriggerActions();
}

public func GetAdditionalInfo()
{
	return(last_user);
}