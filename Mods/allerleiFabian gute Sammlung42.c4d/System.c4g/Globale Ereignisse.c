/*-- Funktionen --*/

// globale Ereignis Funktionen. Z.b monsterangriff oder Katastrophen

#strict 3
#appendto *

global func MMonsterAttack(smonster, iaufeinmal, ierledigen,ikill,ikeinflug,imessage) // string welches Monster, wieviele aufeinmal und wieviele getötet werden müssen, damit Angriff aufhört. ikill == 1 Wenn angriff aufhört, alle übrigen monster automatisch töten, wenn ikill==2 dann entfernen, iFlug bei Zufall, ob Flugeinheiten erlaubt.
{    // imessage, wenn wahr, wird nach erledigung der Monster eine erfolgsnachricht ausgegeben
var entferner;
if(Or(Or(!smonster, !iaufeinmal), !ierledigen)) {Log("MMonsterAttack fehlt Parameter");return;}

var MonsterArray = CreateArray();
var MonsterArrayohneFlug = CreateArray();
MonsterArray=["Schlangen","Monster","Voegel","Baeren","FeuerMonster","Killerwipfe","Fledermaeuse","Kraehen","Phoenixe","Berserker","Jumper","Kamikaze","Moloch","Schroffel","Schmonk","Clonk","Ritter","Magier","Mutanten","Mireok","Predator","Scorpion","Hazard","Assasine","Wurm"];
MonsterArrayohneFlug = ["Schlangen","Monster","Baeren","FeuerMonster","Killerwipfe","Berserker","Jumper","Kamikaze","Schroffel","Schmonk","Clonk","Ritter","Magier","Mutanten","Hazard","Assasine","Wurm"];

if(Equal(smonster, "Zufall1"))  // leichte Gegner
{
    if(!ikeinflug) smonster=MonsterArray[(Random(3) ?? 0)];  // 0 bis 2 also schlangen bis voegel
    else smonster=MonsterArrayohneFlug[(Random(2) ?? 0)];  // 0 bis 1
}
    
if(Equal(smonster, "Zufall2"))  // mittelstarke Gegner
{
    if(!ikeinflug) smonster=MonsterArray[(RandomX(3,8) ?? 0)];  // 3 bis 8, randomx verhaelt sich anders als random!
    else smonster=MonsterArrayohneFlug[(RandomX(2,4) ?? 0)];  // baeren fmonster und killerwipfe
}
    
if(Equal(smonster, "Zufall3"))  // intelligente "leichte" Feinde
{
    if(!ikeinflug) smonster=MonsterArray[(RandomX(15,16) ?? 0)];   // Clonks,ritter und zaubererclonks
    else smonster=MonsterArrayohneFlug[(RandomX(10,12) ?? 0)];
}   

if(Equal(smonster, "Zufall4"))  // Aliens
{
    if(!ikeinflug) smonster=MonsterArray[(RandomX(9,14) ?? 0)];
    else smonster=MonsterArrayohneFlug[(RandomX(5,9) ?? 0)];
}   

if(Equal(smonster, "Zufall5"))  // intelligente mittlere Feinde
{
    if(!ikeinflug) smonster=MonsterArray[(RandomX(18,19) ?? 0)];   // Mutanten und Mireok
    else smonster=MonsterArrayohneFlug[13];  // Mutant
}   

if(Equal(smonster, "Zufall6"))  // intelligente starke Feinde
{
    if(!ikeinflug) smonster=MonsterArray[(RandomX(20,24) ?? 0)];  // predator, scorpion, hazardclonk, assasine, Wurm
    else smonster=MonsterArrayohneFlug[(RandomX(14,16) ?? 0)];   // hazardclonk, assasine, Wurm
}   

if(Equal(smonster, "Zufall"))
{
    if(!ikeinflug) smonster=MonsterArray[(RandomX(nil,24) ?? 0)];
    else smonster=MonsterArrayohneFlug[(RandomX(nil,16) ?? 0)];
}       

if(Equal(smonster, "Mutanten"))  // Mutanten haben aktuell noch ein mieses KI verhalten und treffen den spieler nicht, wenn nofirendly fire. Erst noch mit EnemyAI skript machen.
{
    if(!ikeinflug) smonster=MonsterArray[(RandomX(nil,17) ?? 0)]; // so dass mutant nicht nochmal drankommen kann
    else smonster=MonsterArrayohneFlug[(RandomX(nil,12) ?? 0)];
}   
    
if(Equal(smonster, "Monster"))
 {
  while ((ObjectCount(MOAN) ?? 0)<(iaufeinmal ?? 0))
  CreateObject(MOAN,1,1,-1);
  entferner = CreateObject(AAN_,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(Equal(smonster, "Berserker"))
 {
  while ((ObjectCount(BERA) ?? 0)<(iaufeinmal ?? 0))
  CreateObject(BERA,1,1,-1);
  entferner = CreateObject(AAN8,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(Equal(smonster, "FeuerMonster"))
 {
  while ((ObjectCount(FEAN) ?? 0)<(iaufeinmal ?? 0))
  CreateObject(FEAN,1,1,-1);
  entferner = CreateObject(AAN0,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(Equal(smonster, "Fledermaeuse"))
 {
  while ((ObjectCount(FL_N) ?? 0)<(iaufeinmal ?? 0))
  CreateObject(FL_N,1,1,-1);
  entferner = CreateObject(AA10,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(Equal(smonster, "Jumper"))
 {
  while ((ObjectCount(JUMA) ?? 0)<(iaufeinmal ?? 0))
  CreateObject(JUMA,1,1,-1);
  entferner = CreateObject(AAN6,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(Equal(smonster, "Kamikaze"))
 {
  while ((ObjectCount(KAMA) ?? 0)<(iaufeinmal ?? 0))
  CreateObject(KAMA,1,1,-1);
  entferner = CreateObject(AAN7,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(Equal(smonster, "Kraehen"))
 {
  while ((ObjectCount(K_AN) ?? 0)<(iaufeinmal ?? 0))
  CreateObject(K_AN,1,1,-1);
  entferner = CreateObject(AAN9,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(Equal(smonster, "Mutanten"))
 {
  while ((ObjectCount(MURA) ?? 0)<(iaufeinmal ?? 0))
  CreateObject(MURA,1,1,-1);
  entferner = CreateObject(AA12,1,1,-1);               
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(Equal(smonster, "Phoenixe"))
 {
  while ((ObjectCount(PH_A) ?? 0)<(iaufeinmal ?? 0))
  CreateObject(PH_A,1,1,-1);
  entferner = CreateObject(AA11,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(Equal(smonster, "Schlangen"))
 {
  while ((ObjectCount(SNAN) ?? 0)<(iaufeinmal ?? 0))
  CreateObject(SNAN,1,1,-1);
  entferner = CreateObject(AAN1,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(Equal(smonster, "Voegel"))
 {
  while ((ObjectCount(VOAN) ?? 0)<(iaufeinmal ?? 0)+3)  // hier plus 3, weil es nicht unbedingt leicht ist, jeden Vogel zu erwischen, deswegen ein paar mehr machen
  CreateObject(VOAN,1,1,-1);
  entferner = CreateObject(AAN2,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(Equal(smonster, "Moloch"))
 {
  while ((ObjectCount(MOA_) ?? 0)<(iaufeinmal ?? 0)+3)
  CreateObject(MOA_,1,1,-1);
  entferner = CreateObject(AAN5,1,1,-1);
  entferner ->~ Einstellen((ierledigen ?? 0)+6,ikill,imessage);  // molochs erledigen sich in kürzester zeit selbst, weil sie iwo gegenfliegen... also nen paar mehr erledigen
 }

else if(Equal(smonster, "Predator"))
 {
  while ((ObjectCount(PRA_) ?? 0)<(iaufeinmal ?? 0))
  CreateObject(PRA_,1,1,-1);
  entferner = CreateObject(AAN3,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(Equal(smonster, "Scorpion"))
 {
  while ((ObjectCount(SCA_) ?? 0)<(iaufeinmal ?? 0))
  CreateObject(SCA_,1,1,-1);
  entferner = CreateObject(AAN4,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(Equal(smonster, "Baeren"))
 {
  while ((ObjectCount(AN01) ?? 0)<(iaufeinmal ?? 0))
  CreateObject(AN01,1,1,-1);
  entferner = CreateObject(AA13,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }

else if(Equal(smonster, "Mireok"))
 {
  while ((ObjectCount(AN02) ?? 0)<(iaufeinmal ?? 0))
  CreateObject(AN02,1,1,-1);
  entferner = CreateObject(AA14,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }
 
 else if(Equal(smonster, "Killerwipfe"))
 {
  while ((ObjectCount(AN04) ?? 0)<(iaufeinmal ?? 0))
  CreateObject(AN04,1,1,-1);
  entferner = CreateObject(AA18,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }
 
 else if(Equal(smonster, "Schmonk"))
 {
  while ((ObjectCount(AN05) ?? 0)<(iaufeinmal ?? 0))
  CreateObject(AN05,1,1,-1);
  entferner = CreateObject(AA16,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }
 
 else if(Equal(smonster, "Schroffel"))
 {
  while ((ObjectCount(AN06) ?? 0)<(iaufeinmal ?? 0))
  CreateObject(AN06,1,1,-1);
  entferner = CreateObject(AA17,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }
 
 else if(Equal(smonster, "Clonk"))
 {
  while ((ObjectCount(CLAN) ?? 0)<(iaufeinmal ?? 0))
  CreateObject(CLAN,1,1,-1);
  entferner = CreateObject(AA19,1,1,-1);     
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }
 
 else if(Equal(smonster, "Ritter"))
 {
  while ((ObjectCount(RI_N) ?? 0)<(iaufeinmal ?? 0))
  CreateObject(RI_N,1,1,-1);
  entferner = CreateObject(AA21,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }
 
 else if(Equal(smonster, "Magier"))
 {
  while ((ObjectCount(MA_N) ?? 0)<(iaufeinmal ?? 0))
  CreateObject(MA_N,1,1,-1);
  entferner = CreateObject(AA20,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }
 
 else if(Equal(smonster, "Hazard"))
 {
  while ((ObjectCount(HZ_N) ?? 0)<(iaufeinmal ?? 0))
  CreateObject(HZ_N,1,1,-1);
  entferner = CreateObject(AA22,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }
 
  else if(Equal(smonster, "Assasine"))
 {
  while ((ObjectCount(AS_N) ?? 0)<(iaufeinmal ?? 0))
  CreateObject(AS_N,1,1,-1);
  entferner = CreateObject(AA23,1,1,-1);
  entferner ->~ Einstellen(ierledigen,ikill,imessage);
 }
 
   else if(Equal(smonster, "Wurm"))
 {
  if ((iaufeinmal ?? 0)>=2) iaufeinmal = (iaufeinmal ?? 0)/2; // zuviele wurmer aufeinmal sind zu krass, deswegen halbieren.  spaeter evlt mal die zahl der wurmer por gebaude auf 1 setzen in wurmangriff
  while ((ObjectCount(AN03) ?? 0)<(iaufeinmal ?? 0))
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
    if((GetTemperature() ?? 0)<10) aKatastrophen = ["Erdbeben","Eissturm","Blitze","Eissturm","Eissturm","Gewitter","Quelle","Sauerevulkan"]; // eissturm wahrscheinlicher, wenn unter 5 grad
    if(Equal(skatastrophe, "Zufall"))
    {
        skatastrophe=aKatastrophen[(RandomX(nil,7) ?? 0)];
    }
    
    var dauer;
    if(!idauer) dauer = 1000; // wenn nicht angegeben, dann immer 1000 Frames lang (ca. 30 sekunden) 
    else dauer = idauer;
    var staerke = istaerke;
    if(!staerke) staerke=RandomX(1,10); // wenn nicht angegeben, zufällige Staerke
    if(Equal(skatastrophe, "Gewitter"))
    {
        DynMessage(Format("%s der Stufe %v!",skatastrophe,staerke),nil,24,RGBa(204,203,245,nil));
        if(FindObject(WOL_)) // wenn keine wolken erlaubt, dann nur Blitze und regen
        {
            ScheduleKatastrophenAufruf("lBlitze", (staerke ?? 0)*5, dauer); // Chance pro Frame: staerke*5 von 3700, bei etwa 37 Frames pro Sekunde.
            ScheduleKatastrophenAufruf("Regenxyz", (staerke ?? 0)*2, dauer); // Ein Tropfen je Einheit staerke*2 pro Frame, ueber die Landschaft verteilt.
        }
        else
        {
            Regenwolkexyz((staerke ?? 0)*10,dauer,(staerke ?? 0)*20);
        }
    }
    else if(Equal(skatastrophe, "Blitze"))
    {
        DynMessage(Format("%s der Stufe %v!",skatastrophe,staerke),nil,24,RGBa(204,203,245,nil));
        ScheduleKatastrophenAufruf("lBlitze", (staerke ?? 0)*5, dauer);
    }
    else if(Equal(skatastrophe, "Erdbeben"))
    {
        DynMessage(Format("%s der Stufe %v!",skatastrophe,staerke),nil,24,RGBa(204,203,245,nil));
        ScheduleKatastrophenAufruf("lErdbeben", (staerke ?? 0)*2, dauer); // Wie bei Blitzen wird die Wahrscheinlichkeit pro Frame ausgewertet.
    }
    else if(Equal(skatastrophe, "Vulkan"))
    {
        DynMessage(Format("%s der Stufe %v!",skatastrophe,staerke),nil,24,RGBa(204,203,245,nil));
        // Schedule(Format("lVulcan(%v,%v,%v)",staerke*3,staerke*5,"Zufall"),1, dauer);   // ein ausbruch reicht, daher deaktivert
        Vulcanxyz(nil,"Zufall",(staerke ?? 0)*5);
    }
    else if(Equal(skatastrophe, "Meteoriten"))
    {
        DynMessage(Format("%s der Stufe %v!",skatastrophe,staerke),nil,24,RGBa(204,203,245,nil));
        ScheduleKatastrophenAufruf("lMeteoriten", (staerke ?? 0)*5, dauer);
    }
    else if(Equal(skatastrophe, "Eissturm")) // Eissturm, mit eismeteoriten
    {
        DynMessage(Format("%s der Stufe %v!",skatastrophe,staerke),nil,24,RGBa(204,203,245,nil));
        Eismeteoritenxyz(staerke, 1, dauer); // unabhängig von Temperatur (temperatur nicht ändern, weil sonst gewitter aufziehen)
        if((GetTemperature() ?? 0)<0) ScheduleKatastrophenAufruf("Regenxyz", (staerke ?? 0)*2, dauer); // schnee/eisregen nur, wenn es kalt genug ist.
    }
    else if(Equal(skatastrophe, "Quelle")) // Wasser oder Öl sprudeln als vulkan iwo in der erde
    {
        DynMessage(Format("%s der Stufe %v!",skatastrophe,staerke),nil,24,RGBa(204,203,245,nil));
        var aMat = ["Water","Oil"];
        Vulcanxyz(nil,"Zufall",(staerke ?? 0)*5,aMat[(RandomX(nil,1) ?? 0)]);
    }
    else if(Equal(skatastrophe, "Sauerevulkan"))
    {
        DynMessage(Format("%s der Stufe %v!",skatastrophe,staerke),nil,24,RGBa(204,203,245,nil));
        Vulcanxyz(nil,"Zufall",(staerke ?? 0)*5,"Acid");
    }
    else 
        Log(Format("ungueltiger Katastrophen Name: %v",skatastrophe));
}

global func ScheduleKatastrophenAufruf(string funktion, int wert, int dauer)
{
    wert ??= 0;
    dauer ??= 0;
    if (this()) return(ScheduleCall(this(), "Sammlung42KatastrophenTimer", 1, dauer, funktion, wert));
    return(Schedule(Format("%s(%v)", funktion, wert), 1, dauer));
}

// ScheduleCall sucht die Funktion im Skript des Zielobjekts.
private func Sammlung42KatastrophenTimer(string funktion, int wert)
{
    wert ??= 0;
    if (!!SEqual(funktion, "lBlitze")) return(lBlitze(wert));
    if (!!SEqual(funktion, "lErdbeben")) return(lErdbeben(wert));
    if (!!SEqual(funktion, "lMeteoriten")) return(lMeteoriten(wert));
    if (!!SEqual(funktion, "Regenxyz")) return(Regenxyz(wert));
}

global func lBlitze(ichance) 
{
    if((RandomX(1,3700) ?? 0)<=(ichance ?? 0))  Blitzexyz(); // 3700, weil es pro sekunde mit einer chance von ichance Prozent gemacht werden soll, es aber jeden frame aufgerufen wird und eine sekunde ca 37frames sind.
}

global func lErdbeben(ichance) 
{
    if((RandomX(1,3700) ?? 0)<=(ichance ?? 0))  Erdbebenxyz();
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
    if((RandomX(1,3700) ?? 0)<=(ichance ?? 0))  Meteoritenxyz();
}


