/*-- Luke --*/

#strict
#appendto HTCH
//Target


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
		return("÷ffnen");
	}
	if(a==2)
	{
		return("Schlieﬂen");
	}
  return(1);
}

public func Trigger(a)
{
	if(a==1)
	{
		if(state == 0)
			ControlUp();
	}
	if(a==2)
	{
		if(state == 1)
			ControlDownSingle();
	}
        return(1);	
}
