/*-- Angriffsentferner --*/

#strict 3
local anzahl, wert; // anzahl wird als lokale Variable definiert
local kill,fertig,message; // wenn message wahr ist, dann wird bei erledigung eine nachricht ausgegeben

func Activate()
{
  MessageWindow(Format("%v von %v Feinden erledigt",anzahl,wert),Par());
  return(1);
}

func Initialize()
{
SetPosition(1,1);
anzahl=nil;
}

func Einstellen(ierledigen,ikill,imessage)
{
wert = ierledigen; // die anzahl an monstern die getötet werden muss
kill = ikill; // wenn wahr, werden alle übrigen Monster nach erreichen getötet
message = imessage;
}

func Zaehler1()  // Monsterangriff entfernen
{
  //Log("monsterzaehler aufgerufen");
    S42MigrationStep(anzahl, 1, true);
  if (And((anzahl ?? 0) >= (wert ?? 0), !fertig))
  { 
    Beenden();
  }
}

func Beenden()
{
    if ( FindObject(MOAN) )  
      RemoveAll(MOAN);
    if(kill)
    {
        fertig=1;  // damit dies beim töten der monster nicht nochmal aufgerufen wird
        var monster;
        if (FindObject(MONS)) 
         for (monster in FindObjects(Find_ID(MONS)))
         {if(Equal(kill, 1)) Kill(monster); if(Equal(kill, 2)) RemoveObject(monster);}
        if(message) Message("Das waren alle Monster!");
    }
    if (!kill) if(message) Message("Es kommen keine neuen Monster mehr nach");
    GameCall("Losgehts"); // ein Call an ein eventuelles Szenario skript, damit es weiß, dass der Angriff zurückgeschlagen wurde
    for(var auftragshaus in FindObjects(Find_ID(_SH5))) auftragshaus->~Jagdfertig(this());
    RemoveObject();  
}
