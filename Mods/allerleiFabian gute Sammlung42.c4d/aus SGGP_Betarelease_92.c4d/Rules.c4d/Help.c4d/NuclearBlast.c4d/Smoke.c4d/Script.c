#strict
local TimeToLive;
func Initialize()
{
	SetAction(Format("%d",RandomX(1,15)));
	TimeToLive = 0;
	SetRDir(RandomX(-10,10));
}

func Timer()
{
	TimeToLive++;
	SetYDir(RandomX(5,-5));
//	if(TimeToLive < 200) CastPXS("FlyAshes",1,5,RandomX(-20,20),RandomX(-20,20));
	SetXDir(RandomX(-5,5));
	SetClrModulation(RGBa(255,255,255,TimeToLive));
	var pCl;
	for(pCl in FindObjects(Find_Distance(GetCon()/2),Find_OCF(OCF_Alive)))
	{
		SetClrModulation(RGB(128,64,64),pCl);
		if(!GetEffect("Genuced",pCl)) AddEffect("Genuced",pCl, 3,40, pCl);
	}		
	if(TimeToLive > 254)
	{
		RemoveObject();
	}	
}

global func FxGenucedTimer(object pTarget)
{	
	pTarget -> DoDamage(1);
	pTarget -> DoEnergy(-1);
	if(InLiquid(pTarget)) 
	{
		SetClrModulation(0,pTarget);
		return(-1);
	}
	if(GetID(Contained(pTarget)) == GOSA) 
	{
		SetClrModulation(0,pTarget);
		return(-1);
	}
}
