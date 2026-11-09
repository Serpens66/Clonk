/*-- DynChar --*/

#strict 2

//Flags für DynMessage
static const CHAR_FADEOUT = 1;

//Globale Funktion
global func DynMessage(string msg , object obj, int size, int color, int time, int flags)
	{
	//String hat länge 0
	if(!GetChar(msg,0))
			return 0;
	
	//Defaults setzen
	if(!color)
				color = RGBa(255,255,255,0);
				
	if(!size)
				size = 32;
				
	if(!time)
				time = 36+StringLength(msg)*12;
				

  //Zeichenkette erstellen
  var String;	
	String = CreateObject(CHAR,GetX(obj)-((StringLength(msg)*size)/4),GetY(obj)-(size/2)-10,-1)->Set(msg,0,time,size,color,flags,obj);
	
	
	//Zeiger auf Zeischenkette zurückgeben
	return String;
	}
	
global func StringLength(string str)
	{
	for(var i=0;i<1000;i++)
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
	SetClrModulation(color);
	SetCon((size*100)/32);
	Timer=time;
	
	if(target)
			{
		Target=target;
		XDistance=GetX()-GetX(Target);
		YDistance=GetY(Target)-GetY();
			}
	else
			{
			//Parallax setzen
			SetCategory(60817408);
			SetPosition(100+(char*size/2),100);
		
			}
	
	
	//Letztes Zeichen?
	var ascii;
	ascii = GetChar(msg,char);
	
	if(ascii == 0)
			return RemoveObject();
					
  //Aussehen Setzen
	var x,y;
	x=ascii;
	while(x >= 16)
			{
			x-=16;			
			y++;
			}
	SetAction(Format("%d",y+1));
	SetPhase(x);

	//Nachfolge Zeichen erstellen
	Next = CreateObject(CHAR,size/2,size/2,-1);
	
	//Und werte Festlegen
	Next->Set(msg,char+1,time,size,color,flags,target);
	
	}
	
public func Update()
	{

	}
	
public func FxTimeTimer()
	{
	Timer--;
	if(Timer<=0)RemoveObject();
	
	//Zum Ziel bewegen
	if(Target)
			SetPosition(GetX(Target)+XDistance,GetY(Target)-YDistance);
	}
