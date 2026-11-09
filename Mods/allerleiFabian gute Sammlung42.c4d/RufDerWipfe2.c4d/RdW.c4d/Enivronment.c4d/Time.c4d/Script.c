/*-- Tageszeiten --*/

#strict

// Kann mittels des neuen Spielzielauswählers
public func IsVariabelChooseable2() { return(1); }

static flash,dayhelligkeit,pseudodayhelligkeit,dayspeed,daytime,fDay,iDay,startdayhelligkeit,minhelligkeit;

static LightNeeded; 
//DayDarkness;      // globale Variablen:
//static Time_RedIntensity, daytime;    // wenn kein Time-Objekt aktiviert ist, sind alle 0
local speed, redfactor;

/* Initialisierung */

protected func Initialize()
{
  var pTime;
  if(pTime=FindObject(TIME))// Ist schon eins da?
  {
   dayspeed++;// Geschwindigkeit erhöhen
   RemoveObject();
   return 0;
  }
  // Sonst Grundobjekt
  dayspeed=2;
  dayhelligkeit=60;
  startdayhelligkeit=60;
  minhelligkeit = 15;
//  pseudodayhelligkeit=50;  // anstelle von pseudohelligkeit, wird alles weiterhin normal gerechnet, nur setgamma und matadjust wird nicht ausgeführt
  // GO!!!
  SetAction("Time");
  // Grafiken anpassen nicht vergessen...
  UpdateGraphics();
  // ...und die Sonne in den Himmel setzen
  CreateObject(SMON,10,10,-1);
  CreateObject(SSUN,10,10,-1);
  
  return(1);
}
  
/* Konfiguration */

// ianpassen ist 1, wenn regel mitten im Spiel aktiv wird, damit mithilfe von setTime dann mittags gemacht wird und die Lichtverhältnisse gemacht werden
// wenn KOHE aktiviert und der normale tagesverlauf stattfindet, dann soll die Änderung in material nicht gemacht werde. Aber wenn KOHE aktiviert wird, muss SetTime mittags mit ianpassen gesetzt werden, damit die farben so sind, wie sich tagsüber ohne helligkeitsänderungen wären.
// d.h. für die normale Funktion ohne KOHE, ist der ianpassen wert egal

public func SetTimeTIME(iTime,ianpassen) // int iTime 0-100=Mittag-Mittag
{
  // Zeit setzen
  daytime=Par(0)*TimeResolution()/100;
  // Helligkeit anpassen
//if(FindObject(KOHE))
//{dayhelligkeit=BoundBy( Cos((daytime*360)/TimeResolution(),250)+60 ,startdayhelligkeit,startdayhelligkeit);}  // bleibt immer beim Wert von starthelligkeit... 
//else {
dayhelligkeit=BoundBy( Cos((daytime*360)/TimeResolution(),250)+60 ,0,startdayhelligkeit);
//}  // minimal 5 und maximal starthelligkeit
//pseudodayhelligkeit=BoundBy( Cos((daytime*360)/TimeResolution(),250)+60 ,0,startdayhelligkeit);  // im Fall von konstanter Helligkeit, damit Tag/Nacht trzd. funktioniert
  RestoreColors(BoundBy(dayhelligkeit+minhelligkeit,minhelligkeit,startdayhelligkeit),ianpassen);
  return(1);
}

global func Flash(blitz)
{
  return(flash+=blitz);
}
 
global func TimeResolution(){ return 10000;}
  
/* Zeitverlauf */  

private func Timing()
{
  // Zeitfortschritt
  daytime=(daytime+dayspeed/2)%TimeResolution();
  // Helligkeit nach Tageszeit berechnen
  var gamma;
//if(FindObject(KOHE))
// gamma=BoundBy( Cos((daytime*360)/TimeResolution(),250)+60 ,startdayhelligkeit,startdayhelligkeit);  // bleibt immer beim Wert von starthelligkeit... 
//else
  gamma=BoundBy( Cos((daytime*360)/TimeResolution(),250)+60 ,0,startdayhelligkeit);
  if(flash>0)
   flash=BoundBy(flash-15,0,200);
  // Helligkeit und Himmelsfarbe anpassen
  if (gamma>dayhelligkeit) ++dayhelligkeit;
  if (gamma<dayhelligkeit) --dayhelligkeit;
  if (gamma!=dayhelligkeit||flash||Random(10)>9) RestoreColors(BoundBy(dayhelligkeit+minhelligkeit,minhelligkeit,startdayhelligkeit)+flash);
  // Fertig
  return(1);
}
  
protected func UpdateTransferZone()
{
  // Kleiner Trick, damit der Himmel nach dem Laden aktualisiert wird
  if (GetAction() S= "Time") RestoreColors(BoundBy(dayhelligkeit+minhelligkeit,minhelligkeit,startdayhelligkeit));
  return(0);  
}
  
/* Status */

