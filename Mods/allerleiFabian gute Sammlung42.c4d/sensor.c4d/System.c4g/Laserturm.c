/*-- Laserturm --*/

#strict 3
#appendto SLTW


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
		return("Abschlieﬂen");
	}
	if(Equal(a, 2))
	{
		return("Aufschlieﬂen");
	}
	if(Equal(a, 3))
	{
		return("Ausschalten");
	}
	if(Equal(a, 4))
	{
		return("Einschalten");
	}
  return(1);
}

public func Trigger(a,pSensor)
{
	if(Equal(a, 1))
	{
		return(zuu=1);
	}
	if(Equal(a, 2))
	{
		return(zuu=2);
	}
	if(Equal(a, 3))
	{
		return(ControlUpDouble(nil,2));
	}
	if(Equal(a, 4))
	{
		return(ControlUpDouble(nil,1));
	}
        return(1);	
}
