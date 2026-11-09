#strict 3

static globalwestwinde,globalostwinde;
static horizont;
static WINDAUFLOESUNG;
static maxtiefe;

local ost,west,hang,factor; 	// für Rasterobjekte

/* Start. Entweder ein Speicherobjekt oder das Berechnungsobjekt */
protected func Initialize()
{
	if((ObjectCount(WIND) ?? 0)>=1) return(1); // gewöhnliches Rasterobjekt?
	MutterObjekt();
	return(1);
}

/* In einem Rasterobjekt werden die Windstärken gespeichert. 
   Nach vollenden des globalen Rechenvorganges wird dieses Objekt gelöscht. */
public func RasterObjekt()
{
	ost=west=hang=nil;
	/* Wenn Himmel gefunden wird, soll das Objekt bestehen */
	for(var i=-(WINDAUFLOESUNG ?? 0)/2; (i ?? 0)<(WINDAUFLOESUNG ?? 0)/2; S42MigrationStep(i, 1, true))
	 for(var j=-(WINDAUFLOESUNG ?? 0)/2; (j ?? 0)<(WINDAUFLOESUNG ?? 0)/2; S42MigrationStep(j, 1, true))
	  if(Equal(GetMaterial(i,j), Material("Sky")))
	   return(1);
	/* Ansonsten wird es eben gelöscht */
	RemoveObject();
	return(nil);
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
	var temp=nil;
	/* Horizont ermitteln */
	for(var x=nil; (x ?? 0)<(LandscapeWidth() ?? 0); S42MigrationStep(x, 1, true))
	{
		S42MigrationAssign(temp, "+=", (GibHorizont(x) ?? 0));
		horizont=(temp ?? 0)/(LandscapeWidth() ?? 0);
		/* Die maximale Tiefe soll 50% zählen */
		horizont=((horizont ?? 0)+(maxtiefe ?? 0))/2;
	}
}

/* Ermittelt die Erdoberfläche in Spalte X und speichert zudem (falls nötig) einen neuen Höchstwert.
   Dieser gibt dann die niedrigste Höhe an, die die Erdoberfläche hat */
global func GibHorizont(x)
{
	for(var y=nil; (y ?? 0)<(LandscapeHeight() ?? 0); S42MigrationStep(y, 1, true))
	  if(!Equal(GetMaterial(x,y), Material("Sky")))
	  {
		if((maxtiefe ?? 0)<(y ?? 0)) maxtiefe=y;
	    	return(y);
	  }
	if((maxtiefe ?? 0)<(y ?? 0)) maxtiefe=y;
 	return(y);
}

