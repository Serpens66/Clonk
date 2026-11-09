/*-- DynChar --*/

#strict 3

//Flags für DynMessage
static const CHAR_FADEOUT = 1;

//Globale Funktion
global func DynMessage(string msg , object obj, int size, int color, int time, int flags)
	{
	//String hat länge 0
	size ??= 0;
	color ??= 0;
	time ??= 0;
	flags ??= 0;
	if(!GetChar(msg,nil))
			return nil;
	
	//Defaults setzen
	if(!color)
				color = RGBa(255,255,255,nil);
				
	if(!size)
				size = 32;
				
	if(!time)
				time = 36+(StringLength(msg) ?? 0)*12;
				

  //Zeichenkette erstellen
  var String;	
	String = CreateObject(CHAR,(GetX(obj) ?? 0)-(((StringLength(msg) ?? 0)*(size ?? 0))/4),(GetY(obj) ?? 0)-((size ?? 0)/2)-10,-1)->Set(msg,nil,time,size,color,flags,obj);
	
	
	//Zeiger auf Zeischenkette zurückgeben
	return String;
	}
	
global func StringLength(string str)
	{
	for(var i=nil;(i ?? 0)<1000;S42MigrationStep(i, 1, true))
		if(!GetChar(str,i))
				return i;	
	}
	
//Locale Variablen
local Next;
local Timer;

//Für die Positionierung
local Target;
local XDistance,YDistance;

	
func Initialize()
	{
	AddEffect("Time",this,3,1,this);
	}


public func Set(string msg, int char, int time, int size, int color, int flags, object target)
	{
	//Werte Setzen
	char ??= 0;
	time ??= 0;
	size ??= 0;
	color ??= 0;
	flags ??= 0;
	SetClrModulation(color);
	SetCon(((size ?? 0)*100)/32);
	Timer=time;
	
	if(target)
			{
		Target=target;
		XDistance=(GetX() ?? 0)-(GetX(Target) ?? 0);
		YDistance=(GetY(Target) ?? 0)-(GetY() ?? 0);
			}
	else
			{
			//Parallax setzen
			SetCategory(60817408);
			SetPosition(100+((char ?? 0)*(size ?? 0)/2),100);
		
			}
	
	
	//Letztes Zeichen?
	var ascii;
	ascii = GetChar(msg,char);
	
	if(S42MigrationEqual(ascii, nil))
			return RemoveObject();
					
  //Aussehen Setzen
	var x,y;
	x=ascii;
	while((x ?? 0) >= 16)
			{
			S42MigrationAssign(x, "-=", 16);
			S42MigrationStep(y, 1, true);
			}
	SetAction(Format("%d",(y ?? 0)+1));
	SetPhase(x);

	//Nachfolge Zeichen erstellen
	Next = CreateObject(CHAR,(size ?? 0)/2,(size ?? 0)/2,-1);
	
	//Und werte Festlegen
	Next->Set(msg,(char ?? 0)+1,time,size,color,flags,target);
	
	}
	
public func Update()
	{

	}
	
public func FxTimeTimer()
	{
	S42MigrationStep(Timer, -1, true);
	if((Timer ?? 0)<=0)RemoveObject();
	
	//Zum Ziel bewegen
	if(Target)
			SetPosition((GetX(Target) ?? 0)+(XDistance ?? 0),(GetY(Target) ?? 0)-(YDistance ?? 0));
	}
