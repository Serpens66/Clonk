/*-- Angriffsentferner --*/

#strict 3
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
anzahl=nil;
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


func Zaehler13()  // Monsterangriff entfernen
{
  S42MigrationStep(anzahl, 1, true); // anzahl wird um 1 erhöht
  if (And((anzahl ?? 0) >= (wert ?? 0), !fertig)) // Bei if die Bedingung immer in runde Klammern verpacken
  { // Durch diesen Block geschweifter Klammern nach einer If-Abfrage kannst du mehrere Befehle unter dem if zusammenfassen, die nur im Falle von anzahl > wert ausgeführt werden
    Beenden();
  }
}

func Beenden()
{
    if ( FindObject(PH_A) )  // Zur Sicherheit überprüfst du vor dem Entfernen, ob das Zielobjekt vorhanden ist
      RemoveAll(PH_A);
    anzahl = nil; // anzahl zurücksetzen
    fertig=1;  // damit dies beim töten der monster nicht nochmal aufgerufen wird
    //Log("PhoenixRegel entfernt");
    if(kill)
    {
    var monster;
    if (FindObject(PHNX)) 
     for (monster in FindObjects(Find_ID(PHNX)))
      {if(Equal(kill, 1)) Kill(monster); if(Equal(kill, 2)) RemoveObject(monster);}
    if(message) Message("Und es bleibt nur ein Haufen Asche über!");
    }
    if(!kill) if(message) Message("Es kommen keine neuen Phoenixe mehr nach");
    GameCall("Losgehts"); // ein Call an ein eventuelles Szenario skript, damit es weiß, dass der Angriff zurückgeschlagen wurde
    for(var auftragshaus in FindObjects(Find_ID(_SH5))) auftragshaus->~Jagdfertig(this());
    RemoveObject();  // sich selbst nach getaner Arbeit entfernen
}

//Die Zaehlerfunktion muss beim Tod eines Monsters in deren Skript hier info liefern

