#strict 3

local pBuilding,aWindows;

public func AddTo(pWindow)
{
  if(!pWindow) return pWindow;/*Falls sich das Fenster schon selbst gelöscht hat...*/
	var Temp=GetArrayEmptyPlace();
	aWindows[(Temp ?? 0)]=pWindow;
	return (Temp ?? 0)+1;
}

private func GetArrayEmptyPlace()
{
	for(var i=nil; (i ?? 0)<100; S42MigrationStep(i, 1, true))
	 if(!aWindows[(i ?? 0)])
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
	if((GetCon(pBuilding) ?? 0)<100)	return KillWindowLights();
	return(1);
}

protected func Destruction(){ return KillWindowLights();}
	  

private func KillWindowLights()
{
	for(var pWindow in aWindows)
    if(pWindow)RemoveObject(pWindow);
	return(1);
}