global func IsDay()
{
//  if(FindObject(KOHE))  // falls keine helligkeitsänderung, dann an pseudohelligkeit orientieren
//  if(ObjectCount2(Find_ID(TIME),Find_Action("Time")))
//    return pseudodayhelligkeit>5;

//  if(!FindObject(KOHE))
  if(ObjectCount2(Find_ID(TIME),Find_Action("Time")))
    return dayhelligkeit>5;
return 0;
}

global func IsNight()
{
  if(ObjectCount2(Find_ID(TIME),Find_Action("Time")))
    return !IsDay();
}

global func IsVeryDay()
{

//  if(FindObject(KOHE))  // falls keine helligkeitsänderung, dann an pseudohelligkeit orientieren
//  if(ObjectCount2(Find_ID(TIME),Find_Action("Time")))
//    return pseudodayhelligkeit>25;

//  if(!FindObject(KOHE))
  if(ObjectCount2(Find_ID(TIME),Find_Action("Time")))
    return dayhelligkeit>25;
}

/* Farbfunktionen */
public func RestoreColors(gamma, ianpassen)
{
 var blue=50-gamma/2;
  var pMoon,iMoon;
  if(pMoon=FindObject(SMON))
  {
    iMoon=Abs(pMoon->GetMoonPhase()-6)*6;
    gamma+=BoundBy((iMoon-gamma/3),0,50);
  }

  RestoreSkyColors(gamma,0,0,blue);
  RestoreMatColors(gamma,0,0,blue,ianpassen);
  // Umwelt druff
  var green=ProzentVon(globalnature,globalpollution)/3;
  var red  =CalcDayRed();
  gamma = (gamma-50)*2;

// setgamma weglassen aber check light für sonnenbewegung machen, wenn KOHE. ianpassen ist 1, wenn regel mitten im Spiel aktiv wird, damit mithilfe von setTime dann mittags gemacht wird und die Lichtverhältnisse gemacht werden
if(FindObject(KOHE) && !ianpassen)
{
for(var obj in FindObjects(Find_Func("IsLight")))
    obj->~CheckLight();
return(1);
}

  SetGamma(RGB(BoundBy(0+ red/2,0,255),BoundBy(0+ green,0,255),0),RGB(BoundBy(128+gamma+red,0,255),BoundBy(128+green+gamma,0,255),BoundBy(128+gamma,0,255)),RGB(BoundBy(245+gamma/4+red/4,0,255),BoundBy(245+gamma/4,0,255),BoundBy(245+gamma/4,0,255)),1);
for(var obj in FindObjects(Find_Func("IsLight")))
    obj->~CheckLight();
  return(1);
}

public func RestoreSkyColors(ipercent,exred,exgreen,exblue)
{ 
// himmel kann ruhig dunkler werden
//if(FindObject(KOHE))  
//return(1);

  /* Helligkeit */
  var red,green,blue;
if (IsVeryDay()) {red=green=blue=255*(ipercent-20)/100;};
 if (IsNight()) {red=green=blue=255*(ipercent-40)/100;};
if(IsDay()) {  red=green=blue=255*ipercent/100;};
  /* Extrafärbung drauf */
  red+=exred;
  green+=exgreen;
  blue+=exblue;
  /* Limits */
  if(red>255) red=255;  if(green>255) green=255;  if(blue>255) blue=255;
  if(red<000) red=000;  if(green<000) green=000;  if(blue<000) blue=000;
  /* Umfärben */
// if (IsNight()){  SetSkyAdjust(RGB((red+55)/2,(green+55)/2,(blue+55)/2));}    // himmel soll nicht pechschwarz sein
// else {
SetSkyAdjust(RGB((red+255)/2,(green+255)/2,(blue+255)/2));
// };
  return(1);
}  

public func RestoreMatColors(ipercent,exred,exgreen,exblue,ianpassen)
{
  exred+=GetTemperature()*2;
  exblue-=GetTemperature()*2;
  var red,green,blue;
  var gamma=100-(100-ipercent)*5/6;
  red=green=blue=255*gamma/100;
  /* Extrafärbung drauf */
  red+=exred;
  green+=exgreen;
  blue+=exblue;
  /* Limits */
  if(red>255) red=255;  if(green>255) green=255;  if(blue>255) blue=255;
  if(red<000) red=000;  if(green<000) green=000;  if(blue<000) blue=000;
  /* Umfärben */
if(FindObject(KOHE) && !ianpassen) // wenn KOHE aktiviert und der normale tagesverlauf stattfindet, dann soll die folgende Änderung nicht gemacht werde. Aber wenn KOHE aktiviert wird, muss SetTime mittags mit ianpassen gesetzt werden, damit die farben so sind, wie sich tagsüber ohne helligkeitsänderungen wären.
return(1);
  SetMatAdjust(RGB((red+255+255)/3,(green+255+255)/3,(blue+255+255)/3));
  return(1);  
}

