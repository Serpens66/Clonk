/*-- Alter und neuer Wachturm --*/

#strict
#appendto TWRF
#appendto TWR2
#appendto BG5C  //Tor vom bunker


func Initialize(a,b,c,d,e,f,g)
{
//variable definiert in "eing‰nge sperren" appendto an alles
andere=1;
return(_inherited(a,b,c,d,e,f,g));
}

//Target

public func GetTriggerName(a)
{
	if(a==1)
	{
		return("Linkes Tor bedienen");
	}
	if(a==2)
	{
		return("Rechtes Tor bedienen");
	}
	if(a==3)
	{
		return("Abschlieﬂen");
	}
	if(a==4)
	{
		return("Aufschlieﬂen");
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
	if(a==3)
	{
		zuu=1;
	}
	if(a==4)
	{
		zuu=0;
	}
        return(1);	
}
