/*-- Pumpe --*/

#strict 3
#appendto CTW0


func Initialize(a,b,c,d,e,f,g)
{
//variable definiert in "eing‰nge sperren" appendto an alles
andere=1;
return(_inherited(a,b,c,d,e,f,g));
}

public func GetTriggerName(a)
{
	if(Equal(a, 1))
	{
		return("Nach links bewegen");
	}
	if(Equal(a, 2))
	{
		return("Nach rechts bewegen");
	}
	if(Equal(a, 3))
	{
		return("Anhalten");
	}
	if(Equal(a, 4))
	{
		return("Schieﬂen");
	}
	if(Equal(a, 5))
	{
		return("Abschlieﬂen");
	}
	if(Equal(a, 6))
	{
		return("Aufschlieﬂen");
	}
  return(1);
}

public func Trigger(a,pSensor)
{
	if(Equal(a, 1))
	{
		return(ContainedLeft(pSensor));
	}
	if(Equal(a, 2))
	{
		return(ContainedRight(pSensor));
	}
	if(Equal(a, 3))
	{
		return(ContainedUp(pSensor));
	}
	if(Equal(a, 4))
	{
		return(ContainedDig(pSensor));
	}
	if(Equal(a, 5))
	{
		zuu=1;
	}
	if(Equal(a, 6))
	{
		zuu=nil;
	}
        return(1);	
}