private func CalcDayRed()
{
  var perc = (dayhelligkeit*2-10)*2;
  if(!Inside(perc,1,199))
    return 0;
  if(perc>100)
    return Prozent(40,200-perc);
  return Prozent(40,perc);
}

/* Zeitverlauf */

private func TimingTwo()
{
    
    // ein großes schwarzes rechteck, welches nachts die sicht ein wenig verdunkeln soll, damit setgamma nicht so dunkel gemacht werden muss (weil man sonst auch das menü nicht mehr sieht)
    var grad;
    if(GetLength(hashSpielRegeln))
        grad=HashGet(hashSpielRegeln, "Dunkelheitsgrad"); // kann 100 oder 50 annehmen. Wenn 100, alles normal. Wenn 50, dann dieses dunkelheitsobjekt nicht verdunkeln
    if(!FindObject(KOHE) && grad==100) // wenn die entsprechendenn Regeln gesetzt sind
        for(var i = 0; i < GetPlayerCount(); i++)  
        {
            if(!FindObject2(Find_ID(DUUU),Find_Owner(GetPlayerByIndex(i)))) // nur erstellen, wenn es fuer den spieler noch keinees gibt 
                CreateObject(DUUU,0,0,GetPlayerByIndex(i));  // das verhalten wird im DUUU objekt selbst gemacht
            if(ObjectCount2(Find_ID(DUUU),Find_Owner(GetPlayerByIndex(i)))>=2) // wenn 2 oder mehr gefunden werden, dann eines entfernen
                RemoveObject(FindObject2(Find_ID(DUUU),Find_Owner(GetPlayerByIndex(i))));
        }
    else // wenn Regeln keine DUUU objekte erlauben, dann alle entfernen
    {
        for(var obj in FindObjects(Find_ID(DUUU)))
            RemoveObject(obj);
    }
    if(FindObject2(Find_ID(DUUU),Find_Owner(-1))) // besitzerlose objekte entfernen
        RemoveObject(FindObject2(Find_ID(DUUU),Find_Owner(-1)));
        
    
  // Grafiken updaten...
  UpdateGraphics();
  // Zeitfortschritt
if(IsNight()) LightNeeded=1;  
if(IsVeryDay())
  {
    if(!fDay) {SunRise();LightNeeded=0;}
    if(FindObject2(Find_Action("Fensterln"))) RemoveWindows();
  }
  else
  	if(fDay) Dawn();
  // Fertig
  return(1);
}

public func UpdateGraphics()
{
if(!FindObject2(Find_ID(SES_)))  //Regel für keine Jahreszeiten
{
  // Schneegrafiken
  var object,graphics="";
  if(GetTemperature()<0) graphics="Schnee";
  for(object in FindObjects(Find_Exclude(),Find_Not(Find_OCF(OCF_OnFire)),Find_Or(Find_Category(C4D_Structure|C4D_Vehicle),Find_Func("ForceGraphChange")),Find_Not(Find_Func("RejectGraphChange",graphics))))
  {
    // Am Anfang Grafiken sofort aktualisieren
    if(FrameCounter()<51)
    {        
      if(!object->~ChangeGraphic(graphics))
        SetGraphics(graphics,object);
    }
    else
    {
      if(!Random(15))
        if(!object->~UpdateGraphic(graphics))
          FadeGraphics(graphics,object);
    }
  }
  // Bäume haben spezielle Grafiken
 // ForestChange();  wird schon im umweltschutz aufgerufen
}
}

private func SunRise()
{
  // Tag/Nacht umschalten
  fDay=1;
  iDay++;
//  Sound("Bell",1);
//  EventMessage("NewDay",Format("%d. Tag",iDay),0,0);
  var object;
  for(var object in FindObjects(Find_Not(Find_ID(GetID(this())))))
    object->~SunRise();
  return(1);
}

private func Dawn()
{
  // Tag/Nacht umschalten
//  EventMessage("Night","",0,0);
  fDay=0;
  for(var object in FindObjects(Find_Not(Find_ID(GetID(this())))))
	 object->~Dawn(Random(3));// Callback an die Objekte senden
  return(1);
}

private func RemoveWindows()
{
  for(var pObj in FindObjects(Find_Action("FensterIn"))) if(!Random(4)) RemoveObject(pObj);
  return(1);
}

// Spielkonfiguration

public func IsChooserConfig(){return true;}

public func ConfigSettings(iIndex,iSymb)
{
  if(!iIndex--){ if(!iSymb) return Format("Tagesgeschwindigkeit erhöhen...  (%d/10)",dayspeed); if(iSymb==2) return 1; return XSYX;}
  if(!iIndex--){ if(!iSymb) return Format("Tagesgeschwindigkeit veringern...  (%d/10)",dayspeed); if(iSymb==2) return 2; return XSYX;}
}

public func ConfigSettingsCall(iIndex)
{
  if(!iIndex--)
    dayspeed=BoundBy(dayspeed+1,1,10);
  if(!iIndex--)
    dayspeed=BoundBy(dayspeed-1,1,10);
}
