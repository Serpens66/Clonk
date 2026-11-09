/*-- Funktionen --*/

// globale Ereignis Funktionen. Z.b monsterangriff oder Katastrophen

#strict
#appendto *

global func MMonsterAttack(smonster, iaufeinmal, ierledigen,ikill,ikeinflug,imessage) // string welches Monster, wieviele aufeinmal und wieviele getötet werden müssen, damit Angriff aufhört. ikill == 1 Wenn angriff aufhört, alle übrigen monster automatisch töten, wenn ikill==2 dann entfernen, iFlug bei Zufall, ob Flugeinheiten erlaubt.
{    // imessage, wenn wahr, wird nach erledigung der Monster eine erfolgsnachricht ausgegeben
var entferner;
if(!smonster || !iaufeinmal || !ierledigen) {Log("MMonsterAttack fehlt Parameter");return();}

var MonsterArray = CreateArray();
var MonsterArrayohneFlug = CreateArray();
MonsterArray=["Schlangen","Monster","Voegel","Baeren","FeuerMonster","Killerwipfe","Fledermaeuse","Kraehen","Phoenixe","Berserker","Jumper","Kamikaze","Moloch","Schroffel","Schmonk","Clonk","Ritter","Magier","Mutanten","Mireok","Predator","Scorpion","Hazard","Assasine","Wurm"];
MonsterArrayohneFlug = ["Schlangen","Monster","Baeren","FeuerMonster","Killerwipfe","Berserker","Jumper","Kamikaze","Schroffel","Schmonk","Clonk","Ritter","Magier","Mutanten","Hazard","Assasine","Wurm"];

if(smonster=="Zufall1")  // leichte Gegner
{
    if(!ikeinflug) smonster=MonsterArray[Random(3)];  // 0 bis 2 also schlangen bis voegel
    else smonster=MonsterArrayohneFlug[Random(2)];  // 0 bis 1
}
    
if(smonster=="Zufall2")  // mittelstarke Gegner
{
    if(!ikeinflug) smonster=MonsterArray[RandomX(3,8)];  // 3 bis 8, randomx verhaelt sich anders als random!
    else smonster=MonsterArrayohneFlug[RandomX(2,4)];  // baeren fmonster und killerwipfe
}
    
if(smonster=="Zufall3")  // intelligente "leichte" Feinde
{
    if(!ikeinflug) smonster=MonsterArray[RandomX(15,16)];   // Clonks,ritter und zaubererclonks
    else smonster=MonsterArrayohneFlug[RandomX(10,12)];  
}   

if(smonster=="Zufall4")  // Aliens
{
    if(!ikeinflug) smonster=MonsterArray[RandomX(9,14)];
    else smonster=MonsterArrayohneFlug[RandomX(5,9)];  
}   

if(smonster=="Zufall5")  // intelligente mittlere Feinde
{
    if(!ikeinflug) smonster=MonsterArray[RandomX(18,19)];   // Mutanten und Mireok
    else smonster=MonsterArrayohneFlug[13];  // Mutant
}   

if(smonster=="Zufall6")  // intelligente starke Feinde
{
    if(!ikeinflug) smonster=MonsterArray[RandomX(20,24)];  // predator, scorpion, hazardclonk, assasine, Wurm
    else smonster=MonsterArrayohneFlug[RandomX(14,16)];   // hazardclonk, assasine, Wurm
}   

if(smonster=="Zufall")
{
    if(!ikeinflug) smonster=MonsterArray[RandomX(0,24)];
    else smonster=MonsterArrayohneFlug[RandomX(0,16)];
}       

if(smonster=="Mutanten")  // Mutanten haben aktuell noch ein mieses KI verhalten und treffen den spieler nicht, wenn nofirendly fire. Erst noch mit EnemyAI skript machen.
{
    if(!ikeinflug) smonster=MonsterArray[RandomX(0,17)]; // so dass mutant nicht nochmal drankommen kann
    else smonster=MonsterArrayohneFlug[RandomX(0,12)];  
}   
    
if(smonster=="Monster")
 {
  while (ObjectCount(MOAN)<iaufeinmal)
  CreateObject(MOAN,1,1,-1);
  entferner = CreateObject(AAN_,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(smonster=="Berserker")
 {
  while (ObjectCount(BERA)<iaufeinmal)
  CreateObject(BERA,1,1,-1);
  entferner = CreateObject(AAN8,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(smonster=="FeuerMonster")
 {
  while (ObjectCount(FEAN)<iaufeinmal)
  CreateObject(FEAN,1,1,-1);
  entferner = CreateObject(AAN0,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(smonster=="Fledermaeuse")
 {
  while (ObjectCount(FL_N)<iaufeinmal)
  CreateObject(FL_N,1,1,-1);
  entferner = CreateObject(AA10,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(smonster=="Jumper")
 {
  while (ObjectCount(JUMA)<iaufeinmal)
  CreateObject(JUMA,1,1,-1);
  entferner = CreateObject(AAN6,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(smonster=="Kamikaze")
 {
  while (ObjectCount(KAMA)<iaufeinmal)
  CreateObject(KAMA,1,1,-1);
  entferner = CreateObject(AAN7,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(smonster=="Kraehen")
 {
  while (ObjectCount(K_AN)<iaufeinmal)
  CreateObject(K_AN,1,1,-1);
  entferner = CreateObject(AAN9,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(smonster=="Mutanten")
 {
  while (ObjectCount(MURA)<iaufeinmal)
  CreateObject(MURA,1,1,-1);
  entferner = CreateObject(AA12,1,1,-1);               
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(smonster=="Phoenixe")
 {
  while (ObjectCount(PH_A)<iaufeinmal)
  CreateObject(PH_A,1,1,-1);
  entferner = CreateObject(AA11,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(smonster=="Schlangen")
 {
  while (ObjectCount(SNAN)<iaufeinmal)
  CreateObject(SNAN,1,1,-1);
  entferner = CreateObject(AAN1,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(smonster=="Voegel")
 {
  while (ObjectCount(VOAN)<iaufeinmal+3)  // hier plus 3, weil es nicht unbedingt leicht ist, jeden Vogel zu erwischen, deswegen ein paar mehr machen
  CreateObject(VOAN,1,1,-1);
  entferner = CreateObject(AAN2,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(smonster=="Moloch")
 {
  while (ObjectCount(MOA_)<iaufeinmal+3)
  CreateObject(MOA_,1,1,-1);
  entferner = CreateObject(AAN5,1,1,-1);
  entferner ->~ Einstellen(ierledigen+6,ikill,imessage);  // molochs erledigen sich in kürzester zeit selbst, weil sie iwo gegenfliegen... also nen paar mehr erledigen
 }

else if(smonster=="Predator")
 {
  while (ObjectCount(PRA_)<iaufeinmal)
  CreateObject(PRA_,1,1,-1);
  entferner = CreateObject(AAN3,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(smonster=="Scorpion")
 {
  while (ObjectCount(SCA_)<iaufeinmal)
  CreateObject(SCA_,1,1,-1);
  entferner = CreateObject(AAN4,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(smonster=="Baeren")
 {
  while (ObjectCount(AN01)<iaufeinmal)
  CreateObject(AN01,1,1,-1);
  entferner = CreateObject(AA13,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(smonster=="Mireok")
 {
  while (ObjectCount(AN02)<iaufeinmal)
  CreateObject(AN02,1,1,-1);
  entferner = CreateObject(AA14,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }
 
 else if(smonster=="Killerwipfe")
 {
  while (ObjectCount(AN04)<iaufeinmal)     
  CreateObject(AN04,1,1,-1);
  entferner = CreateObject(AA18,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }
 
 else if(smonster=="Schmonk")
 {
  while (ObjectCount(AN05)<iaufeinmal)   
  CreateObject(AN05,1,1,-1);
  entferner = CreateObject(AA16,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }
 
 else if(smonster=="Schroffel")
 {
  while (ObjectCount(AN06)<iaufeinmal)        
  CreateObject(AN06,1,1,-1);
  entferner = CreateObject(AA17,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }
 
 else if(smonster=="Clonk")
 {
  while (ObjectCount(CLAN)<iaufeinmal)
  CreateObject(CLAN,1,1,-1);
  entferner = CreateObject(AA19,1,1,-1);     
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }
 
 else if(smonster=="Ritter")
 {
  while (ObjectCount(RI_N)<iaufeinmal)    
  CreateObject(RI_N,1,1,-1);
  entferner = CreateObject(AA21,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }
 
 else if(smonster=="Magier")
 {
  while (ObjectCount(MA_N)<iaufeinmal)    
  CreateObject(MA_N,1,1,-1);
  entferner = CreateObject(AA20,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }
 
 else if(smonster=="Hazard")
 {
  while (ObjectCount(HZ_N)<iaufeinmal)   
  CreateObject(HZ_N,1,1,-1);
  entferner = CreateObject(AA22,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }
 
  else if(smonster=="Assasine")
 {
  while (ObjectCount(AS_N)<iaufeinmal)   
  CreateObject(AS_N,1,1,-1);
  entferner = CreateObject(AA23,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }
 
   else if(smonster=="Wurm")
 {
  if (iaufeinmal>=2) iaufeinmal = iaufeinmal/2; // zuviele wurmer aufeinmal sind zu krass, deswegen halbieren.  spaeter evlt mal die zahl der wurmer por gebaude auf 1 setzen in wurmangriff
  while (ObjectCount(AN03)<iaufeinmal)   
  CreateObject(AN03,1,1,-1);
  entferner = CreateObject(AA15,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else Log(Format("Ungültiger Monstername: %v",smonster));

if(imessage) Message(Format("%v %v greifen an! Töte %v von ihnen!",iaufeinmal,smonster,ierledigen));
return(entferner); // die entferner Regel returnen, da diese ausgeben kann, wieviele feinde noch zu erledigen sind
}

// Katastrphen staerke evlt noch an Kartengroeße anpassen. Aktuell ist es so, je kleiner die karte, desto heftiger fallen die katastrophen aus.

global func KKatastrophen(skatastrophe, istaerke,idauer,imessage) // istaerke sollte von 1 bis 10, schwach bis stark gewählt werden. idauer die dauer in Frames. imessage sollte wenn wahr, eine nachricht ausgeben, aber zurzeit wird einfach immer eine nachricht ausgegeben.
{
    var aKatastrophen = ["Erdbeben","Meteoriten","Blitze","Vulkan","Eissturm","Gewitter","Quelle","Sauerevulkan"];  // bei blitze kommen unr blitze. Bei Gewitter erscheint eine Gewitterwolke, sofern Wolken erlaubt. Ansonsten auch nur Blitze
    if(GetTemperature()<10) aKatastrophen = ["Erdbeben","Eissturm","Blitze","Eissturm","Eissturm","Gewitter","Quelle","Sauerevulkan"]; // eissturm wahrscheinlicher, wenn unter 5 grad
    if(skatastrophe=="Zufall")
    {
        skatastrophe=aKatastrophen[RandomX(0,7)];
    }
    
    var dauer;
    if(!idauer) dauer = 1000; // wenn nicht angegeben, dann immer 1000 Frames lang (ca. 30 sekunden) 
    else dauer = idauer;
    var staerke = istaerke;
    if(!staerke) staerke=RandomX(1,10); // wenn nicht angegeben, zufällige Staerke
    if(skatastrophe=="Gewitter")
    {
        DynMessage(Format("%s der Stufe %v!",skatastrophe,staerke),0,24,RGBa(204,203,245,0));
        if(FindObject(WOL_)) // wenn keine wolken erlaubt, dann nur Blitze und regen
        {
            ScheduleKatastrophenAufruf("lBlitze", staerke*5, dauer); // Chance pro Frame: staerke*5 von 3700, bei etwa 37 Frames pro Sekunde.
            ScheduleKatastrophenAufruf("Regenxyz", staerke*2, dauer); // Ein Tropfen je Einheit staerke*2 pro Frame, ueber die Landschaft verteilt.
        }
        else
        {
            Regenwolkexyz(staerke*10,dauer,staerke*20); 
        }
    }
    else if(skatastrophe=="Blitze")
    {
        DynMessage(Format("%s der Stufe %v!",skatastrophe,staerke),0,24,RGBa(204,203,245,0));
        ScheduleKatastrophenAufruf("lBlitze", staerke*5, dauer);
    }
    else if(skatastrophe=="Erdbeben")
    {
        DynMessage(Format("%s der Stufe %v!",skatastrophe,staerke),0,24,RGBa(204,203,245,0));
        ScheduleKatastrophenAufruf("lErdbeben", staerke*2, dauer); // Wie bei Blitzen wird die Wahrscheinlichkeit pro Frame ausgewertet.
    }
    else if(skatastrophe=="Vulkan")
    {
        DynMessage(Format("%s der Stufe %v!",skatastrophe,staerke),0,24,RGBa(204,203,245,0));
        // Schedule(Format("lVulcan(%v,%v,%v)",staerke*3,staerke*5,"Zufall"),1, dauer);   // ein ausbruch reicht, daher deaktivert
        Vulcanxyz(0,"Zufall",staerke*5);
    }
    else if(skatastrophe=="Meteoriten")
    {
        DynMessage(Format("%s der Stufe %v!",skatastrophe,staerke),0,24,RGBa(204,203,245,0));
        ScheduleKatastrophenAufruf("lMeteoriten", staerke*5, dauer);
    }
    else if(skatastrophe=="Eissturm") // Eissturm, mit eismeteoriten
    {
        DynMessage(Format("%s der Stufe %v!",skatastrophe,staerke),0,24,RGBa(204,203,245,0));
        Eismeteoritenxyz(staerke, 1, dauer); // unabhängig von Temperatur (temperatur nicht ändern, weil sonst gewitter aufziehen)
        if(GetTemperature()<0) ScheduleKatastrophenAufruf("Regenxyz", staerke*2, dauer); // schnee/eisregen nur, wenn es kalt genug ist.
    }
    else if(skatastrophe=="Quelle") // Wasser oder Öl sprudeln als vulkan iwo in der erde
    {
        DynMessage(Format("%s der Stufe %v!",skatastrophe,staerke),0,24,RGBa(204,203,245,0));
        var aMat = ["Water","Oil"];
        Vulcanxyz(0,"Zufall",staerke*5,aMat[RandomX(0,1)]);
    }
    else if(skatastrophe=="Sauerevulkan") 
    {
        DynMessage(Format("%s der Stufe %v!",skatastrophe,staerke),0,24,RGBa(204,203,245,0));
        Vulcanxyz(0,"Zufall",staerke*5,"Acid");
    }
    else 
        Log(Format("ungueltiger Katastrophen Name: %v",skatastrophe));
}

global func ScheduleKatastrophenAufruf(string funktion, int wert, int dauer)
{
    if (this()) return(ScheduleCall(this(), "Sammlung42KatastrophenTimer", 1, dauer, funktion, wert));
    return(Schedule(Format("%s(%v)", funktion, wert), 1, dauer));
}

// ScheduleCall sucht die Funktion im Skript des Zielobjekts.
private func Sammlung42KatastrophenTimer(string funktion, int wert)
{
    if (funktion S= "lBlitze") return(lBlitze(wert));
    if (funktion S= "lErdbeben") return(lErdbeben(wert));
    if (funktion S= "lMeteoriten") return(lMeteoriten(wert));
    if (funktion S= "Regenxyz") return(Regenxyz(wert));
}

global func lBlitze(ichance) 
{
    if(RandomX(1,3700)<=ichance)  Blitzexyz(); // 3700, weil es pro sekunde mit einer chance von ichance Prozent gemacht werden soll, es aber jeden frame aufgerufen wird und eine sekunde ca 37frames sind.
}

global func lErdbeben(ichance) 
{
    if(RandomX(1,3700)<=ichance)  Erdbebenxyz();
}

// global func lVulcan(ichance,istaerke,y) // vulcan ohne schedule usw. weil ein ausbruch reicht
// {
    // if(RandomX(1,3700)<=ichance)  
    // {
        // Log(Format("%v , %v , %v",ichance,istaerke,y));
        // Vulcanxyz(0,y,istaerke); // zufälliges x
    // }
// }

global func lMeteoriten(ichance) 
{
    if(RandomX(1,3700)<=ichance)  Meteoritenxyz();
}


