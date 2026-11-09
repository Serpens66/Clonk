#strict 3

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
return(nil);
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
	ObjectSetAction(this(),"FlatUp", nil,nil, true);
}


public func Zated()
{
  if(!!SEqual(GetAction(), "Dead"))
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
