/*-- Gangverschluss --*/

#strict 3
#appendto G15Y
#appendto G25Y




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
		return("LEFT");
	}
	if(Equal(a, 2))
	{
		return("RIGHT");
	}
  return(1);
}

public func Trigger(a)
{
	if(Equal(a, 1))
	{
			ControlLeft();
	}
	if(Equal(a, 2))
	{
			ControlRight();
	}
        return(1);	
}
