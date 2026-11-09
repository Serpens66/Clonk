#strict

  static globalpollution,globalpollutionanzeige;
  static globalnature;
  static startnature;
  static globalmeeresspiegel;
  static globalklima,stufee0,stufee1,stufee2,stufee3,stufee4,stufee5;
  static bliitzliiicht;

local erledigt,zeitgeber,gewitterzahl,tempe1,tempe2,startnature2, player1, player2, player3, player4, player5, player6, player7, player8, player1x, player2x, player3x, player4x, player5x, player6x, player7x, player8x, gesamtwaste,baeume, xPos, yPos,meer1,meer2,meer3,meer4,meer5,startklima, playerfh1, playerfh2, playerfh3, playerfh4, playerfh5, playerfh6, playerfh7, playerfh8, playerzf1, playerzf2, playerzf3, playerzf4, playerzf5, playerzf6, playerzf7, playerzf8;
local cloudLava,cloudAsche, cloudAcid, cloudSand,ohnewolkegewitter,ohnewolkegewitterzahler;
local aScenMeeresStufen;

global func Pollute()
{
  if(!ObjectCount(_Z2A))
    CreateObject(_Z2A,15,15,-1);
  /* Kein besitzer? */
//  var owner=GetOwner();
//  if(owner==-1) return(globalpollution+=pollstrength);
  /* Negativer Wert? -> volle "Heilung" */
//  if(pollstrength<0) return(globalpollution+=pollstrength);

  return(globalpollution);
}

//bisher nur bei atombombenexplosion genutzt.
global func DecNature(faktor)
{
  var i=1;
  if(faktor) i=faktor;
  globalnature+=i;
  return(globalnature);
}

// soll bei einer teuren Forschung noch genutzt werden, um atomschäden zu bereinigen
//global func IncNature(faktor)
//{
//  var i=1;
//  if(faktor) i=faktor;
//  globalnature+=i;
//  return(globalnature);
//}



protected func Initialize()
{
  SetPosition(1,1);

  return(1);
}

func Meeresspiegel()
{
    //StartMeeresspiegel
      var object;
      for(object in FindObjects(Find_ID(DRAI)))
    {yPos=GetY(object);}  // davon ausgehend, dass alle DRAI Objekte auf derselben Höhe liegen!

    aScenMeeresStufen = GameCall("ScenarioMeeresStufen"); // eine liste der Form [["Stufe1",5],["Stufe2",15]["Stufe3",25],["Stufe4",30],["Stufe5",40]] // je nach szenario Vorgaben den Meeresspiegel bei verschmutzung erhoehen
    if (!aScenMeeresStufen) aScenMeeresStufen=0;
}

func Klima()
{
//Start Klima
startklima= GetClimate();
}



global func ForestChange()
{

  var graphics="",bDoKill,deathDef,oldDef,oldGraph,feuer,bDoKill2,temperaturjetzt,jahreszeitjetzt;
temperaturjetzt=GetTemperature();
jahreszeitjetzt= GetSeason();
//reihenfolge der graphics setzung ist entscheidend
  /* Jahreszeiten */
if(!FindObject2(Find_ID(SES_)))  //Regel für Jahreszeiten deaktivieren
{ 
//   deaktiviert, weil wenn unter 0°C immer schnee.
//  if((jahreszeitjetzt>=87 || jahreszeitjetzt<12)){
//     graphics="Winter";    }

//Frühling
  if(jahreszeitjetzt>=12 && jahreszeitjetzt<37 && temperaturjetzt>0){
     graphics="Spring";  }

//Sommer
  if(jahreszeitjetzt>=37 && jahreszeitjetzt<62 && temperaturjetzt>0){
     graphics="";  }

//Herbst
  if(jahreszeitjetzt>=62 && jahreszeitjetzt<87 && temperaturjetzt>0){
     graphics="Herbst";  }

//Winter, Jahreszeit egal, immer wenn unter Null Grad, dann Schnee und Wintergrafiken
//unter umweltverschmutzung gelegt
}

  /* Umweltverschmutzung */
if(!FindObject2(Find_ID(UMG_))) // Regel für Umweltverschmutzung deaktivieren
{
if (globalpollution<globalnature*70/100) {bDoKill=0;feuer=0;bDoKill2=0;}

if (globalpollution>globalnature*50/100 && globalpollution<=globalnature*70/100 && temperaturjetzt>0)
    {graphics="Krank";}

if (globalpollution>globalnature*70/100 && globalpollution<=globalnature*100/100)  {bDoKill=true;graphics="Krank";}
}

// hier jetzt der Winter, da auch wenn krank, soll schnee bei unter 0 grad draufliegen
if(!FindObject2(Find_ID(SES_)))  //Regel für Jahreszeiten deaktivieren
{
//Winter, Jahreszeit egal, immer wenn unter Null Grad, dann Schnee und Wintergrafiken
  if(temperaturjetzt<=0) 
	 graphics="Winter";
}


if(FindObject2(Find_ID(KLI_)))  // Regel für tödliche Temperaturen
{
// wenn zu heiß, sterben die Bäume und Baumbrandgefahr
  if(temperaturjetzt>80) 
{bDoKill2=true;feuer=true;}
}


if(!FindObject2(Find_ID(UMG_)) && !FindObject2(Find_ID(SES_))) // Gras und Baumänderungen, wenn mind. eine der beiden Regeln nicht aktiv ist, Umweltverschmutzung oder Jahreszeiten
{ 
//Grasänderungen
  for(var gras in FindObjects(Find_Func("IsBulletGrasTarget")))
  {
    if(!Random(40))
      if(!gras->~ChangeGraphic(graphics))
        FadeGraphics(graphics,gras);
// ImFrühling auf Phase 2, also Blumen setzen
    if(!Random(33) && jahreszeitjetzt>=12 && jahreszeitjetzt<37 && temperaturjetzt>0) 
    SetPhase(2,gras);
// wenn nicht Frühling, dann keine Blumen, sondern nur Phase 0bis1
 if(!Random(166) && (jahreszeitjetzt<12 || jahreszeitjetzt>37)) 
      SetPhase(Random(2),gras);
}

// Baumänderungen
  for(var baum in FindObjects(Find_Or( Find_Func("IsTree"),Find_ID(TR33) )))
  {
    if(!Random(33))
      if(!baum->~ChangeGraphic(graphics))
        FadeGraphics(graphics,baum);
    // Bäume töten
    if(bDoKill && GetID(baum)!=CACT && GetID(baum)!=CAC2)
      if(!Random(33))
      {
        deathDef=GetDefBurnTo(GetID(baum));
        oldDef=GetID(baum);
        oldGraph=GetGraphics(baum);
        if(deathDef)
        {
          Schedule(Format("ChangeDef(%i)",deathDef),35,1,baum);
          ScheduleCall(baum,"Laub",1,35);
          SetGraphics(oldGraph,baum,oldDef);
          FadeGraphics("",baum,deathDef,true);
        }
      }

    // Bäume verdursten
    if(bDoKill2 && GetID(baum)!=CACT && GetID(baum)!=CAC2)
      if(!Random(266))
      {
        deathDef=GetDefBurnTo(GetID(baum));
        oldDef=GetID(baum);
        oldGraph=GetGraphics(baum);
        if(deathDef)
        {
          Schedule(Format("ChangeDef(%i)",deathDef),35,1,baum);
          ScheduleCall(baum,"Laub",1,35);
          SetGraphics(oldGraph,baum,oldDef);
          FadeGraphics("",baum,deathDef,true);
        }
      }


   //Baumbrand Gefahr
  if(feuer && GetID(baum)!=CACT && GetID(baum)!=CAC2)
   if(!Random(3333))
   {Incinerate(baum);}
	}
}

  /* Abschliessen */
  return(1);
}




