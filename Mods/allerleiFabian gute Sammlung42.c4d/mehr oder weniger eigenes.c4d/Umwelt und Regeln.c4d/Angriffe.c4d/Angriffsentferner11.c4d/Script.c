/*-- Angriffsentferner --*/

#strict
local anzahl,wert; // anzahl wird als lokale Variable definiert
local kill,fertig,message;

func Activate()
{
  MessageWindow(Format("%v von %v Feinden erledigt",anzahl,wert),Par());
  return(1);
}

func Initialize()
{
SetPosition(1,1);
anzahl=0;
wert=5; // wenn nichts eingestellt, dann müssen 5 monster erledigt werden.
kill=1; // nicht anders eingestellt, werden nach erreichen der Anzahl, alle übrigen Monster automatisch getötet
}

func Einstellen(ierledigen,ikill,imessage)
{
if(ierledigen)
wert = ierledigen; // die anzahl an monstern die getötet werden muss
kill = ikill; // wenn wahr, werden alle übrigen Monster nach erreichen getötet
message = imessage;
}

func Zaehler11()  // Monsterangriff entfernen
{
  anzahl++; // anzahl wird um 1 erhöht
  if (anzahl >= wert && !fertig)  // Bei if die Bedingung immer in runde Klammern verpacken
  { 
    Beenden();
  }
}

func Beenden()
{
    if ( FindObject(K_AN) )  
      RemoveAll(K_AN);
    fertig=1;  // damit dies beim töten der monster nicht nochmal aufgerufen wird
    if(kill)
    {
    var monster;
    if (FindObject(KRAE)) 
     for (monster in FindObjects(Find_ID(KRAE)))
      {if(kill==1) Kill(monster); if(kill==2) RemoveObject(monster);}
    if(message) Message("Diese nervigen Krähen sind endlich ausgerottet!");
    }
    if (!kill) if(message) Message("Es kommen keine Krähen mehr nach");
    GameCall("Losgehts"); // ein Call an ein eventuelles Szenario skript, damit es weiß, dass der Angriff zurückgeschlagen wurde
    for(var auftragshaus in FindObjects(Find_ID(_SH5))) auftragshaus->~Jagdfertig(this());
    RemoveObject();  // sich selbst nach getaner Arbeit entfernen
}

//Die Zaehlerfunktion muss beim Tod eines Monsters in deren Skript hier info liefern

