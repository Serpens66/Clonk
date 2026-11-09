#strict
local Lev;

func FxFlashPlayersStart(object pTarget, int iEffectNumber) 
{
	SetGamma(RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), 5);
}

func FxFlashPlayersTimer(object pTarget, int iEffectNumber, int iEffectTime) 
{
	var change = iEffectTime*5;
	var darkcolors = 255-change;
	var middlecolors = 255-change/2;
	SetGamma(RGB(darkcolors, darkcolors, darkcolors), RGB(middlecolors, middlecolors, middlecolors), RGB(255,255,255), 5);
	if(darkcolors <= 0) return(-1);
}

func FxFlashPlayersStop(object pTarget, int iEffectNumber) 
{
	ResetGamma(5);
}


func Rauch(int iLevel)
{
	Lev = iLevel;
	//if(!Lev) return();
	Smoke(RandomX(-iLevel,iLevel),RandomX(-iLevel,iLevel),RandomX(20,50));
}