/* Berechnungsobjekt. Dieses berechnet die Windstärken und speichert sie in den Rasterobjekten. */
private func MutterObjekt()
{
	/* Vorbereiten */
	Log("Windströmungen werden berechnet. Der Vorgang kann einige Minuten dauern... ");
	WINDAUFLOESUNG=WindAufloesung();
	SetPosition(nil,nil);
	ErmittleHorizont();

	/* Alle Rasterobjekte erstellen */
	Log(" Windströmung: Speicherobjekte erstellen");
	var obj;
	for(var x=nil; (x ?? 0)<(LandscapeWidth() ?? 0); S42MigrationAssign(x, "+=", (WINDAUFLOESUNG ?? 0)))
	 for(var y=nil; (y ?? 0)<(maxtiefe ?? 0)+(WINDAUFLOESUNG ?? 0)/2; S42MigrationAssign(y, "+=", (WINDAUFLOESUNG ?? 0)))	// Maxtiefe bei der Horizontberechnung ermittelt
	  if(obj=CreateObject(WIND,x,y))
	  {
		  obj->WIND::RasterObjekt();
		  if(obj)
	  	  obj->WIND::SetzHorizont(horizont);
	  }

	/* Stromberechnungen für jede Richtung einzeln */
	for(y=nil; (y ?? 0)<(maxtiefe ?? 0)+(WINDAUFLOESUNG ?? 0)/2; S42MigrationAssign(y, "+=", (WINDAUFLOESUNG ?? 0)/3))
	{
		if(!((y ?? 0)%100)) Log(" Windströme ab Zeile %d berechnen",y);
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
	for(var x=nil; (x ?? 0)<(LandscapeWidth() ?? 0); S42MigrationAssign(x, "+=", (WINDAUFLOESUNG ?? 0)/3))
	{
		/* Frei? Wind verstärken */
		if(!GBackSemiSolid(x,y))
		{
		 if(((S42MigrationAssign(stromstaerke, "+=", (WINDAUFLOESUNG ?? 0)/3)) ?? 0) > 1000)
		  stromstaerke = 1000;
		}
		/* Nicht frei? */
		else
		{
		 	/* Hang? */
			if(stromstaerke) FindObject(WIND,x,y,-1,-1)->WIND::Hang(1);
			stromstaerke=nil;
		}
		/* Windpunkt ändern */
		FindObject(WIND,x,y,-1,-1)->WIND::AenderStaerke(1, (stromstaerke ?? 0)/10);
		//Log("%d/%d",x,LandscapeWidth());
	};
	return;
}

private func StarteStroemungOst(y)
{
  var stromstaerke=40;
	for(var x=LandscapeWidth(); (x ?? 0)>1; S42MigrationAssign(x, "-=", (WINDAUFLOESUNG ?? 0)/3))
	{
		/* Frei? Wind verstärken */
		if(!GBackSemiSolid(x,y))
		{
		 if(((S42MigrationAssign(stromstaerke, "+=", (WINDAUFLOESUNG ?? 0)/3)) ?? 0) > 1000)
		  stromstaerke = 1000;
		}
		/* Nicht frei? */
		else
		{
		 	/* Hang? */
			if(stromstaerke) FindObject(WIND,x,y,-1,-1)->WIND::Hang(nil);
			stromstaerke=nil;
		}
		/* Windpunkt ändern */
		var obj;
		if(obj=FindObject(WIND,x,y,-1,-1)) obj->WIND::AenderStaerke(nil, (stromstaerke ?? 0)/10);
		//Log("%d/%d",x,LandscapeWidth());
	};
	return;
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
		 return(nil);
		S42MigrationAssign(west, "+=", (Prozent(staerke,RandomX(90,110)) ?? 0));
		west=(west ?? 0)/2;
		if((west ?? 0)<5) west=5; // Windstille solls nun doch nicht geben
		return(1);
	}
	if(!bwest)
	{
		if(GetBit(hang,1)) // Ostthang?
		 return(nil);
		S42MigrationAssign(ost, "+=", (Prozent(staerke,RandomX(90,110)) ?? 0));
		ost=(ost ?? 0)/2;
		if((ost ?? 0)<5) ost=5; // Windstille solls nun doch nicht geben
		return(1);
	}
	return;
}

/* Dieses Rasterobjekt ist an einem Hang.*/
public func Hang(bwest)
{
	if(bwest) // westen? 
	 if(!GetBit(hang,2))
	  S42MigrationAssign(west, "+=", 50);
	if(!bwest) // Osten?
	 if(!GetBit(hang,1))
	  S42MigrationAssign(ost, "+=", 50);
	hang=SetBit(hang,(bwest ?? 0)+1,1);
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
	if((normalwind ?? 0)>=0)
		return((globalwestwinde->GetStrength((GetX() ?? 0)+(x ?? 0),(GetY() ?? 0)+(y ?? 0)) ?? 0)*(normalwind ?? 0)/100);
	else
        return((globalwestwinde->GetStrength((GetX() ?? 0)+(x ?? 0),(GetY() ?? 0)+(y ?? 0)) ?? 0)*(normalwind ?? 0)/100);
}