// wenn ein Clonk stirbt, kann ein neuer dafür nachgekauft werden. Dadurch, und durch die im Szenario eingestelle kaufmenge, soll garantiert werden, dass jeder immer maximal eine Menge x an Clonks haben kann, die Kaufmenge beim Tod von Clonks aber niemals erschöpft ist.
func OnClonkDeath(object pClonk, int iKiller) {
  DoHomebaseMaterial(GetOwner(pClonk), GetID(pClonk), 1);
//Log("neuer clonk sollte kaufbar sein");
return _inherited(pClonk, iKiller);
}



public func CheckPollution()
{

/* Immer notwendige Features */

   /* LaubPartikel bewegen */
  if(!Random(10))
  PushParticles("Laub",GetWind()/20+RandomX(-5,6),0);
  if(!Random(30))  
   PushParticles("Laub",0,RandomX(-10,0));


//die Blitzlichtpartikel entfernen, wenn welche von wolken erstellt, globale variable. Wenn keine da, dann stört das nicht, deswegen dalassen.
if(bliitzliiicht==1)
if(!Random(1))
{ClearParticles("PSpark");bliitzliiicht=0;}

// nur einmal zu anfang:
if (erledigt==0)
{
startnature2 = HashGet(hashSpielRegeln, "Umweltbelastbarkeit"); //werte pro Spieler

//globalnature der Spielerzahl anpassen
globalnature=startnature2*GetPlayerCount(C4PT_User);
//Log("sollte angepasst sein");
startnature=globalnature;
Log("globalnature gesetzt auf: %v", globalnature);

Meeresspiegel();
Klima();
InitResearchAcc();  //Arraysystem für die Kosten des Forschens
erledigt=1;
};


  /* Tödliches Klima */ 


//Gewitter bei starker Temp Änderung, evtl 
//weil Temp am Spielstart sich immer stark ändert, folgende Bedingung:
if (FrameCounter()>=3000)
{
//wenn die Temperaturänderung innerhalb von 500Frames größer als 10 ist, dann gibts Gewitter
if (gewitterzahl<=166)
{++gewitterzahl;
if(gewitterzahl==1) {tempe1=GetTemperature ();tempe2=GetTemperature ();}
}

if(gewitterzahl>166)
{tempe2=GetTemperature ();
gewitterzahl=0;}

if(Abs(tempe1-tempe2) >= 10)
{
Log("ein Gewitter zieht auf");

// # hier bei AntiLagRegel keine Wolke, sondern einfach nur Blitze!!
if(!(ObjectCount(WOL_)))
{
Regenwolkexyz(10,RandomX(1000,4000),RandomX(20,100));Regenwolkexyz(10,RandomX(1000,4000),RandomX(20,100)); }
//zwei regenwolken mit wenig regen, die 1000-4000Frames hält und zufällige Gewitterstufe hat
else{ohnewolkegewitter=1;}
}
}

//ohnewolkegewitter ausführen
if(ohnewolkegewitter==1)
{
++ohnewolkegewitterzahler;
if(ohnewolkegewitterzahler>=1000) {ohnewolkegewitter=0;ohnewolkegewitterzahler=0;}
if(!Random(100)) Blitzexyz();
}



//Klimaschaden
// wenn Temperatur zwischen -30°C und -40°C geht gibt es eisigen Wind der den Clonks schadet 

var jetzigetemperatur;
jetzigetemperatur=GetTemperature();

if(FindObject2(Find_ID(KLI_)))  // nur wenn Regel aktiv ist!
{

if(jetzigetemperatur<-50 && jetzigetemperatur>-60)
 {
 DynMessage("Kältewarnung! 1",0,16,RGBa(31,209,213,0));
 }

if(jetzigetemperatur<=-60 && jetzigetemperatur>=-80) 
 {
     DynMessage("Kältewarnung! 2,5",0,16,RGBa(31,209,213,0));
// Erstellt/entfernt ab und zu die Eismeteoritenregel, die zahl ist die Stärke.
if(!Random(333)) Entferneeismeteoritenxyz();
if(!Random(1000)) 
   {
Entferneeismeteoritenxyz();Eismeteoritenxyz(RandomX(1,8));
   }


// Kältedingens, muss mindestens alle 100Frames neu aufgerufen werden.
for( var target in FindObjects(Find_OCF(OCF_CrewMember), Find_NoContainer(),Find_Not(Find_Func("IsAI")) ))
   {
//wenn die Funktion Klimaschutz im clonk true zurückliefert, dann kein Ziel,1 steht für Kälte, 2 für Hitze
if(target->~Klimaschutz(1)) continue;
if(!FindObject(CL22,0,0,0,0,0,"WarnCold",target))
      {
var zeichen = CreateObject(CL22,0,0,-1);
zeichen->~Warning(target,1); 
       } // kältefarbe, das ziel und der zu machende schaden wird übergeben
   }
 }

// wenn Temperatur unter -40°C geht gibt es eisigen Wind der den Clonks mehr schadet und sie werden ab und zu eingefroren. 
if(jetzigetemperatur<-80) {
DynMessage("Kältewarnung! 4",0,16,RGBa(31,209,213,0));
// Erstellt/entfernt ab und zu die Eismeteoritenregel, die zahl ist die Stärke.
if(!Random(500)) Entferneeismeteoritenxyz();
if(!Random(666)) {Entferneeismeteoritenxyz();Eismeteoritenxyz(RandomX(1,10));}

// Kältedingens, muss mindestens alle 100Frames neu aufgerufen werden.
for( var target in FindObjects(Find_OCF(OCF_CrewMember), Find_NoContainer(),Find_Not(Find_Func("IsAI")) ))
{
//wenn die Funktion Klimaschutz im clonk true zurückliefert, dann kein Ziel
if(target->~Klimaschutz(1)) continue;
//nur kälteschaden, wenn nicht bereits vorhanden, oder eingefroren
if(!FindObject(CL22,0,0,0,0,0,"WarnCold",target) && !FindObject(IC22,0,0,0,0,0,"Freeze",target)){
var  zeichen = CreateObject(CL22,0,0,-1);
zeichen->~Warning(target,2);}  // kältefarbe, das ziel und der zu machende schaden wird übergeben

//ab und zu einfrieren, wenn noch nicht eingefroren
if(!FindObject(IC22,0,0,0,0,0,"Freeze",target))
if(!Random(533)){var eisblock;
ObjectSetAction(eisblock=CreateObject(IC22,GetX(target),GetY(target),-1),"Freeze",target);
    Extinguish(target);
    AddVertex(GetVertex(0,0,target),GetVertex(0,1,target),eisblock);}
}}

//HITZE
if(jetzigetemperatur>60 && jetzigetemperatur<70)
 {
 DynMessage("Hitzewarnung! 1",0,16,RGBa(208,105,36,0));
 }
// wenn Temperatur zwischen 35°C und 40C° geht ist es so heiß, dass Clonks etwas schaden nehmen
if(jetzigetemperatur>=70 && jetzigetemperatur<=80) {
    DynMessage("Hitzewarnung! 2",0,16,RGBa(208,105,36,0));
// Hitzedingens, muss mindestens alle 100Frames neu aufgerufen werden.
for( var target in FindObjects(Find_OCF(OCF_CrewMember), Find_NoContainer(),Find_Not(Find_Func("IsAI")) ))
{
//wenn die Funktion Klimaschutz im Clonk true zurückliefert, dann kein Ziel
if(target->~Klimaschutz(2)) continue;
if(!FindObject(CL22,0,0,0,0,0,"WarnColdX",target)) {
var  zeichen = CreateObject(CL22,0,0,-1);
zeichen->~WarningX(target,1); } // hitzefarbe, das ziel und der zu machende schaden wird übergeben
}}

// wenn Temperatur über 40C° geht ist es so heiß, dass Clonks mehr schaden nehmen
if(jetzigetemperatur>80 && jetzigetemperatur<90) {
    DynMessage("Hitzewarnung! 3",0,16,RGBa(208,105,36,0));
// Hitzedingens, muss mindestens alle 100Frames neu aufgerufen werden.
for( var target in FindObjects(Find_OCF(OCF_CrewMember), Find_NoContainer(),Find_Not(Find_Func("IsAI")) ))
{
//wenn die Funktion Klimaschutz im Clonk true zurückliefert, dann kein Ziel
if(target->~Klimaschutz(2)) continue;
if(!FindObject(CL22,0,0,0,0,0,"WarnColdX",target)) {
 var zeichen = CreateObject(CL22,0,0,-1);
zeichen->~WarningX(target,2); } // hitzefarbe, das ziel und der zu machende schaden wird übergeben
}}


// wenn Temperatur über 45C° geht ist es so heiß, dass Clonks viel schaden nehmen
if(jetzigetemperatur>=90) {
    DynMessage("Hitzewarnung! 4",0,16,RGBa(208,105,36,0));
// Hitzedingens, muss mindestens alle 100Frames neu aufgerufen werden.
for( var target in FindObjects(Find_OCF(OCF_CrewMember), Find_NoContainer(),Find_Not(Find_Func("IsAI")) ))
{
//wenn die Funktion Klimaschutz im Clonk true zurückliefert, dann kein Ziel
if(target->~Klimaschutz(2)) continue;
if(!FindObject(CL22,0,0,0,0,0,"WarnColdX",target)) {
 var zeichen = CreateObject(CL22,0,0,-1);
zeichen->~WarningX(target,3); } // hitzefarbe, das ziel und der zu machende schaden wird übergeben
}}

}




// nur alle 5 durchläufe, also alle 15 frames luftverschmutzungsgrad aktualsisieren
if(!Random(5)) 
{
  /* Luftreinigung samt Grenzhaltung der Werte */ 
FilterAir();
  /* Luftverschmutzung durch Müll und Industriegebäude */
 Waste();

//globalpollution berechnen (statischer wert)

globalpollution=gesamtwaste-baeume;
globalpollutionanzeige = gesamtwaste-baeume; // dieser wert dient der Anzeige und kann kleiner null oder größer globalnature sein, damit man sieht, wieviel einem noch genau fehlt.
  CheckLimits();
}


// es könnte allgemein Probleme geben, wenn durch Laufzeit Beitritte/Austritte der Index der Spieler über 8 hinausgeht,bzw. der index 7 für spieler 8 vergeben wird, es aber keine 8 spieler gibt.

/*   funktioniert so nicht
// 5 Kaufbare Hazardclonks, sobald zukunftsFundament erforscht:
if(GetPlrKnowledge(0,B556) && playerzf1==0) {playerzf1=1;DoHomebaseMaterial(0,HZCK,5);}
if(GetPlrKnowledge(1,B556) && playerzf2==0) {playerzf2=1;DoHomebaseMaterial(1,HZCK,5);}
if(GetPlrKnowledge(2,B556) && playerzf3==0) {playerzf3=1;DoHomebaseMaterial(2,HZCK,5);}
if(GetPlrKnowledge(3,B556) && playerzf4==0) {playerzf4=1;DoHomebaseMaterial(3,HZCK,5);}
if(GetPlrKnowledge(4,B556) && playerzf5==0) {playerzf5=1;DoHomebaseMaterial(4,HZCK,5);}
if(GetPlrKnowledge(5,B556) && playerzf6==0) {playerzf6=1;DoHomebaseMaterial(5,HZCK,5);}
if(GetPlrKnowledge(6,B556) && playerzf7==0) {playerzf7=1;DoHomebaseMaterial(6,HZCK,5);}
if(GetPlrKnowledge(7,B556) && playerzf8==0) {playerzf8=1;DoHomebaseMaterial(7,HZCK,5);}


// sorg dafür dass immer x 5er Goldstücke zu kaufen sind
if(GetHomebaseMaterial(0,GOL5)==0) {DoHomebaseMaterial(0,GOL5,5);}
if(GetHomebaseMaterial(1,GOL5)==0) {DoHomebaseMaterial(1,GOL5,5);}
if(GetHomebaseMaterial(2,GOL5)==0) {DoHomebaseMaterial(2,GOL5,5);}
if(GetHomebaseMaterial(3,GOL5)==0) {DoHomebaseMaterial(3,GOL5,5);}
if(GetHomebaseMaterial(4,GOL5)==0) {DoHomebaseMaterial(4,GOL5,5);}
if(GetHomebaseMaterial(5,GOL5)==0) {DoHomebaseMaterial(5,GOL5,5);}
if(GetHomebaseMaterial(6,GOL5)==0) {DoHomebaseMaterial(6,GOL5,5);}
if(GetHomebaseMaterial(7,GOL5)==0) {Log("für spieler 8 gold erstellt");DoHomebaseMaterial(7,GOL5,5);}
*/

// Forschungen kontrollieren, reicht auch alle 5 Durchgänge, also 15 Frames
if(!Random(5))
{

//Umwelt stärken
if(GetPlrKnowledge(0,_BAL) && player1==0) {player1=1;Natureverbesserung();}
if(GetPlrKnowledge(1,_BAL) && player2==0) {player2=1;Natureverbesserung();}
if(GetPlrKnowledge(2,_BAL) && player3==0) {player3=1;Natureverbesserung();}
if(GetPlrKnowledge(3,_BAL) && player4==0) {player4=1;Natureverbesserung();}
if(GetPlrKnowledge(4,_BAL) && player5==0) {player5=1;Natureverbesserung();}
if(GetPlrKnowledge(5,_BAL) && player6==0) {player6=1;Natureverbesserung();}
if(GetPlrKnowledge(6,_BAL) && player7==0) {player7=1;Natureverbesserung();}
if(GetPlrKnowledge(7,_BAL) && player8==0) {player8=1;Natureverbesserung();}

//Freier Händler
if (ObjectCount2(Find_ID(SAIT))<=4)
{
if(GetPlrKnowledge(0,ERIT) && playerfh1==0) {playerfh1=1;CreateObject(SAIT,1,1,-1);}
if(GetPlrKnowledge(1,ERIT) && playerfh2==0) {playerfh2=1;CreateObject(SAIT,1,1,-1);}
if(GetPlrKnowledge(2,ERIT) && playerfh3==0) {playerfh3=1;CreateObject(SAIT,1,1,-1);}
if(GetPlrKnowledge(3,ERIT) && playerfh4==0) {playerfh4=1;CreateObject(SAIT,1,1,-1);}
if(GetPlrKnowledge(4,ERIT) && playerfh5==0) {playerfh5=1;CreateObject(SAIT,1,1,-1);}
if(GetPlrKnowledge(5,ERIT) && playerfh6==0) {playerfh6=1;CreateObject(SAIT,1,1,-1);}
if(GetPlrKnowledge(6,ERIT) && playerfh7==0) {playerfh7=1;CreateObject(SAIT,1,1,-1);}
if(GetPlrKnowledge(7,ERIT) && playerfh8==0) {playerfh8=1;CreateObject(SAIT,1,1,-1);}
}

}


ForestChange();

//Stufen
if (globalpollution<=globalnature*40/100) Stufe0();
if (globalpollution>globalnature*40/100 && globalpollution<=globalnature*50/100) Stufe1(); 
if (globalpollution>globalnature*50/100 && globalpollution<=globalnature*60/100) Stufe2(); 
if (globalpollution>globalnature*60/100 && globalpollution<=globalnature*70/100) Stufe3(); 
if (globalpollution>globalnature*70/100 && globalpollution<globalnature*100/100) Stufe4(); 
if (globalpollution==globalnature*100/100) Stufe5(); 


  return(1);
}
















