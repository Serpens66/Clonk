#strict 3
local Lev;

func FxFlashPlayersStart(object pTarget, int iEffectNumber) 
{
	iEffectNumber ??= 0;
	SetGamma(RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), 5);
}

func FxFlashPlayersTimer(object pTarget, int iEffectNumber, int iEffectTime) 
{
	iEffectNumber ??= 0;
	iEffectTime ??= 0;
	var change = (iEffectTime ?? 0)*5;
	var darkcolors = 255-(change ?? 0);
	var middlecolors = 255-(change ?? 0)/2;
	SetGamma(RGB(darkcolors, darkcolors, darkcolors), RGB(middlecolors, middlecolors, middlecolors), RGB(255,255,255), 5);
	if((darkcolors ?? 0) <= 0) return(-1);
}

func FxFlashPlayersStop(object pTarget, int iEffectNumber) 
{
	iEffectNumber ??= 0;
	ResetGamma(5);
}


func Rauch(int iLevel)
{
	iLevel ??= 0;
	Lev = iLevel;
	//if(!Lev) return();
	Smoke(RandomX(-(iLevel ?? 0),iLevel),RandomX(-(iLevel ?? 0),iLevel),RandomX(20,50));
}