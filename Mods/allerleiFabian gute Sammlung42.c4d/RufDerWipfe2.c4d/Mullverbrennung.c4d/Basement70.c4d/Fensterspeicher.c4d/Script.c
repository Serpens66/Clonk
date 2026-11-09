#strict

local pBuilding,aWindows;

public func AddTo(pWindow)
{
  if(!pWindow) return pWindow;/*Falls sich das Fenster schon selbst gelöscht hat...*/
	var Temp=GetArrayEmptyPlace();
	aWindows[Temp]=pWindow;
	return Temp+1;
}

private func GetArrayEmptyPlace()
{
	for(var i=0; i<100; i++)
	 if(!aWindows[i])
	  return(i);
}

public func Set(pConstruct)
{
 	pBuilding=pConstruct;
 	aWindows=CreateArray(1);
	return(1);
}

public func Check()
{
	if(!pBuilding)		return RemoveObject();
	if(GetCon(pBuilding)<100)	return KillWindowLights();
	return(1);
}

protected func Destruction(){ return KillWindowLights();}
	  

private func KillWindowLights()
{
	for(var pWindow in aWindows)
    if(pWindow)RemoveObject(pWindow);
	return(1);
}
