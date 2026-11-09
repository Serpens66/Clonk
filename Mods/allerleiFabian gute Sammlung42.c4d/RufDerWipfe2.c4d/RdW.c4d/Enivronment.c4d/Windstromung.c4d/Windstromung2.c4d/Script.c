#strict
local maxindex;		

/* Aufruf von Aussen */
public func SpeicherOst() 
{
	SpeicherObjekt(0);
}

public func SpeicherWest()
{
	SpeicherObjekt(1);
}

/* Speichert alle Winddaten einer Richtung in diesem Objekt */
private func SpeicherObjekt(bwest)
{
	maxindex=0;
	var thisindex;
	/* Alle anderen Objekte "aufsaugen" */
	var windobj;
	while(windobj=FindObject(WIND,0,0,0,0,0,0,0,0,windobj))
	{
		thisindex =Index(GetX(windobj),GetY(windobj));
		if(thisindex>maxindex) maxindex=thisindex;
		Local(thisindex)=windobj->WIND::GetStrength(bwest);
	}
	return(1);
}

/* Gibt die absolute Windstärke an der Position x/y an. 
   Dazu wird der Normalwind (staerke) mit der lokalen, relativen Stärke verrechnet */
public func GibLokalWind(staerke,x,y)
{
	return(Prozent(staerke,GetStrength(x,y)));
}

/* Gibt die prozentuale Stärke vom Normalwind an */
public func GetStrength(x,y)
{
	var index=Index(x,y);
	if(index>maxindex) return(5); // da es dieses Windobjekt nicht gibt (entfernte sich wegen NixHimmel) -> 5%;
	return(Max(5,Local(index)));
}

/* Errechnet den Index des Windspeicherobjektes für x/y */
private func Index(x,y)
{
	var index =x/WINDAUFLOESUNG; // X-Koordinate
	    index+=y/WINDAUFLOESUNG * LandscapeWidth()/WINDAUFLOESUNG; // Y-Koordinate
	return(index);
}

/* Speichert den Index des letzten Windspeicherobjektes */
public func SetMaxIndex(index)
{
	maxindex = index;
	return(1);
}

global func LogWind()
{
  Log("/*");
  Log("--Windströmungsspeicherung");
  Log("--automatisch per Skript erstellt");
  Log("--");
  Log("--void LogWind():");
  Log("--Logt die Windstärken so das sie später wieder im Skript benutzt werden können");
  Log("*/");
  Log("");
  Log("global func DoWhatIWHant(){ return %d;}",horizont);
  Log("");
  globalostwinde->MyLogWind("E");
  Log("");
  globalwestwinde->MyLogWind("W");
}

global func VisualWind(alpha,fReal)
{
  var x,y,str,str2;
  for(x=0; x<LandscapeWidth(); x+=50)
    for(y=0; y<LandscapeHeight(); y+=50)
    {
      if(!fReal)
      {
        str=globalwestwinde->~GetStrength(x,y);
        str2=globalostwinde->~GetStrength(x,y);
      }
      else
      {
        // Uffz... das hat mir ne zeit lang Kopfzerbrechen bereitet (und das nur weil GetWind innerhalb von 1nem Frame immer das gleiche zurückgibt)
        //SetWind(100);
        str=GetWind(x,y);
        //SetWind(-100);
        //str2=GetWind(x,y);
      }
      if(str<0) str=!str;
      if(str2<0) str2=!str2;
      CreateParticle("WindSpark",x,y,0,0,1000,RGBa(BoundBy(1+str*3/2,1,255),0,BoundBy(1+str2*3/2,1,255),alpha));
    }
}

public func MyLogWind(szName)
{
  Log("global func DoWhatIW%sant(){",szName);
  Log("  var o = CreateObject(WSPE, 0, 0, -1);");
  for(var i = 1; i <= maxindex; i++)
    Log("  SetLocal(%d, %d, o);",i,Local(i));
  Log("  o->SetMaxIndex(%d);",i,maxindex);
  Log("  return o;");
  Log("}");
}
