/*-- Pumpe --*/

#strict
#appendto CTW0


func Initialize(a,b,c,d,e,f,g)
{
//variable definiert in "eing‰nge sperren" appendto an alles
andere=1;
return(_inherited(a,b,c,d,e,f,g));
}

public func GetTriggerName(a)
{
	if(a==1)
	{
		return("Nach links bewegen");
	}
	if(a==2)
	{
		return("Nach rechts bewegen");
	}
	if(a==3)
	{
		return("Anhalten");
	}
	if(a==4)
	{
		return("Schieﬂen");
	}
	if(a==5)
	{
		return("Abschlieﬂen");
	}
	if(a==6)
	{
		return("Aufschlieﬂen");
	}
  return(1);
}

public func Trigger(a,pSensor)
{
	if(a==1)
	{
		return(ContainedLeft(pSensor));
	}
	if(a==2)
	{
		return(ContainedRight(pSensor));
	}
	if(a==3)
	{
		return(ContainedUp(pSensor));
	}
	if(a==4)
	{
		return(ContainedDig(pSensor));
	}
	if(a==5)
	{
		zuu=1;
	}
	if(a==6)
	{
		zuu=0;
	}
        return(1);	
}