func Stufe0()
{
if (stufee0==0){
//Meeresspiegel normalisieren
Water();SetClimate(startklima);
Message("Keine Umweltbelastung, alles im grünen Bereich");stufee0=1;stufee1=0;stufee2=0;stufee3=0;stufee4=0;stufee5=0;ClearParticles("Nebel2"); RemoveAll(VM9Z);if(cloudAcid) RemoveObject(cloudAcid); if(cloudLava) RemoveObject(cloudLava);if(cloudAsche) RemoveObject(cloudAsche);};




}


func Stufe1()
{
if (stufee1==0) {Log("Stufe1");Water();SetClimate(startklima+3);Message("Leichte Umweltbelastung Stufe1, Meeresspiegel klein wenig erhöht, startklima+3"); stufee0=0;stufee1=1;stufee2=0;stufee3=0;stufee4=0;stufee5=0;ClearParticles("Nebel2"); RemoveAll(VM9Z);if(cloudAcid) RemoveObject(cloudAcid); if(cloudLava) RemoveObject(cloudLava);if(cloudAsche) RemoveObject(cloudAsche);}



}


func Stufe2()
{
if (stufee2==0) {Log("Stufe2");Water();SetClimate(startklima+6);Message("Umweltbelastung Stufe2; die Bäume werden krank, wenn es schlimmer wird, könnten sie absterben; Leichter Smog behindert die Sicht; Meeresspiegel leicht erhöht; startklima+6"); stufee0=0;stufee1=0;stufee2=1;stufee3=0;stufee4=0;stufee5=0;if(cloudAcid) RemoveObject(cloudAcid); if(cloudLava) RemoveObject(cloudLava);if(cloudAsche) RemoveObject(cloudAsche);}

//Verschmutzungsnebel, nur wenn nicht deaktiviert

if(!FindObject2(Find_Or(Find_ID(UMG_),Find_ID(VSCH)))) 
{
  if(ObjectCount2(Find_ID(VM9Z))>1)  {ClearParticles("Nebel2"); RemoveAll(VM9Z);}
if(ObjectCount2(Find_ID(VM9Z))<1) {CreateObject(VM9Z);}
}

}


