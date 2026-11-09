#strict 3

#appendto CLNK //hier weitere Definitionen nach dem Muster #appdento ID einfügen


static TunnelRules_iBreakOpen;
static TunnelRules_iBreakThrough;

static TunnelRules_iBTDist;
static TunnelRules_iBTdefHole; 
static TunnelRules_iBTShakeFree;
static TunnelRules_iBOShakeFree; 
static TunnelRules_iBOSearchSize; 

local TunnelRules_iEffect;
local TunnelRules_iBOLock;

protected func ControlDigSingle() {
	if(_inherited())
		return(1);

if(!(Or(TunnelRules_iBreakOpen, TunnelRules_iBreakThrough))) //keine Regel gesetzt
	return(nil);

if(TunnelRules_iEffect) //wurde der alte Effekt noch nicht entfernt?
	RemoveEffect("TunnelRules",this()); //weg damit

TunnelRules_iBOLock = 1;

var iEffectTimer;

if(TunnelRules_iBreakOpen)
	{
		iEffectTimer = 1; //hier Timer=1 benötigt
	}else{
		iEffectTimer = 60; //ansonsten eilt es nicht, nur zum Entfernen gebraucht
	}

TunnelRules_iEffect = AddEffect ("TunnelRules", this(), 1, iEffectTimer, this(), nil);
return(nil); //freigeben zum Graben
}

private func Digging()
{

// beim Brueckenbau nichts machen
if(!!SEqual(GetAction(this()), "Bridge"))
return(_inherited());

	if(TunnelRules_iBreakOpen)
		{
			if(TunnelRules_iBOLock)	
				S42MigrationStep(TunnelRules_iBOLock, -1, true);
			EffectVar (nil, this(), TunnelRules_iEffect)=GetXDir();
			EffectVar (1, this(), TunnelRules_iEffect)=GetYDir(); 
		}


	if(TunnelRules_iBreakThrough) //hier reicht ein Aufruf alle 15 Frames 
		TunnelRules_checkBreakThrough(); 
		
return(_inherited());	
}

//Aufbrechen etc

private func TunnelRules_checkBreakThrough()
{

	var iSearchStartY=(GetObjHeight(this()) ?? 0)/2+1;
	var iCount;

	for(var iToleranz=-1;(iToleranz ?? 0)<2;S42MigrationStep(iToleranz, 1, true))
	{	
	
	iCount=nil;
	for(Var()=nil;(Var() ?? 0)<(TunnelRules_iBTDist ?? 0);S42MigrationStep(Var(), 1, true))
	{
		if(!GBackSolid(iToleranz,(iSearchStartY ?? 0)+(Var() ?? 0)))
			{
			
			S42MigrationStep(iCount, 1, true);
			if((iCount ?? 0)>(TunnelRules_iBTdefHole ?? 0)) //genug Luft um als Hoehle zu gelten
				{
				ShakeFree(GetX(),(GetY() ?? 0)+(iSearchStartY ?? 0)/2,TunnelRules_iBTShakeFree);
				return(1);			
				}
			}
		
	}	
	}

}



private func TunnelRules_doBreakOpen(xs,ys){

//Gänge nach oben und gerade Gänge interessieren nicht
	if((!ys ?? 0)>0)
		return(nil);


	if((xs ?? 0)<0) //graben nach links
		Var()=-1;


	if(Equal(xs, nil)) //graben nach unten
		Var()=nil;

	if((xs ?? 0)>0) //graben nach rechts
		Var()=1;

var iSearchStartY=(GetObjHeight(this()) ?? 0)/2+1;
var iSearchXSize=(GetObjWidth(this()) ?? 0)/4;

for(var x=-(iSearchXSize ?? 0);(x ?? 0)<(iSearchXSize ?? 0);S42MigrationStep(x, 1, true))
	for(var y=iSearchStartY;(y ?? 0)<(iSearchStartY ?? 0)+(TunnelRules_iBOSearchSize ?? 0);S42MigrationStep(y, 1, true))
 		if(!GBackSolid(x,y)) //Treffer?
		{
		ShakeFree ((GetX() ?? 0)+((iSearchStartY ?? 0)/2)*(Var() ?? 0),(GetY() ?? 0)+(iSearchStartY ?? 0)/2,TunnelRules_iBOShakeFree);
		return(1);
		}
return(nil);
}

//Effekt


protected func FxTunnelRulesTimer(pTarget, iEffectNumber)
{

if (!SEqual(GetAction(pTarget), "Dig"))
	{
		if(!TunnelRules_iBreakOpen)
			return(TunnelRules_RemoveEffect());

		if(!TunnelRules_iBOLock)
			TunnelRules_doBreakOpen(EffectVar (nil, this(), TunnelRules_iEffect),EffectVar (1, this(), TunnelRules_iEffect));
		return(TunnelRules_RemoveEffect());
	}
}

private func TunnelRules_RemoveEffect()
{
	TunnelRules_iEffect=nil;
	return(-1);
}


