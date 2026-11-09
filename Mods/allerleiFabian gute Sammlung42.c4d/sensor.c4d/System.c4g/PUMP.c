/*-- Pumpe --*/

#strict 3
#appendto PUMP

func Initialize(a,b,c,d,e,f,g)
{
//variable definiert in "eingänge sperren" appendto an alles
andere=1;
return(_inherited(a,b,c,d,e,f,g));
}


public func GetTriggerName(a)
{
	if(Equal(a, 1))
	{
		return("Pumpen");
	}
	if(Equal(a, 2))
	{
		return("Aufhören zu Pumpen");
	}
  return(1);
}

public func Trigger(a,pSensor)
{
	if(Equal(a, 1))
	{
		return(SetAction("Pump"));
	}
	if(Equal(a, 2))
	{
		return(SetAction("Wait"));
	}
        return(1);	
}