func Stufe3()
{
if (stufee3==0) {Log("Stufe3");Water();SetClimate(startklima+12);Message("Umweltbelastung Stufe3;Mittelstarker Smog;Es regnet Asche; Um Stufe 4 (Baumsterben) zu verhindern sollten Bäume gepflanzt, Filteranlagen gebaut und Müll/Industriegebäude beseitigt werden; Meeresspiegel erhöht; startklima+12"); stufee0=0;stufee1=0;stufee2=0;stufee3=1;stufee4=0;stufee5=0;if(cloudAcid) RemoveObject(cloudAcid); if(cloudLava) RemoveObject(cloudLava);}

Ascheregenxyz();Ascheregenxyz();Ascheregenxyz();

//Verschmutzungsnebel
if(!FindObject2(Find_Or(Find_ID(UMG_),Find_ID(VSCH)))) 
{
  if(ObjectCount2(Find_ID(VM9Z))>4) {ClearParticles("Nebel2"); RemoveAll(VM9Z);  CreateObject(VM9Z);CreateObject(VM9Z);CreateObject(VM9Z);CreateObject(VM9Z);}
if(ObjectCount2(Find_ID(VM9Z))<4) {CreateObject(VM9Z);}
}


// wenn "keine wolken" aktiv ist, dann keine wolken
if(!FindObject(WOL_))
 {
if(!cloudAsche) {
//Ascheregenwolke.. erstmal nur eine, weil ich noch nicht weiß, wie ich mehrere am besten entferne
var blitze=0;
var ende = 0;
 var strenght = RandomX(40,100);
  var iMat= Material("Ashes");
  cloudAsche = CreateObject(FX22,Random(LandscapeWidth()),0,NO_OWNER());
  if(!cloudAsche) return(0);
  cloudAsche-> Activate(iMat,LandscapeWidth(),strenght,blitze);}
 }
}

