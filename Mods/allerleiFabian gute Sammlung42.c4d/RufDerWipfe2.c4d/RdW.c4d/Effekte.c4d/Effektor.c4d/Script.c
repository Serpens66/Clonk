#strict 3

// static musikerlaubt,cheatmode,g_iFrameRate;

public func Initialize()
{
  // SetPosition(1,1);
  // if(!FindObject(SMUS))CreateObject(SMUS,0,0,-1);
  // var i = 0;
  // if(!GameCall("DontAllowFoW"))
  // {
    // for (var plrnum = GetPlayerCount (); plrnum; i++)
      // if (GetPlayerName (i)&&!GetBit(satellites,i))
      // {
        // plrnum--;
        // SetFoW (1, i);
      // }
  // }
  // if(FrameCounter()<10)
    // GiveHintOfDay();
  // return(1);
}

// global func GiveHintOfDay()
// {
  // var szHint,Hints;
  // Hints=["Die Gesundheit bestimmt wie schnell sich der Clonk regeneriert... sie sinkt wenn entweder die Bedürfnisse gering sind oder durch Steintreffer o.a.",
         // "Im Schlaf regeneriert der Clonk Lebenspunkte, Gesundheit und Psyche. Ausserdem werden Krankheiten auskuriert",
         // "Krankheiten können ganz plötzlich entstehen und verschlechtern den Zustand des Clonks. Sie können aber auch durch Gifte entstehen",
         // "Gelangt die Psyche unter ein bestimmtes Minimum dreht der Clonk durch! Die Psyche sinkt hauptsächlich durch besondere Gegebenheiten: Geister, Strahlung, Krankheit sowie Krieg und Tod",
         // "im Kontextmenü unter Effekte/Head Up Display konfigurieren das Aussehen des HUDs ändern",
         // "Es empfiehlt sich gleich am Anfang ein Dorf zu bauen... damit ist eine kleine Einnahmequlle für Notzeiten schonmal gesichert",
         // "Atomkraftwerke sollten sie ausserhalb der Siedlung bauen... Unfälle können immer mal passieren.",
         // "Es gibt keinen zuverlässigen Schutz gegen atomare Strahlung... der Schutzanzug ist zum schützt nur vor Giften gedacht",
         // "zum Benutzen von Gewehren benötigen die Clonks eine Soldatenausbildung (Soldaten benötigen im Gegensatz zum alten RdW kein Sold)",
         // "Die Fackel lässt sich auch als Waffe gegen Gebäude benutzen, einfach vor einem Gebäude einen \"Brand entfachen\" die Fackel fügt dem Gebäude nun permanent Schaden zu.",
         // "Geister lassen sich ganz normal vernichten, allerdings ist es mit den Zaubern der \"Schattenschlange\" weitaus leichter",
         // "Geister müssen in der Nähe des Objektes bleiben an die sie gebunden sind (im Normalfall die Leiche)",
         // "Fackeln lassen sich per [Doppelt-Werfen] wieder entfernen",
         // "Kräuter müssen per Anfassen -> [Doppelt-Graben] eingesammelt werden",
         // "Vergrabener Müll wirkt sich nur schwach auf die Umweltverschmutzung aus",
         // "Giftflaschen und Schlafgasbomben entfalten ihre volle Wirkung in Minenschächten",
         // "Mit Dünger lässt sich der Weizenwachstum beschleunigen",
         // "Alkohol sollte nur mässig verzehrt werden da zu hohe Dosis den Clonk stark schädigt",
         // "Mit dem Heiligenkrone-Artefakt lassen sich tote Clonks wieder ins Leben zurückholen.",
         // "Falls sie Performanceprobleme haben empfielt es sich die Anti-Lag-Regel zu aktivieren. Dies deaktiviert z.b. die Partikelwolken sowie die Feuerlichter.",
         // "Wenn sie Bugs finden können sie uns helfen in dem sie diese im öffentlichen Bugtracker auf dem CPPP eintragen. Featurewünsche werden wenn sie sinnvoll und nicht zu schwer umzusetzen sind auch aktzeptiert.",
         // "Wer Zeichensetzungsfehler findet darf sie Aufessen.",
         // "Sehe das Leben positiv. ;)",
         // "Mach dir nichts draus wenn du mal verlierst... denn durch Fehler lernt man.",
         // "Was hier steht ist normalerweise ein Tipp^^"];
  // Log("<c ffff00>Tipp</c>:%s",Hints[Random(GetLength(Hints))]);
// }

// public func InitializePlayer(iPlr)
// {
  // if(!GameCall("DontAllowFoW")&&!GetBit(satellites,iPlr))
    // SetFoW(1,iPlr);
// }

// public func UpdateTransferZone()
// {
  // Initialize();
// }

protected func ControlEffects()
{
  // /* Partikel */
  // PushParticles("Laub",GetWind()/20+RandomX(-5,6),0);
  // if(!Random(10))  
   // PushParticles("Laub",0,RandomX(-10,0));
  return(1);
} 

