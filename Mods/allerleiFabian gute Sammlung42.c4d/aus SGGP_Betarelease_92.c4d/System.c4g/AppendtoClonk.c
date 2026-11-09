#strict

// #appendto CLNK // mal deaktivieren

func ContextRelease(pCaller) 
{
	[Hilfsregel|Image=HELP|Condition=findrule]
  	HelpContext(GetController());
  	return(1);
}

func findrule()
{
if(FindObject(HELP)) return(1);
return(0);
}

func IsBulletTarget()
{
	return(true);
}

func IsThreat()
{
	return(true);
}

func ReRise()
{
	ObjectSetAction(this(),"FlatUp", 0,0, true); 
}


public func Zated()
{
  if(GetAction() eq "Dead")
  {
//   SetAction("Dead",this());
   EMPShockEffect(50);
   return(1);
  }
  SetAction("Dead");
  Schedule("ReRise()",300);
  EMPShockEffect(50);
  return(1);
}