func Stufe4()
{
if (stufee4==0) {Log("Stufe4");Water();SetClimate(startklima+24);Message("Umweltbelastung Stufe4;Bäume sterben und filtern nur noch sehr schwach die Luft, wodurch die Verschmutzung ansteigt; Es regnet Säure und starker Smog bedeckt die Sicht;einige Katastrophen;Stufe5 sollte unbedingt verhindert werden! ; Meeresspiegel stark erhöht; startklima+24"); stufee0=0;stufee1=0;stufee2=0;stufee3=0;stufee4=1;stufee5=0;if(cloudLava) RemoveObject(cloudLava); if(cloudAsche) RemoveObject(cloudAsche);}
Saureregenxyz();Saureregenxyz();Saureregenxyz();
if(!Random(166)) Blitzexyz();
if(!Random(666)) Erdbebenxyz();
if(!Random(116)) Meteoritenxyz();
if(!Random(333)) Vulcanxyz();

//Verschmutzungsnebel
   if(!FindObject2(Find_Or(Find_ID(UMG_),Find_ID(VSCH)))) 
{
  if(ObjectCount2(Find_ID(VM9Z))>7)  {ClearParticles("Nebel2"); RemoveAll(VM9Z);  CreateObject(VM9Z);CreateObject(VM9Z);CreateObject(VM9Z);CreateObject(VM9Z);CreateObject(VM9Z);CreateObject(VM9Z);CreateObject(VM9Z);}
if(ObjectCount2(Find_ID(VM9Z))<7) {CreateObject(VM9Z);}
}

// wenn "keine wolken" aktiv ist, dann keine wolken
if(!FindObject(WOL_))
 {
if(!cloudAcid) {
//Acidregenwolke.. erstmal nur eine, weil ich noch nicht weiß, wie ich mehrere am besten entferne
var blitze=0;
var ende = 0;
 var strenght = RandomX(40,100);
  var iMat= Material("Acid");
  cloudAcid = CreateObject(FX22,Random(LandscapeWidth()),0,NO_OWNER());
  if(!cloudAcid) return(0);
  cloudAcid-> Activate(iMat,LandscapeWidth(),strenght,blitze);}
 }
}


func Stufe5()
{ 
if (stufee5==0) {Log("Stufe5");Water();SetClimate(startklima+40);Message("Umweltbelastung Stufe5; Jetzt heißt es nur noch überleben.Sehr starker Smog bedeckt die Sicht, Es regnet Lava und Meteoriten, heftige Gewitter, Erdbeben und Vulkanausbrüche sind an der Tagesordnung; Das einzig positive ist, dass dadurch möglicherweise genug zerstört wird, um die Umweltbelastung zu senken und die Natur erholt sich wieder; Meeresspiegel sehr stark erhöht; startklima+40"); stufee0=0;stufee1=0;stufee2=0;stufee3=0;stufee4=0;stufee5=1;if(cloudAcid) RemoveObject(cloudAcid); if(cloudAsche) RemoveObject(cloudAsche);}
Lavaregenxyz();Lavaregenxyz();Lavaregenxyz();
if(!Random(66)) Blitzexyz();
if(!Random(333)) Erdbebenxyz();
if(!Random(33)) Meteoritenxyz();
if(!Random(166)) Vulcanxyz();
if(!Random(20)) CastObjects(LAVA,1,100,Random(LandscapeWidth()),0);  // Lavabrocken 

//Verschmutzungsnebel

if(!FindObject2(Find_Or(Find_ID(UMG_),Find_ID(VSCH)))) 
{
  if(ObjectCount2(Find_ID(VM9Z))>10) {ClearParticles("Nebel2"); RemoveAll(VM9Z);  CreateObject(VM9Z);CreateObject(VM9Z);CreateObject(VM9Z);CreateObject(VM9Z);CreateObject(VM9Z);CreateObject(VM9Z);CreateObject(VM9Z);CreateObject(VM9Z);CreateObject(VM9Z);CreateObject(VM9Z);}
if(ObjectCount2(Find_ID(VM9Z))<10) {CreateObject(VM9Z);}
}

// wenn "keine wolken" aktiv ist, dann keine wolken
if(!FindObject(WOL_))
 {
if(!cloudLava) {
//Lavaregenwolke.. erstmal nur eine, weil ich noch nicht weiß, wie ich mehrere am besten entferne
var blitze=0;
var ende = 0;
 var strenght = RandomX(40,100);
  var iMat= Material("Lava");
  cloudLava = CreateObject(FX22,Random(LandscapeWidth()),0,NO_OWNER());
  if(!cloudLava) return(0);
  cloudLava-> Activate(iMat,LandscapeWidth(),strenght,blitze);}
 }
}


