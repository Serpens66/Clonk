/*-- Laserturm --*/

#strict
#appendto SLTW


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
		return("Abschlieﬂen");
	}
	if(a==2)
	{
		return("Aufschlieﬂen");
	}
	if(a==3)
	{
		return("Ausschalten");
	}
	if(a==4)
	{
		return("Einschalten");
	}
  return(1);
}

public func Trigger(a,pSensor)
{
	if(a==1)
	{
		return(zuu=1);
	}
	if(a==2)
	{
		return(zuu=2);
	}
	if(a==3)
	{
		return(ControlUpDouble(0,2));
	}
	if(a==4)
	{
		return(ControlUpDouble(0,1));
	}
        return(1);	
}
