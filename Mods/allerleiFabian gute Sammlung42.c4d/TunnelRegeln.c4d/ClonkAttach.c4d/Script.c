#strict

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

if(!(TunnelRules_iBreakOpen||TunnelRules_iBreakThrough)) //keine Regel gesetzt
	return(0);

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

TunnelRules_iEffect = AddEffect ("TunnelRules", this(), 1, iEffectTimer, this(), 0);
return(0); //freigeben zum Graben
}

private func Digging()
{

// beim Brueckenbau nichts machen
if(GetAction(this()) eq "Bridge")
return(_inherited());

	if(TunnelRules_iBreakOpen)
		{
			if(TunnelRules_iBOLock)	
				TunnelRules_iBOLock--;
			EffectVar (0, this(), TunnelRules_iEffect)=GetXDir(); 
			EffectVar (1, this(), TunnelRules_iEffect)=GetYDir(); 
		}


	if(TunnelRules_iBreakThrough) //hier reicht ein Aufruf alle 15 Frames 
		TunnelRules_checkBreakThrough(); 
		
return(_inherited());	
}

//Aufbrechen etc

private func TunnelRules_checkBreakThrough()
{

	var iSearchStartY=GetObjHeight(this())/2+1;
	var iCount;

	for(var iToleranz=-1;iToleranz<2;iToleranz++)
	{	
	
	iCount=0;
	for(Var()=0;Var()<TunnelRules_iBTDist;Var()++)
	{
		if(!GBackSolid(iToleranz,iSearchStartY+Var())) 
			{
			
			iCount++;
			if(iCount>TunnelRules_iBTdefHole) //genug Luft um als Hoehle zu gelten			
				{
				ShakeFree(GetX(),GetY()+iSearchStartY/2,TunnelRules_iBTShakeFree);
				return(1);			
				}
			}
		
	}	
	}

}



private func TunnelRules_doBreakOpen(xs,ys){

//Gänge nach oben und gerade Gänge interessieren nicht
	if(!ys>0) 
		return(0);


	if(xs<0) //graben nach links
		Var()=-1;


	if(xs==0) //graben nach unten
		Var()=0;

	if(xs>0) //graben nach rechts
		Var()=1;

var iSearchStartY=GetObjHeight(this())/2+1;
var iSearchXSize=GetObjWidth(this())/4;

for(var x=-iSearchXSize;x<iSearchXSize;x++)
	for(var y=iSearchStartY;y<iSearchStartY+TunnelRules_iBOSearchSize;y++)
 		if(!GBackSolid(x,y)) //Treffer?
		{
		ShakeFree (GetX()+(iSearchStartY/2)*Var(),GetY()+iSearchStartY/2,TunnelRules_iBOShakeFree);
		return(1);
		}
return(0);
}

//Effekt


protected func FxTunnelRulesTimer(pTarget, iEffectNumber)
{

if (GetAction(pTarget) ne "Dig")
	{
		if(!TunnelRules_iBreakOpen)
			return(TunnelRules_RemoveEffect());

		if(!TunnelRules_iBOLock)
			TunnelRules_doBreakOpen(EffectVar (0, this(), TunnelRules_iEffect),EffectVar (1, this(), TunnelRules_iEffect));
		return(TunnelRules_RemoveEffect());
	}
}

private func TunnelRules_RemoveEffect()
{
	TunnelRules_iEffect=0;
	return(-1);
}