public func Natureverbesserung()
{
//sorgt dafür, dass globalnature mit 2,5 Multipliziert wird, sobald alles spieler es erforscht haben (bei einzelnen Spielern ist es halt ein Bruchteil davon)
globalnature+=(((startnature*250/100)-startnature)/GetPlayerCount(C4PT_User));

//sorgt dafür, dass alle vorhandenen Bäume sich 2 mal fortpflanzen. -> keine neuen Bäume, wenn keine alten da, oder wenn kein Platz mehr ist
var obj;
for(var obj in FindObjects(Find_OCF(OCF_Chop)))
{obj->Reproduction();obj->Reproduction();};
return();
}



public func Water() // erhoehe den Meeresspiegel bei verschmutzung ( -> kleinere Y werte).  setzt den meeresspiegel auf einen wert
{
    // die Szenariospezifischen Meeresstufen sind in aScenMeeresStufen der Form [["Stufe1",5],["Stufe2",15]["Stufe3",25],["Stufe4",30],["Stufe5",40]]. Bzw. wenn es keine Vorgaben gibt, ist es 0
    // yPos ist eine lokale variable, die den Start meeresspiegel beinhaltet, sofern drai objkete vorhanden

if (globalpollution<=globalnature*40/100) // stufe 0
{gChangeMeeresspiegel(yPos,1);meer1=0;meer2=0;meer3=0;meer4=0;meer5=0;globalmeeresspiegel=yPos;};    
    
if (globalpollution>globalnature*40/100 && globalpollution<=globalnature*50/100 && meer1==0) 
{if(aScenMeeresStufen) {gChangeMeeresspiegel(yPos-aScenMeeresStufen[0][1],1);globalmeeresspiegel=yPos-aScenMeeresStufen[0][1];} else {gChangeMeeresspiegel(yPos-5,1);globalmeeresspiegel=yPos-5;}meer1=1;meer2=0;meer3=0;meer4=0;meer5=0;}  // gChangeMeeresspiegel(2,1) setzt die position aller DRAI objekte auf Y = 2. funktioniert daher nur, wenn die Environment meeresspiegel aktiv ist.

if (globalpollution>globalnature*50/100 && globalpollution<=globalnature*60/100 && meer2==0) 
{if(aScenMeeresStufen) {gChangeMeeresspiegel(yPos-aScenMeeresStufen[1][1],1);globalmeeresspiegel=yPos-aScenMeeresStufen[1][1];} else {gChangeMeeresspiegel(yPos-12,1);globalmeeresspiegel=yPos-12;}meer1=0;meer2=1;meer3=0;meer4=0;meer5=0;}

if (globalpollution>globalnature*60/100 && globalpollution<=globalnature*70/100 && meer3==0) 
{if(aScenMeeresStufen) {gChangeMeeresspiegel(yPos-aScenMeeresStufen[2][1],1);globalmeeresspiegel=yPos-aScenMeeresStufen[2][1];} else {gChangeMeeresspiegel(yPos-22,1);globalmeeresspiegel=yPos-22;}meer1=0;meer2=0;meer3=1;meer4=0;meer5=0;}

if (globalpollution>globalnature*70/100 && globalpollution<globalnature*100/100 && meer4==0) 
{if(aScenMeeresStufen) {gChangeMeeresspiegel(yPos-aScenMeeresStufen[3][1],1);globalmeeresspiegel=yPos-aScenMeeresStufen[3][1];} else {gChangeMeeresspiegel(yPos-37,1);globalmeeresspiegel=yPos-37;}meer1=0;meer2=0;meer3=0;meer4=1;meer5=0;}

if (globalpollution>=globalnature*100/100 && meer5==0) 
{if(aScenMeeresStufen) {gChangeMeeresspiegel(yPos-aScenMeeresStufen[4][1],1);globalmeeresspiegel=yPos-aScenMeeresStufen[4][1];} else {gChangeMeeresspiegel(yPos-60,1);globalmeeresspiegel=yPos-60;}meer1=0;meer2=0;meer3=0;meer4=0;meer5=1;}

  return(1);
}

