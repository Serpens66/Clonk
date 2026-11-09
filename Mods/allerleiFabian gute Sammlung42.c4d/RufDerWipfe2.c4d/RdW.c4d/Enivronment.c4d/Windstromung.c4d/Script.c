#strict

static globalwestwinde,globalostwinde;
static horizont;
static WINDAUFLOESUNG;
static maxtiefe;

local ost,west,hang,factor; 	// für Rasterobjekte

/* Start. Entweder ein Speicherobjekt oder das Berechnungsobjekt */
protected func Initialize()
{
	if(ObjectCount(WIND)>=1) return(1); // gewöhnliches Rasterobjekt?
	MutterObjekt();
	return(1);
}

/* In einem Rasterobjekt werden die Windstärken gespeichert. 
   Nach vollenden des globalen Rechenvorganges wird dieses Objekt gelöscht. */
public func RasterObjekt()
{
	ost=west=hang=0;
	/* Wenn Himmel gefunden wird, soll das Objekt bestehen */
	for(var i=-WINDAUFLOESUNG/2; i<WINDAUFLOESUNG/2; i++)
	 for(var j=-WINDAUFLOESUNG/2; j<WINDAUFLOESUNG/2; j++)
	  if(GetMaterial(i,j)==Material("Sky"))
	   return(1);
	/* Ansonsten wird es eben gelöscht */
	RemoveObject();
	return(0);
}

/* Bestimmt das Raster */
global func WindAufloesung()
{
	/* Je kleiner die Zahl, desto feiner das Raster */
	/* Warnung: Bei RdW sind die vorberechneten Werte auf 50 ausgelegt */
	return(50);
}

/* Der Horizont ist in RdW die durchschnittliche Höhe der Erdoberfläche. 
   Hier fällt der Niederschlag und hier ist auch die höchste Windstärke */
global func ErmittleHorizont()
{
	var temp=0;
	/* Horizont ermitteln */
	for(var x=0; x<LandscapeWidth(); x++)
	{
		temp+=GibHorizont(x);
		horizont=temp/LandscapeWidth();
		/* Die maximale Tiefe soll 50% zählen */
		horizont=(horizont+maxtiefe)/2;
	}
}

/* Ermittelt die Erdoberfläche in Spalte X und speichert zudem (falls nötig) einen neuen Höchstwert.
   Dieser gibt dann die niedrigste Höhe an, die die Erdoberfläche hat */
global func GibHorizont(x)
{
	for(var y=0; y<LandscapeHeight(); y++)
	  if(GetMaterial(x,y)!=Material("Sky"))
	  {
		if(maxtiefe<y) maxtiefe=y;
	    	return(y);
	  }
	if(maxtiefe<y) maxtiefe=y;
 	return(y);
}

/* Berechnungsobjekt. Dieses berechnet die Windstärken und speichert sie in den Rasterobjekten. */
private func MutterObjekt()
{
	/* Vorbereiten */
	Log("Windströmungen werden berechnet. Der Vorgang kann einige Minuten dauern... ");
	WINDAUFLOESUNG=WindAufloesung();
	SetPosition(0,0);
	ErmittleHorizont();

	/* Alle Rasterobjekte erstellen */
	Log(" Windströmung: Speicherobjekte erstellen");
	var obj;
	for(var x=0; x<LandscapeWidth(); x+=WINDAUFLOESUNG)
	 for(var y=0; y<maxtiefe+WINDAUFLOESUNG/2; y+=WINDAUFLOESUNG)	// Maxtiefe bei der Horizontberechnung ermittelt
	  if(obj=CreateObject(WIND,x,y))
	  {
		  obj->WIND::RasterObjekt();
		  if(obj)
	  	  obj->WIND::SetzHorizont(horizont);
	  }

	/* Stromberechnungen für jede Richtung einzeln */
	for(y=0; y<maxtiefe+WINDAUFLOESUNG/2; y+=WINDAUFLOESUNG/3)
	{
		if(!(y%100)) Log(" Windströme ab Zeile %d berechnen",y);
		StarteStroemungOst(y);
		StarteStroemungWest(y);
	}

	/* Alles speichern */
	Log(" Windströmung: Speichern");
	globalwestwinde = CreateObject(WSPE);
		globalwestwinde->SpeicherWest();
	globalostwinde =  CreateObject(WSPE);
		globalostwinde->SpeicherOst();

	/* Alles töten */
	Log(" Windströmung: Überflüssige Speicherobjekte entfernen");
	while(obj=FindObject(WIND))
	  RemoveObject(obj);
	return(1);
}

