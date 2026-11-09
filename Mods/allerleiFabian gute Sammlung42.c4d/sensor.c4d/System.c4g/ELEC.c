/*-- Fahrstuhlkorb --*/

#strict
#appendto ELEC
#appendto ELE_

func Initialize(a,b,c,d,e,f,g)
{
//variable definiert in "eingänge sperren" appendto an alles
andere=1;
return(_inherited(a,b,c,d,e,f,g));
}

//protected func Activity()
//{
//  UpdateTransferZone();
//  // Legacy
//  if (Local(8) && !RangeTop) ImportSettings();
//  // Fahrstuhl zerstört
//  // if (!Elevator() || OnFire(Elevator())) { SetAction("Idle"); Incinerate(); return(); }
//
//  /*
//  // Zu wartenden Clonks fahren
//  if (AutoMode == 0 && !Passenger() && !GetCommand())
//    MoveToWaitingClonk();
//  */
//
//  // Sicherheitscheck: Bewegung ohne Kommando nur bei Schachtbohrung
//  if (GetComDir())
//    if (!GetCommand() && GetAction() ne "Drill")
//      Halt();
//  // Fertig
//  return(1);
//}

public func GetTriggerName(a)
{
	if(a==1)
	{
		return("Hoch");
	}
	if(a==2)
	{
		return("Runter");
	}
	if(a==3)
	{
		return("Stop");
	}
	if(a==4)
	{
		return("GoToSensor");
	}
	if(a==5)
	{
		return("GoToSpecial");
	}
  return(1);
}

public func Trigger(a,pSensor,anySpecial)
{
	if(a==1)
	{
		return(DoControlUp(pSensor));
	}
	if(a==2)
	{
		return(DoControlDown(pSensor));
	}
	if(a==3)
	{
		return(DoControlStop(pSensor));
	}
	if(a==4)
	{
		return(SetMoveTo(GetY(pSensor)));
	}
	if(a==5)
	{
		if(anySpecial)
		  if(GetType(anySpecial) == C4V_C4Object)
		    return(SetMoveTo(GetY(pSensor)));
	}
        return(1);	
}