public func Waste()
{
//Der Müll jedes Spielers wird gesondert behandelt. Je nach dem ob "Umweltbewusstsein1 bzw 2" entwickelt wurde, oder nicht, wird der Schadenswert verringert.

var waste,temp,waste1,temp1,waste2,temp2,waste3,temp3,waste4,temp4,waste5,temp5,waste6,temp6,waste7,temp7;

  for(var obj in FindObjects(Find_Func("IsWaste"),Find_Owner(0)))
{
    temp=obj->IsWaste();
    if(GetPlrKnowledge(0,_BAO) && !(GetPlrKnowledge(0,_BA2))) 
    {
        if(VerticesStuck(obj)>=GetVertexNum(obj) && GetID(obj)!=VBWS && GetID(obj)!=_X1A) // beim verbrannter Müll objekt ist feststecken egal, und auch bei Filternanlage
          waste+=(temp/2*(200/3))/100;     
        else  waste+=(temp*(200/3))/100;
    }
    else if(GetPlrKnowledge(0,_BAO) && GetPlrKnowledge(0,_BA2))
    {
        if(VerticesStuck(obj)>=GetVertexNum(obj) && GetID(obj)!=VBWS && GetID(obj)!=_X1A) // beim verbrannter Müll objekt ist feststecken egal
          waste+=temp/4;     
        else  waste+=temp/2;
    }
    else
    {   
         if(VerticesStuck(obj)>=GetVertexNum(obj) && GetID(obj)!=VBWS && GetID(obj)!=_X1A) // beim verbrannter Müll objekt ist feststecken egal//Im Erdboden vergraben : Nicht so umweltschädigent
          waste+=temp/2;
         else  waste+=temp;
    }
};

  for(var obj in FindObjects(Find_Func("IsWaste"),Find_Owner(1)))
  {
    temp1=obj->IsWaste();
if(GetPlrKnowledge(1,_BAO) && !(GetPlrKnowledge(1,_BA2))) {if(VerticesStuck(obj)>=GetVertexNum(obj) && GetID(obj)!=VBWS && GetID(obj)!=_X1A) // beim verbrannter Müll objekt ist feststecken egal
      waste1+=(temp1/2*(200/3))/100;     
else     waste1+=(temp1*(200/3))/100;}
else if(GetPlrKnowledge(1,_BAO) && GetPlrKnowledge(1,_BA2))
{if(VerticesStuck(obj)>=GetVertexNum(obj) && GetID(obj)!=VBWS && GetID(obj)!=_X1A) // beim verbrannter Müll objekt ist feststecken egal
      waste1+=temp1/4;     
else     waste1+=temp1/2;}
else{   if(VerticesStuck(obj)>=GetVertexNum(obj) && GetID(obj)!=VBWS && GetID(obj)!=_X1A) // beim verbrannter Müll objekt ist feststecken egal//Im Erdboden vergraben : Nicht so umweltschädigent
      waste1+=temp1/2;
     else     waste1+=temp1;};
};

  for(var obj in FindObjects(Find_Func("IsWaste"),Find_Owner(2)))
  {
    temp2=obj->IsWaste();
if(GetPlrKnowledge(2,_BAO) && !(GetPlrKnowledge(2,_BA2))) {if(VerticesStuck(obj)>=GetVertexNum(obj) && GetID(obj)!=VBWS && GetID(obj)!=_X1A) // beim verbrannter Müll objekt ist feststecken egal
      waste2+=(temp2/2*(200/3))/100;     
else     waste2+=(temp2*(200/3))/100;}
else if(GetPlrKnowledge(2,_BAO) && GetPlrKnowledge(2,_BA2))
{if(VerticesStuck(obj)>=GetVertexNum(obj) && GetID(obj)!=VBWS && GetID(obj)!=_X1A) // beim verbrannter Müll objekt ist feststecken egal
      waste2+=temp2/4;     
else     waste2+=temp2/2;}
else{   if(VerticesStuck(obj)>=GetVertexNum(obj) && GetID(obj)!=VBWS && GetID(obj)!=_X1A) // beim verbrannter Müll objekt ist feststecken egal//Im Erdboden vergraben : Nicht so umweltschädigent
      waste2+=temp2/2;
     else     waste2+=temp2;};
};

  for(var obj in FindObjects(Find_Func("IsWaste"),Find_Owner(3)))
  {
    temp3=obj->IsWaste();
if(GetPlrKnowledge(3,_BAO) && !(GetPlrKnowledge(3,_BA2))) {if(VerticesStuck(obj)>=GetVertexNum(obj) && GetID(obj)!=VBWS && GetID(obj)!=_X1A) // beim verbrannter Müll objekt ist feststecken egal
      waste3+=(temp3/2*(200/3))/100;     
else     waste3+=(temp3*(200/3))/100;}
else if(GetPlrKnowledge(3,_BAO) && GetPlrKnowledge(3,_BA2))
{if(VerticesStuck(obj)>=GetVertexNum(obj) && GetID(obj)!=VBWS && GetID(obj)!=_X1A) // beim verbrannter Müll objekt ist feststecken egal
      waste3+=temp3/4;     
else     waste3+=temp3/2;}
else{   if(VerticesStuck(obj)>=GetVertexNum(obj) && GetID(obj)!=VBWS && GetID(obj)!=_X1A) // beim verbrannter Müll objekt ist feststecken egal//Im Erdboden vergraben : Nicht so umweltschädigent
      waste3+=temp3/2;
     else     waste3+=temp3;};
};

  for(var obj in FindObjects(Find_Func("IsWaste"),Find_Owner(4)))
  {
    temp4=obj->IsWaste();
if(GetPlrKnowledge(4,_BAO) && !(GetPlrKnowledge(4,_BA2))) {if(VerticesStuck(obj)>=GetVertexNum(obj) && GetID(obj)!=VBWS && GetID(obj)!=_X1A) // beim verbrannter Müll objekt ist feststecken egal
      waste4+=(temp4/2*(200/3))/100;     
else     waste4+=(temp4*(200/3))/100;}
else if(GetPlrKnowledge(4,_BAO) && GetPlrKnowledge(4,_BA2))
{if(VerticesStuck(obj)>=GetVertexNum(obj) && GetID(obj)!=VBWS && GetID(obj)!=_X1A) // beim verbrannter Müll objekt ist feststecken egal
      waste4+=temp4/4;     
else     waste4+=temp4/2;}
else{   if(VerticesStuck(obj)>=GetVertexNum(obj) && GetID(obj)!=VBWS && GetID(obj)!=_X1A) // beim verbrannter Müll objekt ist feststecken egal//Im Erdboden vergraben : Nicht so umweltschädigent
      waste4+=temp4/2;
     else     waste4+=temp4;};
};

  for(var obj in FindObjects(Find_Func("IsWaste"),Find_Owner(5)))
  {
    temp5=obj->IsWaste();
if(GetPlrKnowledge(5,_BAO) && !(GetPlrKnowledge(5,_BA2))) {if(VerticesStuck(obj)>=GetVertexNum(obj) && GetID(obj)!=VBWS && GetID(obj)!=_X1A) // beim verbrannter Müll objekt ist feststecken egal
      waste5+=(temp5/2*(200/3))/100;     
else     waste5+=(temp5*(200/3))/100;}
else if(GetPlrKnowledge(5,_BAO) && GetPlrKnowledge(5,_BA2))
{if(VerticesStuck(obj)>=GetVertexNum(obj) && GetID(obj)!=VBWS && GetID(obj)!=_X1A) // beim verbrannter Müll objekt ist feststecken egal
      waste5+=temp5/4;     
else     waste5+=temp5/2;}
else{   if(VerticesStuck(obj)>=GetVertexNum(obj) && GetID(obj)!=VBWS && GetID(obj)!=_X1A) // beim verbrannter Müll objekt ist feststecken egal//Im Erdboden vergraben : Nicht so umweltschädigent
      waste5+=temp5/2;
     else     waste5+=temp5;};
};

  for(var obj in FindObjects(Find_Func("IsWaste"),Find_Owner(6)))
  {
    temp6=obj->IsWaste();
if(GetPlrKnowledge(6,_BAO) && !(GetPlrKnowledge(6,_BA2))) {if(VerticesStuck(obj)>=GetVertexNum(obj) && GetID(obj)!=VBWS && GetID(obj)!=_X1A) // beim verbrannter Müll objekt ist feststecken egal
      waste6+=(temp6/2*(200/3))/100;     
else     waste6+=(temp6*(200/3))/100;}
else if(GetPlrKnowledge(6,_BAO) && GetPlrKnowledge(6,_BA2))
{if(VerticesStuck(obj)>=GetVertexNum(obj) && GetID(obj)!=VBWS && GetID(obj)!=_X1A) // beim verbrannter Müll objekt ist feststecken egal
      waste6+=temp6/4;     
else     waste6+=temp6/2;}
else{   if(VerticesStuck(obj)>=GetVertexNum(obj) && GetID(obj)!=VBWS && GetID(obj)!=_X1A) // beim verbrannter Müll objekt ist feststecken egal//Im Erdboden vergraben : Nicht so umweltschädigent
      waste6+=temp6/2;
     else     waste6+=temp6;};
};

  for(var obj in FindObjects(Find_Func("IsWaste"),Find_Owner(7)))
  {
    temp7=obj->IsWaste();
if(GetPlrKnowledge(7,_BAO) && !(GetPlrKnowledge(7,_BA2))) {if(VerticesStuck(obj)>=GetVertexNum(obj) && GetID(obj)!=VBWS && GetID(obj)!=_X1A) // beim verbrannter Müll objekt ist feststecken egal
      waste7+=(temp7/2*(200/3))/100;     
else     waste7+=(temp7*(200/3))/100;}
else if(GetPlrKnowledge(7,_BAO) && GetPlrKnowledge(7,_BA2))
{if(VerticesStuck(obj)>=GetVertexNum(obj) && GetID(obj)!=VBWS && GetID(obj)!=_X1A) // beim verbrannter Müll objekt ist feststecken egal
      waste7+=temp7/4;     
else     waste7+=temp7/2;}
else{   if(VerticesStuck(obj)>=GetVertexNum(obj) && GetID(obj)!=VBWS && GetID(obj)!=_X1A) // beim verbrannter Müll objekt ist feststecken egal//Im Erdboden vergraben : Nicht so umweltschädigent
      waste7+=temp7/2;
     else     waste7+=temp7;};
};

gesamtwaste=waste+waste1+waste2+waste3+waste4+waste5+waste6+waste7;
 return(gesamtwaste);
}


