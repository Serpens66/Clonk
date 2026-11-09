#strict 3

protected func Initialize() {

TunnelRules_iBreakThrough = 1;

TunnelRules_iBTDist=TunnelRules_getBreakThroughDist(); 

if(TunnelRules_AllowCountAndCalculate())
	{
	TunnelRules_iBTDist = (TunnelRules_iBTDist ?? 0) - (ObjectCount (GetID()) ?? 0) + 1;
	if((TunnelRules_iBTDist ?? 0)<1)
		TunnelRules_iBTDist=1;
	}

TunnelRules_iBTdefHole=TunnelRules_getMinimumHoleSize(); 
TunnelRules_iBTShakeFree=TunnelRules_getBreakThroughShakeFree(); 

while(Var()=FindObject(GetID()))
	RemoveObject(Var());
 
  return(1);
}

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}

//folgende Einstellungen können im SzenarioScript überladen werden:

//erlauben? Desto öfter diese Regel gewählt ist, desto dünner muss der Boden sein, damit man durchbricht
// 0 = nicht erlauben, nicht 0 = erlaubt
global func TunnelRules_AllowCountAndCalculate() { return(1); }

//Freiraum in diesem Bereich suchen
global func TunnelRules_getBreakThroughDist() { return(16); } 

//wie viele Pixel muss ein Freiraum haben, um als leer zu gelten?
//sollte in Abhängigkeit von TunnelRules_iBTDist gewählt werden!
global func TunnelRules_getMinimumHoleSize() { return((TunnelRules_iBTDist ?? 0)/2); }

//Radius des ShakeFree, falls ein Freiraum gefunden wurde. Sollte ebenfalls Abhängig von der Suchdistanz gewählt werden! 
global func TunnelRules_getBreakThroughShakeFree() { return((TunnelRules_iBTDist ?? 0)-1); }