// /* Menü */
// public func Menu(clonk,selection)
// {
   // if(!IsNetwork()) if(Cheat(clonk)) return(CallMessageBoard());
   // CreateMenu(EFS1,clonk,this(),0,"Nix",0,1);
     // if( musikerlaubt) AddMenuItem("%s deaktivieren","Flip",SMUS,clonk);
     	// else	       AddMenuItem("%s aktivieren","Flip",SMUS,clonk);
   // AddMenuItem("Head Up Display konfigurieren",Format("OpenHeadUpMenu(Object(%d))",ObjectNumber(clonk)),EFEC,clonk);
   // AddMenuItem("KI konfigurieren",Format("OpenKIOptionsMenu(Object(%d))",ObjectNumber(clonk)),EFEC,clonk);
   // return(1);
// }

// private func Cheat(clonk)
// {
  // if(IsDay())
   // if(GetID(Contained(clonk))==_I1A)
    // if(FindContents(WIPF,clonk))
     // return(1);
  // return(0);
// }

// protected func InputCallback(eingabe)
// {
  // if(eingabe ne "Der Wipf ruft") return(0);
  // Log("Schummelmodus aktiviert");
  // Sound("Trumpet",1);
  // cheatmode=1; 
  // return(1);
// }


// public func Flip(symbol)
// { 
  // if(!FindObject(SMUS))CreateObject(SMUS,0,0,-1);
  // if(musikerlaubt){ musikerlaubt=0; DeactivateAutoMusic();}
  // else            { musikerlaubt=1; ActivateAutoMusic();}
  
  // return(1);
// }

// public func OpenHeadUpMenu(clonk)
// {
  // CreateMenu(EFS1,clonk,this(),0,"Nix",0,1,true);
  // AddMenuItem("Design ändern",Format("ChangeStyle(%d)",GetOwner(clonk)),EFEC,clonk,0,0,"Das Aussehen des HUDs verändern.");   //AddMenuItem("Grösse ändern",Format("SetPlrExtraData(%d,\"RdW_HUD_Small\",!GetPlrExtraData(%d,\"RdW_HUD_Small\"))&&ResetHUD(%d)",GetOwner(clonk),GetOwner(clonk),GetOwner(clonk)),EFEC,clonk,0,0,"Die Grösse des HUDs verändern.");
// }

// public func ChangeStyle(iPlr)
// {
  // var style=GetPlrExtraData(iPlr,"RdW_HUD_Style");
  // var to;
  // to=style+1;
  // if(to>5)
    // to=0;
  // SetPlrExtraData(iPlr,"RdW_HUD_Style",to);
  // ResetHUD(iPlr);
// }

// public func OpenKIOptionsMenu(clonk)
// {
  // CreateMenu(EFS1,clonk,this(),0,"Nix",0,1);
  // AddMenuItem("Suchverhalten",Format("OpenKIOptionsMenuX1(Object(%d))",ObjectNumber(clonk)),_ZFK,clonk,0,0,"KIs sind nicht schlau, verbessert aber die Performance ein wenig",2,1);
  // AddMenuItem("Suchreichweite" ,Format("OpenKIOptionsMenuX2(Object(%d))",ObjectNumber(clonk)),_ZFK,clonk,0,0,"KIs sind immer noch nicht wirklich schlau, aber imschlauer als die schwache KI",2,2);   //AddMenuItem("Stark"  ,Format("SetKIMode(2,%d)",GetOwner(clonk)),_ZFK,clonk,0,0,"KIs sind mehr oder weniger schlau, die Performance könnte aber dran leiden",2,3);
// }

// public func OpenKIOptionsMenuX1(clonk)
// {
  // CreateMenu(EFS1,clonk,this(),0,"Nix",0,1);
  // AddMenuItem("Dumm",Format("SetKIMode(0,%d)",GetOwner(clonk)),_ZFK,clonk,0,0,"KIs sind nicht schlau, verbessert aber die Performance ein wenig",2,1);
  // AddMenuItem("Standart" ,Format("SetKIMode(1,%d)",GetOwner(clonk)),_ZFK,clonk,0,0,"KIs sind immer noch nicht wirklich schlau, aber immerhin schlauer als die schwache KI",2,2);  //AddMenuItem("Stark"  ,Format("SetKIMode(2,%d)",GetOwner(clonk)),_ZFK,clonk,0,0,"KIs sind mehr oder weniger schlau, die Performance könnte aber dran leiden",2,3);
// }

// public func OpenKIOptionsMenuX2(clonk)
// {
  // CreateMenu(EFS1,clonk,this(),0,"Suchdistanz",0,1);
  // AddMenuItem(" 200 px" ,Format("SetKIDist(-800,%d)",GetOwner(clonk)),_ZFK,clonk,0,0,"...");
  // AddMenuItem(" 600 px" ,Format("SetKIDist(-400,%d)",GetOwner(clonk)),_ZFK,clonk,0,0,"...");
  // AddMenuItem("1000 px" ,Format("SetKIDist(+000,%d)",GetOwner(clonk)),_ZFK,clonk,0,0,"...");
  // AddMenuItem("1400 px" ,Format("SetKIDist(+400,%d)",GetOwner(clonk)),_ZFK,clonk,0,0,"...");
  // AddMenuItem("1800 px" ,Format("SetKIDist(+800,%d)",GetOwner(clonk)),_ZFK,clonk,0,0,"...");
// }

// public func SetKIMode(iValue,iPlr)
// {
  // SetPlrExtraData(iPlr,"RdW_KIMode",iValue);
// }

// public func SetKIDist(iValue,iPlr)
// {
  // SetPlrExtraData(iPlr,"RdW_KIDist",iValue);
// }

