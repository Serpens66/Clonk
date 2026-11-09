/*-- Haubitze --*/

#strict
#appendto CANN

func Initialize(a,b,c,d,e,f,g)
{
//variable definiert in "eingänge sperren" appendto an alles
andere=1;
return(_inherited(a,b,c,d,e,f,g));
}

public func GetTriggerName(a)
{
	if(a==1)
	{
		return("Hoch bewegen");
	}
	if(a==2)
	{
		return("Runter bewegen");
	}
	if(a==3)
	{
		return("Links");
	}
	if(a==4)
	{
		return("Rechts");
	}
	if(a==5)
	{
		return("Schießen");
	}
	if(a==6)
	{
		return("autom. zielen und schießen");
	}
  return(1);
}

public func Trigger(a,pSensor,pSpecial)
{
	if(a==1)
	{
		return(AimUp(pSensor, 1, "ControlConf"));
	}
	if(a==2)
	{
		return(AimDown(pSensor, 1, "ControlConf"));
	}
	if(a==3)
	{
		return(SetDir (DIR_Left));
	}
	if(a==4)
	{
		return(SetDir (DIR_Right));
	}
	if(a==5)
	{
		return(Fire());
	}
	if(a==6)
	{
          if(pSpecial)
            if(Distance(GetX(),GetY(),GetX(pSpecial),GetY(pSpecial))>50)
              return(FireAt(GetX(pSpecial),GetY(pSpecial)));
	}
        return(1);	
}