public func CheckLimits()
    {
    globalpollution=BoundBy(globalpollution,0,globalnature);   
    }

public func FilterAir()
{
// ausgewachsene Bäume zählen -6, wachsende Bäume zählen -3, tote Bäume-2 und wachsendes Gras -0,5 und fertiges gras -1
baeume=(ObjectCount2(Find_Func("IsTree"),Find_OCF(OCF_Fullcon),Find_Not(Find_Func("IsDeadTree")))*6 + ObjectCount2(Find_Func("IsTree"),Find_Not(Find_OCF(OCF_Fullcon)),Find_Not(Find_Func("IsDeadTree")))*3 + ObjectCount2(Find_Not(Find_OCF(OCF_Fullcon)),Find_Func("IsBulletGrasTarget"))/2 + ObjectCount2(Find_OCF(OCF_Fullcon),Find_Func("IsBulletGrasTarget"))*1 + ObjectCount2(Find_Func("IsDeadTree"))*2) ;
  return(1);
}

//Activate: return(MessageWindow(GetDesc(),Par()));


global func RemovePlayer(iSpieler)
{
	var clonk;
	while(clonk=FindObjectOwner(0,iSpieler,0,0,0,0,OCF_CrewMember,0,0,clonk))
	  clonk->ResetName();
	return(1);
}



/* Windspeicherobjekte werden im Moment nicht korrekt in Spielständen gespeichert.
   Also einfach bei jedem Start neu erstellen (dauert ja nur 2,3 Sekunden) */	 
global func UpdateTransferZone()
{
	GameCall("Windstrom");
	return(1);
}

// Einstellungsmöglichkeiten

public func IsChooserConfig(){return 1;}

//public func ConfigSettings(iIndex,iSymb)
//{
//  var ret;
//  if(!iIndex--){ if(!iSymb) return Format("Naturbelastbarkeit:%d%",startnature*100/500000); //if(iSymb==2) return 5; return XSYX;}
//  if(!iIndex--){ if(!iSymb) return "+10%"; if(iSymb==2) return 3; return XSYX;}
//  if(!iIndex--){ if(!iSymb) return "+1% "; if(iSymb==2) return 1; return XSYX;}
//  if(!iIndex--){ if(!iSymb) return "-1% "; if(iSymb==2) return 2; return XSYX;}
//  if(!iIndex--){ if(!iSymb) return "-10%"; if(iSymb==2) return 4; return XSYX;}
//}


//public func ConfigSettingsCall(iIndex)
//{
//  if(!iIndex--)
//   return 0;
//  if(!iIndex--)
//    return globalnature=startnature=BoundBy(startnature+50000,50000,5000000);
//  if(!iIndex--)
//    return globalnature=startnature=BoundBy(startnature+5000,50000,5000000);
//  if(!iIndex--)
//    return globalnature=startnature=BoundBy(startnature-5000,50000,5000000);
//  if(!iIndex--)
//    return globalnature=startnature=BoundBy(startnature-50000,50000,5000000);
//}