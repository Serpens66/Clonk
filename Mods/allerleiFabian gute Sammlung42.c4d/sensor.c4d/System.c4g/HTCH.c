/*-- Luke --*/

#strict 3
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
	if(Equal(a, 1))
	{
		return("÷ffnen");
	}
	if(Equal(a, 2))
	{
		return("Schlieﬂen");
	}
  return(1);
}

public func Trigger(a)
{
	if(Equal(a, 1))
	{
		if(Equal(state, nil))
			ControlUp();
	}
	if(Equal(a, 2))
	{
		if(Equal(state, 1))
			ControlDownSingle();
	}
        return(1);	
}