/* Windströmung berechnen */
private func StarteStroemungWest(y)
{
	var stromstaerke=40;
	for(var x=0; x<LandscapeWidth(); x+=WINDAUFLOESUNG/3)
	{
		/* Frei? Wind verstärken */
		if(!GBackSemiSolid(x,y))
		{
		 if((stromstaerke+=WINDAUFLOESUNG/3) > 1000)
		  stromstaerke = 1000;
		}
		/* Nicht frei? */
		else
		{
		 	/* Hang? */
			if(stromstaerke) FindObject(WIND,x,y,-1,-1)->WIND::Hang(1);
			stromstaerke=0;
		}
		/* Windpunkt ändern */
		FindObject(WIND,x,y,-1,-1)->WIND::AenderStaerke(1, stromstaerke/10);
		//Log("%d/%d",x,LandscapeWidth());
	};
	return();
}

private func StarteStroemungOst(y)
{
  var stromstaerke=40;
	for(var x=LandscapeWidth(); x>1; x-=WINDAUFLOESUNG/3)
	{
		/* Frei? Wind verstärken */
		if(!GBackSemiSolid(x,y))
		{
		 if((stromstaerke+=WINDAUFLOESUNG/3) > 1000)
		  stromstaerke = 1000;
		}
		/* Nicht frei? */
		else
		{
		 	/* Hang? */
			if(stromstaerke) FindObject(WIND,x,y,-1,-1)->WIND::Hang(0);
			stromstaerke=0;
		}
		/* Windpunkt ändern */
		var obj;
		if(obj=FindObject(WIND,x,y,-1,-1)) obj->WIND::AenderStaerke(0, stromstaerke/10);
		//Log("%d/%d",x,LandscapeWidth());
	};
	return();
}

/* lokale Windstärke abfragen */
public func GetStrength(bwest)
{
	if( bwest) return(west);
	if(!bwest) return(ost);
}

/* Windströmung ändern */
public func AenderStaerke(bwest,staerke)
{
	if(bwest)
 	{
		if(GetBit(hang,2)) // Westhang?
		 return(0);
		west+=Prozent(staerke,RandomX(90,110));
		west=west/2;
		if(west<5) west=5; // Windstille solls nun doch nicht geben
		return(1);
	}
	if(!bwest)
	{
		if(GetBit(hang,1)) // Ostthang?
		 return(0);
		ost+=Prozent(staerke,RandomX(90,110));
		ost=ost/2;
		if(ost<5) ost=5; // Windstille solls nun doch nicht geben
		return(1);
	}
	return();
}

/* Dieses Rasterobjekt ist an einem Hang.*/
public func Hang(bwest)
{
	if(bwest) // westen? 
	 if(!GetBit(hang,2))
	  west+=50;
	if(!bwest) // Osten?
	 if(!GetBit(hang,1))
	  ost+=50;
	hang=SetBit(hang,bwest+1,1);
	return(1);
}

/* Speichert den Horizontfaktor dieses Rasterobjektes. Je näher am Horizont, desto stärker der Wind. */
public func SetzHorizont(horizont)
{
	return(1);
}

/* Windabfrage */
global func GetWind(x,y,fGlobal)
{
	/* Erstmal den Normalwind ermitteln */
	var normalwind=inherited(x,y,fGlobal);
  if(fGlobal)
   return normalwind;
	/* Falls keine Winddaten gespeichert sind, so wird der Normalwind zurückgegeben */
	if(!globalwestwinde) return(normalwind);

	/* Anhand der Normalwindstärke Ost- oder Westwind zurückgeben */
	if(normalwind>=0) 	
		return(globalwestwinde->GetStrength(GetX()+x,GetY()+y)*normalwind/100);
	else
  	return(globalwestwinde->GetStrength(GetX()+x,GetY()+y)*normalwind/100);
}
