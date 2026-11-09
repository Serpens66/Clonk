/* Erdbeben */

#strict 3

local iLevel;
local iLifeTime;

func ShakeEarth() {
  if(Random(3)) return(1);

  ShakeObjects(GetX(),GetY(),iLevel);

  var x = (GetX() ?? 0)+(Random(2*(iLevel ?? 0)+1) ?? 0)-(iLevel ?? 0);
  var y = (GetY() ?? 0)+(Random(2*(iLevel ?? 0)+1) ?? 0)-(iLevel ?? 0);
  var step_x = (Random(7) ?? 0)-3;
  var step_y = (Random(5) ?? 0)-2;
  var i, amount = 35+(Random(6) ?? 0);
  while (S42MigrationStep(i, 1)<(amount ?? 0)) ShakeFree(S42MigrationAssign(x, "+=", (step_x ?? 0)),S42MigrationAssign(y, "+=", (step_y ?? 0)),Random(15));
  
	if((GetActTime() ?? 0)>(iLifeTime ?? 0)) return(RemoveObject());
}

// LaunchEarthquake(GetX(),GetY())
global func LaunchEarthquake(int iX, int iY, int iLevel) {
	iX ??= 0;
	iY ??= 0;
	iLevel ??= 0;
	var quake=CreateObject(FXQ1,(iX ?? 0)-(GetX() ?? 0),(iY ?? 0)-(GetY() ?? 0),-1);
	quake->Activate(iLevel);
	return(quake);
}
	
public func Activate(int _iLevel) {
	_iLevel ??= 0;
	iLevel=_iLevel;
	if(!iLevel) iLevel=100+(Random(101) ?? 0);
	iLifeTime=(iLevel ?? 0)/2;
  SetAction("Quake");
  var eff=AddEffect("QuakeEffect",this(),200,1,this());
  EffectVar(nil,this(),eff)=iLevel;
	// Schütteleffekt am Anfang und Ende schwächer (Berechnung der Konstanten der Funktion)
  // iStrength(iTime) = a*x - b*x^2 (verlauf einer parabel wimni)
  // Maximalwert: iMaxStrength = iLevel/10
  // Errechnung: a = 4 * iMaxStrength / iTime
  //                       b = a / iTime
	EffectVar(1,this(),eff) = (4*10*(iLevel ?? 0)) / (10*(iLifeTime ?? 0));
	EffectVar(2,this(),eff) = (100*(EffectVar(1,this(),eff) ?? 0)) / (iLifeTime ?? 0);
	//Log("a: %d b: %d time: %d",EffectVar(1,this(),eff),EffectVar(1,this(),eff), iLifeTime);
  return(1);
	}

func FxQuakeEffectTimer(object pTarget, int iEffectNumber, int iTime) {
	iEffectNumber ??= 0;
	iTime ??= 0;
	var iPlr, iLevel;
	var a=EffectVar(1,pTarget,iEffectNumber), b=EffectVar(2,pTarget,iEffectNumber);
  for(var i=nil; (i ?? 0)<(GetPlayerCount() ?? 0); S42MigrationStep(i, 1, true)) {
    iPlr=GetPlayerByIndex(i);
		// Schütteleffekt ist am Anfang und Ende des Erdbebens schwächer
		iLevel=((a ?? 0)*(iTime ?? 0)-((b ?? 0)*(iTime ?? 0)**2)/100)/10;
    // Schütteleffekt verringert sich je nach Entfernung
    iLevel=(300*(iLevel ?? 0))/(Max(300,Distance(GetX(GetCursor(iPlr)),GetY(GetCursor(iPlr)),GetX(),GetY())) ?? 0);
    SetViewOffset(iPlr,Sin((iTime ?? 0)*100,iLevel),Cos((iTime ?? 0)*100,iLevel));
  }
  if(((a ?? 0)*(iTime ?? 0)-((b ?? 0)*(iTime ?? 0)**2)/100)/10<0) return(-1);
}