/*-- Hangar vom Bunker --*/

#strict

#appendto SI0D //Schalter des Hangars vom Bunker


func Initialize(a,b,c,d,e,f,g)
{
//variable definiert in "eingänge sperren" appendto an alles
andere=1;
return(_inherited(a,b,c,d,e,f,g));
}

//Target

public func GetTriggerName(a)
{
	if(a==1)
	{
		return("Hangartor öffnen");
	}
	if(a==2)
	{
		return("Hangartor schließen");
	}
  return(1);
}

public func Trigger(a)
{
	if(a==1)
	{
			ControlLeft();
	}
	if(a==2)
	{
			ControlRight();
	}
        return(1);	
}
