/*-- x Gold angriff --*/

#strict

local aPixStart, aMatStart,aAllInPixStart,aErreicht,locplayer,aDone;

// evlt noch Tiefbrunnen und Bergwerk beachten? Diese können neue Materialien aus dem Nichts erschaffen. Wenn dadurch trotz Verbrauch die aktuell vorhandene Menge von Material dauerhaft bei zb. 6% bleibt, kommt auch nur ein Ereignis, wenn 5% eingestellt ist.
// evlt Wasser und Eis gleichsetzen? Allerdings gibts Eisklumpen, aber keine wasserklumpen
// bei schnee wird der vom himmel gefallene schnee bisher offenbar nicht berücksichtigt, ist aber vemrutlich auch besser so

// Kann mittels des neuen Spielzielauswählers variabel eingestellt werden
public func IsVariabelChooseable2() { return(1); }


func Activate(player) // gibt aus, wieviel wovon abgebaut wurde
{
  var string = "";
  var izahl=0;
    while(izahl<17) 
    {        
        if(izahl==16) // letzter eintrag
        {
            var hinweis = "";
            if(HashGet(hashSpielRegeln, "AbbauEreignisse")[2][izahl])
                hinweis = Format("! %v%",HashGet(hashSpielRegeln, "AbbauEreignisse")[2][izahl]); // zeigt dass diese Regel aktiv ist
            string = Format("%s{{%v}}%v% von %s %s|abgebaut.", //  mit {{IDID}} kann man noch Bilder der materialien einfuegen
            string,
            HashGet(hashSpielRegeln, "AbbauEreignisse")[1][izahl],ErreichteProzent(izahl),HashGet(hashSpielRegeln, "AbbauEreignisse")[0][izahl],hinweis);
        }
        else
        {
            var hinweis = "";
            if(HashGet(hashSpielRegeln, "AbbauEreignisse")[2][izahl])
                hinweis = Format("! %v%",HashGet(hashSpielRegeln, "AbbauEreignisse")[2][izahl]); // zeigt dass diese Regel aktiv ist
            var hinweis2 = "";
            if(HashGet(hashSpielRegeln, "AbbauEreignisse")[2][izahl+1])
                hinweis2 = Format("! %v%",HashGet(hashSpielRegeln, "AbbauEreignisse")[2][izahl+1]); // zeigt dass diese Regel aktiv ist
            string = Format("%s{{%v}}%v% von %s %s|{{%v}}%v% von %s %s|", //  mit {{IDID}} kann man noch Bilder der materialien einfuegen
            string,
            HashGet(hashSpielRegeln, "AbbauEreignisse")[1][izahl],ErreichteProzent(izahl),HashGet(hashSpielRegeln, "AbbauEreignisse")[0][izahl],hinweis,
            HashGet(hashSpielRegeln, "AbbauEreignisse")[1][izahl+1],ErreichteProzent(izahl+1),HashGet(hashSpielRegeln, "AbbauEreignisse")[0][izahl+1],hinweis2); // gleich 2 einträge in einem machen
        }
            
        izahl = izahl + 2;
    }
    MessageWindow(string, player);
}



private func MatObjsCount(inumber)
{
    if(GetMaterialVal("TextureOverlay","Material",Material(MatName(inumber))) == "Liquid") return(0); // bei fluessig gibt es keine klumpen
    return(ObjectCount(HashGet(hashSpielRegeln, "AbbauEreignisse")[1][inumber]));
}
private func MatName(inumber)
{
    return(HashGet(hashSpielRegeln, "AbbauEreignisse")[0][inumber]);
}

private func MatPixelCount(inumber)
{
    if(FrameCounter()<500) return(aPixStart[inumber]); // liefert erst ab 500 werte zurueck!
    else 
    {
        if(MatName(inumber) != "Lava" && MatName(inumber) != "Sand" && MatName(inumber) != "Ashes")
            return(GetMaterialCount(Material(MatName(inumber))));
        else if(MatName(inumber)=="Lava")
            return(GetMaterialCount(Material(MatName(inumber))) + GetMaterialCount(Material("DuroLava"))); // sowohl duro, als auch normale lava beachten
        else if(MatName(inumber)=="Sand")
            return(GetMaterialCount(Material(MatName(inumber))) + GetMaterialCount(Material("FlySand")));
        else if(MatName(inumber)=="Ashes")
            return(GetMaterialCount(Material(MatName(inumber))) + GetMaterialCount(Material("FlyAshes")));
    }
}

protected func Initialize()
{
  aPixStart = [];
  aMatStart = [];
  aAllInPixStart= [];
  aDone = [];
  aErreicht = [];
  for(var inumber=0; inumber<GetLength(HashGet(hashSpielRegeln, "AbbauEreignisse")[0]);inumber++)
  {
    ArrayAdd(aPixStart,100); // fuer jedes material einen Eintrag machen
    ArrayAdd(aMatStart,100);
    ArrayAdd(aAllInPixStart,200);
    ArrayAdd(aDone,0);
    ArrayAdd(aErreicht,HashGet(hashSpielRegeln, "AbbauEreignisse")[2][inumber]); // bei alle 5% also zu anfang 5
    ScheduleCall(0,"WievielamStart",1,510,inumber); // nach 501 frames mal nachzaehlen, wieviel des materials zu beginn da ist.
  }
  return();
}

func CheckTime() // timercall alle 250 frames
{
   if(FrameCounter()<600) return;
   for(var inumber=0; inumber<GetLength(HashGet(hashSpielRegeln, "AbbauEreignisse")[0]);inumber++)
  {
    if(HashGet(hashSpielRegeln, "AbbauEreignisse")[2][inumber] && aAllInPixStart[inumber])  // wenn aktiviert, und startwerte vorhanden sind 
    {
        if(ErreichteProzent(inumber)>=aErreicht[inumber]) // wenn zb alle 5 % und 5% wurde erreicht, wird ereignis ausgelöst und der aErreicht eintrag + 5 erhöht und damit als nächstes bei 10 ausgelöst. 
        {
            // Log(Format("%v% wurde von Material %s erreicht. %v musste erreicht werden.",ErreichteProzent(inumber),HashGet(hashSpielRegeln, "AbbauEreignisse")[0][inumber],aErreicht[inumber]));
            aErreicht[inumber] = aErreicht[inumber] + HashGet(hashSpielRegeln, "AbbauEreignisse")[2][inumber];
            EreignisAusloesen();
        }
    }        
  }
}

func WievielamStart(inumber)
{
    if(FrameCounter()<501 || aDone[inumber]) return;
// beim Start einmal ausführen, damit die Startwerte gesetzt werden
   aPixStart[inumber] = MatPixelCount(inumber);
   aMatStart[inumber] = MatObjsCount(inumber);
   
   if(GetMaterialVal("TextureOverlay","Material",Material(MatName(inumber))) != "Liquid")
   {
       if(GetMaterialVal("Blast2ObjectRatio","Material",Material(MatName(inumber))))
       {
           aAllInPixStart[inumber] = aPixStart[inumber] + aMatStart[inumber] * GetMaterialVal("Blast2ObjectRatio","Material",Material(MatName(inumber))); // wandelt die existierenden Materialien in Pixel um, anahnd der Blast2ObjectRatio.
       }
       else if(GetMaterialVal("Dig2ObjectRatio","Material",Material(MatName(inumber))))
       {
           aAllInPixStart[inumber] = aPixStart[inumber] + aMatStart[inumber] * GetMaterialVal("Dig2ObjectRatio","Material",Material(MatName(inumber))); // wandelt die existierenden Materialien in Pixel um, anahnd der Dig2ObjectRatio
       }
       else
           aAllInPixStart[inumber] = aPixStart[inumber];
   }
   else
   {
       aAllInPixStart[inumber] = aPixStart[inumber];  // bei fluessigem material gibts nur pixel
   }
   aDone[inumber] = 1;
}


func ErreichteProzent(inumber) // gibt zurück, wieviel Prozent bereits abgebaut wurden
{
     var pixel = MatPixelCount(inumber);
     var material = MatObjsCount(inumber);
     var AllInPix;
     
     if(GetMaterialVal("TextureOverlay","Material",Material(MatName(inumber))) != "Liquid")
   {
       if(GetMaterialVal("Blast2ObjectRatio","Material",Material(MatName(inumber))))
       {
           AllInPix = pixel + material * GetMaterialVal("Blast2ObjectRatio","Material",Material(MatName(inumber))); // wandelt die existierenden Materialien in Pixel um, anahnd der Blast2ObjectRatio.
       }
       else if(GetMaterialVal("Dig2ObjectRatio","Material",Material(MatName(inumber))))
       {
           AllInPix = pixel + material * GetMaterialVal("Dig2ObjectRatio","Material",Material(MatName(inumber))); // wandelt die existierenden Materialien in Pixel um, anahnd der Dig2ObjectRatio
       }
       else
           AllInPix = pixel;
   }
   else
   {
       AllInPix = pixel;  // bei fluessigem material gibts nur pixel
   }
   
   // if(HashGet(hashSpielRegeln, "AbbauEreignisse")[0][inumber]=="Gold")
    // Log(Format("Material: %s . AllinPix: %v . AllInPixStart: %v . Prozent: %v",HashGet(hashSpielRegeln, "AbbauEreignisse")[0][inumber],AllInPix,aAllInPixStart[inumber],(((aAllInPixStart[inumber]-AllInPix)*100) / aAllInPixStart[inumber])));

   if(aAllInPixStart[inumber]) return(((aAllInPixStart[inumber]-AllInPix)*100) / aAllInPixStart[inumber]);
   else return(0); // wenn es keine pixel dieses materials gibt, immer 0 returnen
}

func EreignisAusloesen()
{
    if(HashGet(hashSpielRegeln, "AbbauEreignisse")[3]==1) 
        return(Katastrophenereignis());
    if(HashGet(hashSpielRegeln, "AbbauEreignisse")[3]==2) 
        return(Monsterereignis());
    
    // ansonsten beides moeglich
    if(RandomX(0,1)) // 50 zu 50 chance entweder monster oder katastrophe
        return(Monsterereignis());
    else
        return(Katastrophenereignis());
}

func Monsterereignis()
{
    Log("Ein Monsterangriff");
    var Age = 0;
    for(var i = 0; i < GetPlayerCount(); i++)
    {
        if(Age<gPlayerAge(GetPlayerByIndex(i))) Age = gPlayerAge(GetPlayerByIndex(i)); // das hoechste Zeitalter aller spieler rausfinden
    }
    if(Age<=4)  // ab zeitalter 5 hat man armbrust usw.
    {
        var liste = ["Zufall1","Zufall2"];
        var imonster = liste[RandomX(0,1)]; // zufall1 oder zufall2, relativ leichte monster
        var ikeinflug = !Random(10);   // selten flugviecher, weil die ohne vernünftige waffen schwer zu treffen sind.
        var ierledigen=RandomX(1,5);  // wieviele Feinde erledigt werden muessen
        var iaufeinmal=RandomX(4,10); // wieviele monster aufeinmal da sind (wenn eins getötet, kommt ein neues) 
    }
    if(Age>4 && Age<11)  // ab zeitalter 11 hat man zukunftswaffen, ab 5 hat man immerhin armbrust usw.
    {
        var liste = ["Zufall1","Zufall2","Zufall3"];
        var imonster = liste[RandomX(0,2)];  // zufall1 oder zufall2, relativ leichte monster. zusätzlich zufall3, Clonks, ritter und magier
        var ikeinflug = !Random(10);   // selten flugviecher, weil die ohne vernünftige waffen schwer zu treffen sind.
        var ierledigen=RandomX(1,6);  // wieviele Feinde erledigt werden muessen
        var iaufeinmal=RandomX(4,10); // wieviele monster aufeinmal da sind (wenn eins getötet, kommt ein neues) 
    }
    if(Age>=11 && Age<13)  // ab zeitalter 11 hat man zukunftswaffen, aber 13 auch automatische abwehrtürme
    {
        var liste = ["Zufall1","Zufall2","Zufall3","Zufall4"];
        var imonster = liste[RandomX(0,3)]; // zufall1 oder zufall2, relativ leichte monster. zusätzlich zufall3, Clonks, ritter und magier. Und zufall4 Aliens
        var ikeinflug = 0;   
        var ierledigen=RandomX(2,8);  // wieviele Feinde erledigt werden muessen
        var iaufeinmal=RandomX(5,10); // wieviele monster aufeinmal da sind (wenn eins getötet, kommt ein neues) 
    }
    if(Age>=13)  // ab zeitalter 11 hat man zukunftswaffen, aber 13 auch automatische abwehrtürme
    {
        var imonster = "Zufall";  // Alles, auch Hazardclonks, assasine und kleine raumschiffe möglich
        var ikeinflug = 0;  
        var ierledigen=RandomX(2,10);  // wieviele Feinde erledigt werden muessen
        var iaufeinmal=RandomX(5,12); // wieviele monster aufeinmal da sind (wenn eins getötet, kommt ein neues) 
    }
    MMonsterAttack(imonster, iaufeinmal, ierledigen,2,ikeinflug,1); // globale funktion im Globale Ereignisse Skript
}

func Katastrophenereignis()
{
    Log("Eine Katastrophe");
    var Age = 0;
    for(var i = 0; i < GetPlayerCount(); i++)
    {
        if(Age<gPlayerAge(GetPlayerByIndex(i))) Age = gPlayerAge(GetPlayerByIndex(i)); // das hoechste Zeitalter aller spieler rausfinden
    }
    if(Age<=5)  KKatastrophen("Zufall", RandomX(1,5),RandomX(400,1200),1); // globale funktion im Globale Ereignisse Skript
    else if(Age>5 && Age<=12) KKatastrophen("Zufall", RandomX(2,8),RandomX(500,1500),1);
    else if(Age>12) KKatastrophen("Zufall", RandomX(4,10),RandomX(800,2000),1);
    return;
}






// folgendes ist veraltet, weil zu kompliziert gemacht
/*

// folgende Funktionen geben zurück, ob x prozent erreicht wurden
func Prozent5()
{
    // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels5)
  {
    // Materialpixel 
    pixels5 = pixStart*95/100 + 150;
    // Materialstücke                 // erfüllt nach 5% abgebaut
    objs5 = matStart*95/100;
  }
  // noch zu viel?
  if (MatPixelCount() > pixels5)          return(0);
  if (MatObjsCount() > objs5) return(0);
  return(1);
}


func Prozent10()
{
    // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels10)
  {
    // Materialpixel 
    pixels10 = pixStart*90/100 + 150;
    // Materialstücke                 // erfüllt nach 10% abgebaut
    objs10 = matStart*90/100;
  }
  // noch zu viel?
  if (MatPixelCount() > pixels10)          return(0);
  if (MatObjsCount() > objs10) return(0);
  return(1);
}


func Prozent15()
{
    // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels15)
  {
    // Materialpixel 
    pixels15 = pixStart*85/100 + 150;
    // Materialstücke                 // erfüllt nach 15% abgebaut
    objs15 = matStart*85/100;
  }
  // noch zu viel?
  if (MatPixelCount() > pixels15)          return(0);
  if (MatObjsCount() > objs15) return(0);
  return(1);
}


func Prozent20()
{
    // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels20)
  {
    // Materialpixel 
    pixels20 = pixStart*80/100 + 150;
    // Materialstücke                 // erfüllt nach 20% abgebaut
    objs20 = matStart*80/100;
  }
  // noch zu viel?
  if (MatPixelCount() > pixels20)          return(0);
  if (MatObjsCount() > objs20) return(0);
  return(1);
}


func Prozent25()
{
    // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels25)
  {
    // Materialpixel 
    pixels25 = pixStart*75/100 + 150;
    // Materialstücke                 // erfüllt nach 25% abgebaut
    objs25 = matStart*75/100;
  }
  // noch zu viel?
  if (MatPixelCount() > pixels25)          return(0);
  if (MatObjsCount() > objs25) return(0);
  return(1);
}


func Prozent30()
{
    // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels30)
  {
    // Materialpixel 
    pixels30 = pixStart*70/100 + 150;
    // Materialstücke                 // erfüllt nach 30% abgebaut
    objs30 = matStart*70/100;
  }
  // noch zu viel?
  if (MatPixelCount() > pixels30)          return(0);
  if (MatObjsCount() > objs30) return(0);
  return(1);
}


func Prozent35()
{
    // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels35)
  {
    // Materialpixel 
    pixels35 = pixStart*65/100 + 150;
    // Materialstücke                 // erfüllt nach 35% abgebaut
    objs35 = matStart*65/100;
  }
  // noch zu viel?
  if (MatPixelCount() > pixels35)          return(0);
  if (MatObjsCount() > objs35) return(0);
  return(1);
}


func Prozent40()
{
    // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels40)
  {
    // Materialpixel 
    pixels40 = pixStart*60/100 + 150;
    // Materialstücke                 // erfüllt nach 40% abgebaut
    objs40 = matStart*60/100;
  }
  // noch zu viel?
  if (MatPixelCount() > pixels40)          return(0);
  if (MatObjsCount() > objs40) return(0);
  return(1);
}


func Prozent45()
{
    // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels45)
  {
    // Materialpixel 
    pixels45 = pixStart*55/100 + 150;
    // Materialstücke                 // erfüllt nach 45% abgebaut
    objs45 = matStart*55/100;
  }
  // noch zu viel?
  if (MatPixelCount() > pixels45)          return(0);
  if (MatObjsCount() > objs45) return(0);
  return(1);
}


func Prozent50()
{
    // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels50)
  {
    // Materialpixel 
    pixels50 = pixStart*50/100 + 150;
    // Materialstücke                 // erfüllt nach 50% abgebaut
    objs50 = matStart*50/100;
  }
  // noch zu viel?
  if (MatPixelCount() > pixels50)          return(0);
  if (MatObjsCount() > objs50) return(0);
  return(1);
}


func Prozent55()
{
    // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels55)
  {
    // Materialpixel 
    pixels55 = pixStart*45/100 + 150;
    // Materialstücke                 // erfüllt nach 55% abgebaut
    objs55 = matStart*45/100;
  }
  // noch zu viel?
  if (MatPixelCount() > pixels55)          return(0);
  if (MatObjsCount() > objs55) return(0);
  return(1);
}


func Prozent60()
{
    // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels60)
  {
    // Materialpixel 
    pixels60 = pixStart*40/100 + 150;
    // Materialstücke                 // erfüllt nach 60% abgebaut
    objs60 = matStart*40/100;
  }
  // noch zu viel?
  if (MatPixelCount() > pixels60)          return(0);
  if (MatObjsCount() > objs60) return(0);
  return(1);
}


func Prozent65()
{
    // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels65)
  {
    // Materialpixel 
    pixels65 = pixStart*35/100 + 150;
    // Materialstücke                 // erfüllt nach 65% abgebaut
    objs65 = matStart*35/100;
  }
  // noch zu viel?
  if (MatPixelCount() > pixels65)          return(0);
  if (MatObjsCount() > objs65) return(0);
  return(1);
}


func Prozent70()
{
    // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels70)
  {
    // Materialpixel 
    pixels70 = pixStart*30/100 + 150;
    // Materialstücke                 // erfüllt nach 70% abgebaut
    objs70 = matStart*30/100;
  }
  // noch zu viel?
  if (MatPixelCount() > pixels70)          return(0);
  if (MatObjsCount() > objs70) return(0);
  return(1);
}


func Prozent75()
{
    // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels75)
  {
    // Materialpixel 
    pixels75 = pixStart*25/100 + 150;
    // Materialstücke                 // erfüllt nach 75% abgebaut
    objs75 = matStart*25/100;
  }
  // noch zu viel?
  if (MatPixelCount() > pixels75)          return(0);
  if (MatObjsCount() > objs75) return(0);
  return(1);
}


func Prozent80()
{
    // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels80)
  {
    // Materialpixel 
    pixels80 = pixStart*20/100 + 150;
    // Materialstücke                 // erfüllt nach 80% abgebaut
    objs80 = matStart*20/100;
  }
  // noch zu viel?
  if (MatPixelCount() > pixels80)          return(0);
  if (MatObjsCount() > objs80) return(0);
  return(1);
}


func Prozent85()
{
    // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels85)
  {
    // Materialpixel 
    pixels85 = pixStart*15/100 + 150;
    // Materialstücke                 // erfüllt nach 85% abgebaut
    objs85 = matStart*15/100;
  }
  // noch zu viel?
  if (MatPixelCount() > pixels85)          return(0);
  if (MatObjsCount() > objs85) return(0);
  return(1);
}


func Prozent90()
{
    // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels90)
  {
    // Materialpixel 
    pixels90 = pixStart*10/100 + 150;
    // Materialstücke                 // erfüllt nach 90% abgebaut
    objs90 = matStart*10/100;
  }
  // noch zu viel?
  if (MatPixelCount() > pixels90)          return(0);
  if (MatObjsCount() > objs90) return(0);
  return(1);
}

func Prozent91()
{
    // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels91)
  {
    // Materialpixel 
    pixels91 = pixStart*9/100 + 150;
    // Materialstücke                 // erfüllt nach 91% abgebaut
    objs91 = matStart*9/100;
  }
  // noch zu viel?
  if (MatPixelCount() > pixels91)          return(0);
  if (MatObjsCount() > objs91) return(0);
  return(1);
}

func Prozent92()
{
    // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels92)
  {
    // Materialpixel 
    pixels92 = pixStart*8/100 + 150;
    // Materialstücke                 // erfüllt nach 92% abgebaut
    objs92 = matStart*8/100;
  }
  // noch zu viel?
  if (MatPixelCount() > pixels92)          return(0);
  if (MatObjsCount() > objs92) return(0);
  return(1);
}

func Prozent93()
{
    // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels93)
  {
    // Materialpixel 
    pixels93 = pixStart*7/100 + 150;
    // Materialstücke                 // erfüllt nach 93% abgebaut
    objs93 = matStart*7/100;
  }
  // noch zu viel?
  if (MatPixelCount() > pixels93)          return(0);
  if (MatObjsCount() > objs93) return(0);
  return(1);
}

func Prozent94()
{
    // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels94)
  {
    // Materialpixel 
    pixels94 = pixStart*6/100 + 150;
    // Materialstücke                 // erfüllt nach 94% abgebaut
    objs94 = matStart*6/100;
  }
  // noch zu viel?
  if (MatPixelCount() > pixels94)          return(0);
  if (MatObjsCount() > objs94) return(0);
  return(1);
}


func Prozent95()
{
    // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels95)
  {
    // Materialpixel 
    pixels95 = pixStart*5/100 + 150;
    // Materialstücke                 // erfüllt nach 95% abgebaut
    objs95 = matStart*5/100;
  }
  // noch zu viel?
  if (MatPixelCount() > pixels95)          return(0);
  if (MatObjsCount() > objs95) return(0);
  return(1);
}



protected func CheckTime()
{

if((!matStart) && (!pixStart)) {WievielamStart();Log("Startwerte gesetzt");}

var aktiviertWissen1Prozent5, aktiviertWissen1Prozent5, aktiviertWissen1Prozent10, aktiviertWissen1Prozent10, aktiviertWissen1Prozent15, aktiviertWissen1Prozent15, aktiviertWissen1Prozent20, aktiviertWissen1Prozent20, aktiviertWissen1Prozent25, aktiviertWissen1Prozent25, aktiviertWissen1Prozent30, aktiviertWissen1Prozent30, aktiviertWissen1Prozent35, aktiviertWissen1Prozent35, aktiviertWissen1Prozent40, aktiviertWissen1Prozent40, aktiviertWissen1Prozent45, aktiviertWissen1Prozent45, aktiviertWissen1Prozent50, aktiviertWissen1Prozent50, aktiviertWissen1Prozent55, aktiviertWissen1Prozent55, aktiviertWissen1Prozent60, aktiviertWissen1Prozent60, aktiviertWissen1Prozent65, aktiviertWissen1Prozent65, aktiviertWissen1Prozent70, aktiviertWissen1Prozent70, aktiviertWissen1Prozent75, aktiviertWissen1Prozent75, aktiviertWissen1Prozent80, aktiviertWissen1Prozent80, aktiviertWissen1Prozent85, aktiviertWissen1Prozent85, aktiviertWissen1Prozent90, aktiviertWissen1Prozent90, aktiviertWissen1Prozent91, aktiviertWissen1Prozent91, aktiviertWissen1Prozent92, aktiviertWissen1Prozent92, aktiviertWissen1Prozent93, aktiviertWissen1Prozent93, aktiviertWissen1Prozent94, aktiviertWissen1Prozent94, aktiviertWissen1Prozent95, aktiviertWissen1Prozent95, 
aktiviertWissen2Prozent5, aktiviertWissen2Prozent5, aktiviertWissen2Prozent10, aktiviertWissen2Prozent10, aktiviertWissen2Prozent15, aktiviertWissen2Prozent15, aktiviertWissen2Prozent20, aktiviertWissen2Prozent20, aktiviertWissen2Prozent25, aktiviertWissen2Prozent25, aktiviertWissen2Prozent30, aktiviertWissen2Prozent30, aktiviertWissen2Prozent35, aktiviertWissen2Prozent35, aktiviertWissen2Prozent40, aktiviertWissen2Prozent40, aktiviertWissen2Prozent45, aktiviertWissen2Prozent45, aktiviertWissen2Prozent50, aktiviertWissen2Prozent50, aktiviertWissen2Prozent55, aktiviertWissen2Prozent55, aktiviertWissen2Prozent60, aktiviertWissen2Prozent60, aktiviertWissen2Prozent65, aktiviertWissen2Prozent65, aktiviertWissen2Prozent70, aktiviertWissen2Prozent70, aktiviertWissen2Prozent75, aktiviertWissen2Prozent75, aktiviertWissen2Prozent80, aktiviertWissen2Prozent80, aktiviertWissen2Prozent85, aktiviertWissen2Prozent85, aktiviertWissen2Prozent90, aktiviertWissen2Prozent90, aktiviertWissen2Prozent91, aktiviertWissen2Prozent91, aktiviertWissen2Prozent92, aktiviertWissen2Prozent92, aktiviertWissen2Prozent93, aktiviertWissen2Prozent93, aktiviertWissen2Prozent94, aktiviertWissen2Prozent94, aktiviertWissen2Prozent95, aktiviertWissen2Prozent95, 
aktiviertWissen3Prozent5, aktiviertWissen3Prozent5, aktiviertWissen3Prozent10, aktiviertWissen3Prozent10, aktiviertWissen3Prozent15, aktiviertWissen3Prozent15, aktiviertWissen3Prozent20, aktiviertWissen3Prozent20, aktiviertWissen3Prozent25, aktiviertWissen3Prozent25, aktiviertWissen3Prozent30, aktiviertWissen3Prozent30, aktiviertWissen3Prozent35, aktiviertWissen3Prozent35, aktiviertWissen3Prozent40, aktiviertWissen3Prozent40, aktiviertWissen3Prozent45, aktiviertWissen3Prozent45, aktiviertWissen3Prozent50, aktiviertWissen3Prozent50, aktiviertWissen3Prozent55, aktiviertWissen3Prozent55, aktiviertWissen3Prozent60, aktiviertWissen3Prozent60, aktiviertWissen3Prozent65, aktiviertWissen3Prozent65, aktiviertWissen3Prozent70, aktiviertWissen3Prozent70, aktiviertWissen3Prozent75, aktiviertWissen3Prozent75, aktiviertWissen3Prozent80, aktiviertWissen3Prozent80, aktiviertWissen3Prozent85, aktiviertWissen3Prozent85, aktiviertWissen3Prozent90, aktiviertWissen3Prozent90, aktiviertWissen3Prozent91, aktiviertWissen3Prozent91, aktiviertWissen3Prozent92, aktiviertWissen3Prozent92, aktiviertWissen3Prozent93, aktiviertWissen3Prozent93, aktiviertWissen3Prozent94, aktiviertWissen3Prozent94, aktiviertWissen3Prozent95, aktiviertWissen3Prozent95, 
aktiviertWissen4Prozent5, aktiviertWissen4Prozent5, aktiviertWissen4Prozent10, aktiviertWissen4Prozent10, aktiviertWissen4Prozent15, aktiviertWissen4Prozent15, aktiviertWissen4Prozent20, aktiviertWissen4Prozent20, aktiviertWissen4Prozent25, aktiviertWissen4Prozent25, aktiviertWissen4Prozent30, aktiviertWissen4Prozent30, aktiviertWissen4Prozent35, aktiviertWissen4Prozent35, aktiviertWissen4Prozent40, aktiviertWissen4Prozent40, aktiviertWissen4Prozent45, aktiviertWissen4Prozent45, aktiviertWissen4Prozent50, aktiviertWissen4Prozent50, aktiviertWissen4Prozent55, aktiviertWissen4Prozent55, aktiviertWissen4Prozent60, aktiviertWissen4Prozent60, aktiviertWissen4Prozent65, aktiviertWissen4Prozent65, aktiviertWissen4Prozent70, aktiviertWissen4Prozent70, aktiviertWissen4Prozent75, aktiviertWissen4Prozent75, aktiviertWissen4Prozent80, aktiviertWissen4Prozent80, aktiviertWissen4Prozent85, aktiviertWissen4Prozent85, aktiviertWissen4Prozent90, aktiviertWissen4Prozent90, aktiviertWissen4Prozent91, aktiviertWissen4Prozent91, aktiviertWissen4Prozent92, aktiviertWissen4Prozent92, aktiviertWissen4Prozent93, aktiviertWissen4Prozent93, aktiviertWissen4Prozent94, aktiviertWissen4Prozent94, aktiviertWissen4Prozent95, aktiviertWissen4Prozent95, 

aktiviertWissenXXProzent5, aktiviertWissenXXProzent5, aktiviertWissenXXProzent10, aktiviertWissenXXProzent10, aktiviertWissenXXProzent15, aktiviertWissenXXProzent15, aktiviertWissenXXProzent20, aktiviertWissenXXProzent20, aktiviertWissenXXProzent25, aktiviertWissenXXProzent25, aktiviertWissenXXProzent30, aktiviertWissenXXProzent30, aktiviertWissenXXProzent35, aktiviertWissenXXProzent35, aktiviertWissenXXProzent40, aktiviertWissenXXProzent40, aktiviertWissenXXProzent45, aktiviertWissenXXProzent45, aktiviertWissenXXProzent50, aktiviertWissenXXProzent50, aktiviertWissenXXProzent55, aktiviertWissenXXProzent55, aktiviertWissenXXProzent60, aktiviertWissenXXProzent60, aktiviertWissenXXProzent65, aktiviertWissenXXProzent65, aktiviertWissenXXProzent70, aktiviertWissenXXProzent70, aktiviertWissenXXProzent75, aktiviertWissenXXProzent75, aktiviertWissenXXProzent80, aktiviertWissenXXProzent80, aktiviertWissenXXProzent85, aktiviertWissenXXProzent85, aktiviertWissenXXProzent90, aktiviertWissenXXProzent90, aktiviertWissenXXProzent91, aktiviertWissenXXProzent91, aktiviertWissenXXProzent92, aktiviertWissenXXProzent92, aktiviertWissenXXProzent93, aktiviertWissenXXProzent93, aktiviertWissenXXProzent94, aktiviertWissenXXProzent94, aktiviertWissenXXProzent95, aktiviertWissenXXProzent95;




//Wissenstufe 1
// wenn irgendein Spieler Fortschritt x hat, aber noch nicht den Fortschritt y
if( ( GetPlrKnowledge(0,_BAL) || GetPlrKnowledge(1,_BAL) || GetPlrKnowledge(2,_BAL) || GetPlrKnowledge(3,_BAL) || GetPlrKnowledge(4,_BAL) || GetPlrKnowledge(5,_BAL) || GetPlrKnowledge(6,_BAL) || GetPlrKnowledge(7,_BAL) ) && 
( (!GetPlrKnowledge(0,_BAL)) || (!GetPlrKnowledge(1,_BAL)) || (!GetPlrKnowledge(2,_BAL)) || (!GetPlrKnowledge(3,_BAL)) || (!GetPlrKnowledge(4,_BAL)) || (!GetPlrKnowledge(5,_BAL)) || (!GetPlrKnowledge(6,_BAL)) || (!GetPlrKnowledge(7,_BAL)) ) )
{
if (Prozent5() && (!Prozent10()) && (aktiviertWissen1Prozent5==0) ) {aktiviertWissen1Prozent5=1;}
if (Prozent10() && (!Prozent15()) && (aktiviertWissen1Prozent10==0) ) {aktiviertWissen1Prozent10=1;}
if (Prozent15() && (!Prozent20()) && (aktiviertWissen1Prozent15==0) ) {aktiviertWissen1Prozent15=1;}
if (Prozent20() && (!Prozent25()) && (aktiviertWissen1Prozent20==0) ) {aktiviertWissen1Prozent20=1;}
if (Prozent25() && (!Prozent30()) && (aktiviertWissen1Prozent25==0) ) {aktiviertWissen1Prozent25=1;}
if (Prozent30() && (!Prozent35()) && (aktiviertWissen1Prozent30==0) ) {aktiviertWissen1Prozent30=1;}
if (Prozent35() && (!Prozent40()) && (aktiviertWissen1Prozent35==0) ) {aktiviertWissen1Prozent35=1;}
if (Prozent40() && (!Prozent45()) && (aktiviertWissen1Prozent40==0) ) {aktiviertWissen1Prozent40=1;}
if (Prozent45() && (!Prozent50()) && (aktiviertWissen1Prozent45==0) ) {aktiviertWissen1Prozent45=1;}
if (Prozent50() && (!Prozent55()) && (aktiviertWissen1Prozent50==0) ) {aktiviertWissen1Prozent50=1;}
if (Prozent55() && (!Prozent60()) && (aktiviertWissen1Prozent55==0) ) {aktiviertWissen1Prozent55=1;}
if (Prozent60() && (!Prozent65()) && (aktiviertWissen1Prozent60==0) ) {aktiviertWissen1Prozent60=1;}
if (Prozent65() && (!Prozent70()) && (aktiviertWissen1Prozent65==0) ) {aktiviertWissen1Prozent65=1;}
if (Prozent70() && (!Prozent75()) && (aktiviertWissen1Prozent70==0) ) {aktiviertWissen1Prozent70=1;}
if (Prozent75() && (!Prozent80()) && (aktiviertWissen1Prozent75==0) ) {aktiviertWissen1Prozent75=1;}
if (Prozent80() && (!Prozent85()) && (aktiviertWissen1Prozent80==0) ) {aktiviertWissen1Prozent80=1;}
if (Prozent85() && (!Prozent90()) && (aktiviertWissen1Prozent85==0) ) {aktiviertWissen1Prozent85=1;}
if (Prozent90() && (!Prozent91()) && (aktiviertWissen1Prozent90==0) ) {aktiviertWissen1Prozent90=1;}
if (Prozent91() && (!Prozent92()) && (aktiviertWissen1Prozent91==0) ) {aktiviertWissen1Prozent91=1;}
if (Prozent92() && (!Prozent93()) && (aktiviertWissen1Prozent92==0) ) {aktiviertWissen1Prozent92=1;}
if (Prozent93() && (!Prozent94()) && (aktiviertWissen1Prozent93==0) ) {aktiviertWissen1Prozent93=1;}
if (Prozent94() && (!Prozent95()) && (aktiviertWissen1Prozent94==0) ) {aktiviertWissen1Prozent94=1;}
if (Prozent95() && (aktiviertWissen1Prozent95==0) ) {aktiviertWissen1Prozent95=1;}
}


//Wissenstufe 2
// wenn irgendein Spieler Fortschritt x hat, aber noch nicht den Fortschritt y
if( ( GetPlrKnowledge(0,_BAL) || GetPlrKnowledge(1,_BAL) || GetPlrKnowledge(2,_BAL) || GetPlrKnowledge(3,_BAL) || GetPlrKnowledge(4,_BAL) || GetPlrKnowledge(5,_BAL) || GetPlrKnowledge(6,_BAL) || GetPlrKnowledge(7,_BAL) ) && 
( (!GetPlrKnowledge(0,_BAL)) || (!GetPlrKnowledge(1,_BAL)) || (!GetPlrKnowledge(2,_BAL)) || (!GetPlrKnowledge(3,_BAL)) || (!GetPlrKnowledge(4,_BAL)) || (!GetPlrKnowledge(5,_BAL)) || (!GetPlrKnowledge(6,_BAL)) || (!GetPlrKnowledge(7,_BAL)) ) )
{
if (Prozent5() && (!Prozent10()) && (aktiviertWissen2Prozent5==0) ) {aktiviertWissen2Prozent5=1;}
if (Prozent10() && (!Prozent15()) && (aktiviertWissen2Prozent10==0) ) {aktiviertWissen2Prozent10=1;}
if (Prozent15() && (!Prozent20()) && (aktiviertWissen2Prozent15==0) ) {aktiviertWissen2Prozent15=1;}
if (Prozent20() && (!Prozent25()) && (aktiviertWissen2Prozent20==0) ) {aktiviertWissen2Prozent20=1;}
if (Prozent25() && (!Prozent30()) && (aktiviertWissen2Prozent25==0) ) {aktiviertWissen2Prozent25=1;}
if (Prozent30() && (!Prozent35()) && (aktiviertWissen2Prozent30==0) ) {aktiviertWissen2Prozent30=1;}
if (Prozent35() && (!Prozent40()) && (aktiviertWissen2Prozent35==0) ) {aktiviertWissen2Prozent35=1;}
if (Prozent40() && (!Prozent45()) && (aktiviertWissen2Prozent40==0) ) {aktiviertWissen2Prozent40=1;}
if (Prozent45() && (!Prozent50()) && (aktiviertWissen2Prozent45==0) ) {aktiviertWissen2Prozent45=1;}
if (Prozent50() && (!Prozent55()) && (aktiviertWissen2Prozent50==0) ) {aktiviertWissen2Prozent50=1;}
if (Prozent55() && (!Prozent60()) && (aktiviertWissen2Prozent55==0) ) {aktiviertWissen2Prozent55=1;}
if (Prozent60() && (!Prozent65()) && (aktiviertWissen2Prozent60==0) ) {aktiviertWissen2Prozent60=1;}
if (Prozent65() && (!Prozent70()) && (aktiviertWissen2Prozent65==0) ) {aktiviertWissen2Prozent65=1;}
if (Prozent70() && (!Prozent75()) && (aktiviertWissen2Prozent70==0) ) {aktiviertWissen2Prozent70=1;}
if (Prozent75() && (!Prozent80()) && (aktiviertWissen2Prozent75==0) ) {aktiviertWissen2Prozent75=1;}
if (Prozent80() && (!Prozent85()) && (aktiviertWissen2Prozent80==0) ) {aktiviertWissen2Prozent80=1;}
if (Prozent85() && (!Prozent90()) && (aktiviertWissen2Prozent85==0) ) {aktiviertWissen2Prozent85=1;}
if (Prozent90() && (!Prozent91()) && (aktiviertWissen2Prozent90==0) ) {aktiviertWissen2Prozent90=1;}
if (Prozent91() && (!Prozent92()) && (aktiviertWissen2Prozent91==0) ) {aktiviertWissen2Prozent91=1;}
if (Prozent92() && (!Prozent93()) && (aktiviertWissen2Prozent92==0) ) {aktiviertWissen2Prozent92=1;}
if (Prozent93() && (!Prozent94()) && (aktiviertWissen2Prozent93==0) ) {aktiviertWissen2Prozent93=1;}
if (Prozent94() && (!Prozent95()) && (aktiviertWissen2Prozent94==0) ) {aktiviertWissen2Prozent94=1;}
if (Prozent95() && (aktiviertWissen2Prozent95==0) ) {aktiviertWissen2Prozent95=1;}
}


//Wissenstufe 3
// wenn irgendein Spieler Fortschritt x hat, aber noch nicht den Fortschritt y
if( ( GetPlrKnowledge(0,_BAL) || GetPlrKnowledge(1,_BAL) || GetPlrKnowledge(2,_BAL) || GetPlrKnowledge(3,_BAL) || GetPlrKnowledge(4,_BAL) || GetPlrKnowledge(5,_BAL) || GetPlrKnowledge(6,_BAL) || GetPlrKnowledge(7,_BAL) ) && 
( (!GetPlrKnowledge(0,_BAL)) || (!GetPlrKnowledge(1,_BAL)) || (!GetPlrKnowledge(2,_BAL)) || (!GetPlrKnowledge(3,_BAL)) || (!GetPlrKnowledge(4,_BAL)) || (!GetPlrKnowledge(5,_BAL)) || (!GetPlrKnowledge(6,_BAL)) || (!GetPlrKnowledge(7,_BAL)) ) )
{
if (Prozent5() && (!Prozent10()) && (aktiviertWissen3Prozent5==0) ) {aktiviertWissen3Prozent5=1;}
if (Prozent10() && (!Prozent15()) && (aktiviertWissen3Prozent10==0) ) {aktiviertWissen3Prozent10=1;}
if (Prozent15() && (!Prozent20()) && (aktiviertWissen3Prozent15==0) ) {aktiviertWissen3Prozent15=1;}
if (Prozent20() && (!Prozent25()) && (aktiviertWissen3Prozent20==0) ) {aktiviertWissen3Prozent20=1;}
if (Prozent25() && (!Prozent30()) && (aktiviertWissen3Prozent25==0) ) {aktiviertWissen3Prozent25=1;}
if (Prozent30() && (!Prozent35()) && (aktiviertWissen3Prozent30==0) ) {aktiviertWissen3Prozent30=1;}
if (Prozent35() && (!Prozent40()) && (aktiviertWissen3Prozent35==0) ) {aktiviertWissen3Prozent35=1;}
if (Prozent40() && (!Prozent45()) && (aktiviertWissen3Prozent40==0) ) {aktiviertWissen3Prozent40=1;}
if (Prozent45() && (!Prozent50()) && (aktiviertWissen3Prozent45==0) ) {aktiviertWissen3Prozent45=1;}
if (Prozent50() && (!Prozent55()) && (aktiviertWissen3Prozent50==0) ) {aktiviertWissen3Prozent50=1;}
if (Prozent55() && (!Prozent60()) && (aktiviertWissen3Prozent55==0) ) {aktiviertWissen3Prozent55=1;}
if (Prozent60() && (!Prozent65()) && (aktiviertWissen3Prozent60==0) ) {aktiviertWissen3Prozent60=1;}
if (Prozent65() && (!Prozent70()) && (aktiviertWissen3Prozent65==0) ) {aktiviertWissen3Prozent65=1;}
if (Prozent70() && (!Prozent75()) && (aktiviertWissen3Prozent70==0) ) {aktiviertWissen3Prozent70=1;}
if (Prozent75() && (!Prozent80()) && (aktiviertWissen3Prozent75==0) ) {aktiviertWissen3Prozent75=1;}
if (Prozent80() && (!Prozent85()) && (aktiviertWissen3Prozent80==0) ) {aktiviertWissen3Prozent80=1;}
if (Prozent85() && (!Prozent90()) && (aktiviertWissen3Prozent85==0) ) {aktiviertWissen3Prozent85=1;}
if (Prozent90() && (!Prozent91()) && (aktiviertWissen3Prozent90==0) ) {aktiviertWissen3Prozent90=1;}
if (Prozent91() && (!Prozent92()) && (aktiviertWissen3Prozent91==0) ) {aktiviertWissen3Prozent91=1;}
if (Prozent92() && (!Prozent93()) && (aktiviertWissen3Prozent92==0) ) {aktiviertWissen3Prozent92=1;}
if (Prozent93() && (!Prozent94()) && (aktiviertWissen3Prozent93==0) ) {aktiviertWissen3Prozent93=1;}
if (Prozent94() && (!Prozent95()) && (aktiviertWissen3Prozent94==0) ) {aktiviertWissen3Prozent94=1;}
if (Prozent95() && (aktiviertWissen3Prozent95==0) ) {aktiviertWissen3Prozent95=1;}
}



//Wissenstufe 4
// wenn irgendein Spieler Fortschritt x hat, aber noch nicht den Fortschritt y
if( ( GetPlrKnowledge(0,_BAL) || GetPlrKnowledge(1,_BAL) || GetPlrKnowledge(2,_BAL) || GetPlrKnowledge(3,_BAL) || GetPlrKnowledge(4,_BAL) || GetPlrKnowledge(5,_BAL) || GetPlrKnowledge(6,_BAL) || GetPlrKnowledge(7,_BAL) ) && 
( (!GetPlrKnowledge(0,_BAL)) || (!GetPlrKnowledge(1,_BAL)) || (!GetPlrKnowledge(2,_BAL)) || (!GetPlrKnowledge(3,_BAL)) || (!GetPlrKnowledge(4,_BAL)) || (!GetPlrKnowledge(5,_BAL)) || (!GetPlrKnowledge(6,_BAL)) || (!GetPlrKnowledge(7,_BAL)) ) )
{
if (Prozent5() && (!Prozent10()) && (aktiviertWissen4Prozent5==0) ) {aktiviertWissen4Prozent5=1;}
if (Prozent10() && (!Prozent15()) && (aktiviertWissen4Prozent10==0) ) {aktiviertWissen4Prozent10=1;}
if (Prozent15() && (!Prozent20()) && (aktiviertWissen4Prozent15==0) ) {aktiviertWissen4Prozent15=1;}
if (Prozent20() && (!Prozent25()) && (aktiviertWissen4Prozent20==0) ) {aktiviertWissen4Prozent20=1;}
if (Prozent25() && (!Prozent30()) && (aktiviertWissen4Prozent25==0) ) {aktiviertWissen4Prozent25=1;}
if (Prozent30() && (!Prozent35()) && (aktiviertWissen4Prozent30==0) ) {aktiviertWissen4Prozent30=1;}
if (Prozent35() && (!Prozent40()) && (aktiviertWissen4Prozent35==0) ) {aktiviertWissen4Prozent35=1;}
if (Prozent40() && (!Prozent45()) && (aktiviertWissen4Prozent40==0) ) {aktiviertWissen4Prozent40=1;}
if (Prozent45() && (!Prozent50()) && (aktiviertWissen4Prozent45==0) ) {aktiviertWissen4Prozent45=1;}
if (Prozent50() && (!Prozent55()) && (aktiviertWissen4Prozent50==0) ) {aktiviertWissen4Prozent50=1;}
if (Prozent55() && (!Prozent60()) && (aktiviertWissen4Prozent55==0) ) {aktiviertWissen4Prozent55=1;}
if (Prozent60() && (!Prozent65()) && (aktiviertWissen4Prozent60==0) ) {aktiviertWissen4Prozent60=1;}
if (Prozent65() && (!Prozent70()) && (aktiviertWissen4Prozent65==0) ) {aktiviertWissen4Prozent65=1;}
if (Prozent70() && (!Prozent75()) && (aktiviertWissen4Prozent70==0) ) {aktiviertWissen4Prozent70=1;}
if (Prozent75() && (!Prozent80()) && (aktiviertWissen4Prozent75==0) ) {aktiviertWissen4Prozent75=1;}
if (Prozent80() && (!Prozent85()) && (aktiviertWissen4Prozent80==0) ) {aktiviertWissen4Prozent80=1;}
if (Prozent85() && (!Prozent90()) && (aktiviertWissen4Prozent85==0) ) {aktiviertWissen4Prozent85=1;}
if (Prozent90() && (!Prozent91()) && (aktiviertWissen4Prozent90==0) ) {aktiviertWissen4Prozent90=1;}
if (Prozent91() && (!Prozent92()) && (aktiviertWissen4Prozent91==0) ) {aktiviertWissen4Prozent91=1;}
if (Prozent92() && (!Prozent93()) && (aktiviertWissen4Prozent92==0) ) {aktiviertWissen4Prozent92=1;}
if (Prozent93() && (!Prozent94()) && (aktiviertWissen4Prozent93==0) ) {aktiviertWissen4Prozent93=1;}
if (Prozent94() && (!Prozent95()) && (aktiviertWissen4Prozent94==0) ) {aktiviertWissen4Prozent94=1;}
if (Prozent95() && (aktiviertWissen4Prozent95==0) ) {aktiviertWissen4Prozent95=1;}
}



//Wissenstufe 5
// wenn irgendein Spieler Fortschritt x hat, aber noch nicht den Fortschritt y
if( ( GetPlrKnowledge(0,_BAL) || GetPlrKnowledge(1,_BAL) || GetPlrKnowledge(2,_BAL) || GetPlrKnowledge(3,_BAL) || GetPlrKnowledge(4,_BAL) || GetPlrKnowledge(5,_BAL) || GetPlrKnowledge(6,_BAL) || GetPlrKnowledge(7,_BAL) ) && 
( (!GetPlrKnowledge(0,_BAL)) || (!GetPlrKnowledge(1,_BAL)) || (!GetPlrKnowledge(2,_BAL)) || (!GetPlrKnowledge(3,_BAL)) || (!GetPlrKnowledge(4,_BAL)) || (!GetPlrKnowledge(5,_BAL)) || (!GetPlrKnowledge(6,_BAL)) || (!GetPlrKnowledge(7,_BAL)) ) )
{
if (Prozent5() && (!Prozent10()) && (aktiviertWissenXXProzent5==0) ) {aktiviertWissenXXProzent5=1;}
if (Prozent10() && (!Prozent15()) && (aktiviertWissenXXProzent10==0) ) {aktiviertWissenXXProzent10=1;}
if (Prozent15() && (!Prozent20()) && (aktiviertWissenXXProzent15==0) ) {aktiviertWissenXXProzent15=1;}
if (Prozent20() && (!Prozent25()) && (aktiviertWissenXXProzent20==0) ) {aktiviertWissenXXProzent20=1;}
if (Prozent25() && (!Prozent30()) && (aktiviertWissenXXProzent25==0) ) {aktiviertWissenXXProzent25=1;}
if (Prozent30() && (!Prozent35()) && (aktiviertWissenXXProzent30==0) ) {aktiviertWissenXXProzent30=1;}
if (Prozent35() && (!Prozent40()) && (aktiviertWissenXXProzent35==0) ) {aktiviertWissenXXProzent35=1;}
if (Prozent40() && (!Prozent45()) && (aktiviertWissenXXProzent40==0) ) {aktiviertWissenXXProzent40=1;}
if (Prozent45() && (!Prozent50()) && (aktiviertWissenXXProzent45==0) ) {aktiviertWissenXXProzent45=1;}
if (Prozent50() && (!Prozent55()) && (aktiviertWissenXXProzent50==0) ) {aktiviertWissenXXProzent50=1;}
if (Prozent55() && (!Prozent60()) && (aktiviertWissenXXProzent55==0) ) {aktiviertWissenXXProzent55=1;}
if (Prozent60() && (!Prozent65()) && (aktiviertWissenXXProzent60==0) ) {aktiviertWissenXXProzent60=1;}
if (Prozent65() && (!Prozent70()) && (aktiviertWissenXXProzent65==0) ) {aktiviertWissenXXProzent65=1;}
if (Prozent70() && (!Prozent75()) && (aktiviertWissenXXProzent70==0) ) {aktiviertWissenXXProzent70=1;}
if (Prozent75() && (!Prozent80()) && (aktiviertWissenXXProzent75==0) ) {aktiviertWissenXXProzent75=1;}
if (Prozent80() && (!Prozent85()) && (aktiviertWissenXXProzent80==0) ) {aktiviertWissenXXProzent80=1;}
if (Prozent85() && (!Prozent90()) && (aktiviertWissenXXProzent85==0) ) {aktiviertWissenXXProzent85=1;}
if (Prozent90() && (!Prozent91()) && (aktiviertWissenXXProzent90==0) ) {aktiviertWissenXXProzent90=1;}
if (Prozent91() && (!Prozent92()) && (aktiviertWissenXXProzent91==0) ) {aktiviertWissenXXProzent91=1;}
if (Prozent92() && (!Prozent93()) && (aktiviertWissenXXProzent92==0) ) {aktiviertWissenXXProzent92=1;}
if (Prozent93() && (!Prozent94()) && (aktiviertWissenXXProzent93==0) ) {aktiviertWissenXXProzent93=1;}
if (Prozent94() && (!Prozent95()) && (aktiviertWissenXXProzent94==0) ) {aktiviertWissenXXProzent94=1;}
if (Prozent95() && (aktiviertWissenXXProzent95==0) ) {aktiviertWissenXXProzent95=1;}
}



//Wissenstufe 6
// wenn irgendein Spieler Fortschritt x hat, aber noch nicht den Fortschritt y
if( ( GetPlrKnowledge(0,_BAL) || GetPlrKnowledge(1,_BAL) || GetPlrKnowledge(2,_BAL) || GetPlrKnowledge(3,_BAL) || GetPlrKnowledge(4,_BAL) || GetPlrKnowledge(5,_BAL) || GetPlrKnowledge(6,_BAL) || GetPlrKnowledge(7,_BAL) ) && 
( (!GetPlrKnowledge(0,_BAL)) || (!GetPlrKnowledge(1,_BAL)) || (!GetPlrKnowledge(2,_BAL)) || (!GetPlrKnowledge(3,_BAL)) || (!GetPlrKnowledge(4,_BAL)) || (!GetPlrKnowledge(5,_BAL)) || (!GetPlrKnowledge(6,_BAL)) || (!GetPlrKnowledge(7,_BAL)) ) )
{
if (Prozent5() && (!Prozent10()) && (aktiviertWissenXXProzent5==0) ) {aktiviertWissenXXProzent5=1;}
if (Prozent10() && (!Prozent15()) && (aktiviertWissenXXProzent10==0) ) {aktiviertWissenXXProzent10=1;}
if (Prozent15() && (!Prozent20()) && (aktiviertWissenXXProzent15==0) ) {aktiviertWissenXXProzent15=1;}
if (Prozent20() && (!Prozent25()) && (aktiviertWissenXXProzent20==0) ) {aktiviertWissenXXProzent20=1;}
if (Prozent25() && (!Prozent30()) && (aktiviertWissenXXProzent25==0) ) {aktiviertWissenXXProzent25=1;}
if (Prozent30() && (!Prozent35()) && (aktiviertWissenXXProzent30==0) ) {aktiviertWissenXXProzent30=1;}
if (Prozent35() && (!Prozent40()) && (aktiviertWissenXXProzent35==0) ) {aktiviertWissenXXProzent35=1;}
if (Prozent40() && (!Prozent45()) && (aktiviertWissenXXProzent40==0) ) {aktiviertWissenXXProzent40=1;}
if (Prozent45() && (!Prozent50()) && (aktiviertWissenXXProzent45==0) ) {aktiviertWissenXXProzent45=1;}
if (Prozent50() && (!Prozent55()) && (aktiviertWissenXXProzent50==0) ) {aktiviertWissenXXProzent50=1;}
if (Prozent55() && (!Prozent60()) && (aktiviertWissenXXProzent55==0) ) {aktiviertWissenXXProzent55=1;}
if (Prozent60() && (!Prozent65()) && (aktiviertWissenXXProzent60==0) ) {aktiviertWissenXXProzent60=1;}
if (Prozent65() && (!Prozent70()) && (aktiviertWissenXXProzent65==0) ) {aktiviertWissenXXProzent65=1;}
if (Prozent70() && (!Prozent75()) && (aktiviertWissenXXProzent70==0) ) {aktiviertWissenXXProzent70=1;}
if (Prozent75() && (!Prozent80()) && (aktiviertWissenXXProzent75==0) ) {aktiviertWissenXXProzent75=1;}
if (Prozent80() && (!Prozent85()) && (aktiviertWissenXXProzent80==0) ) {aktiviertWissenXXProzent80=1;}
if (Prozent85() && (!Prozent90()) && (aktiviertWissenXXProzent85==0) ) {aktiviertWissenXXProzent85=1;}
if (Prozent90() && (!Prozent91()) && (aktiviertWissenXXProzent90==0) ) {aktiviertWissenXXProzent90=1;}
if (Prozent91() && (!Prozent92()) && (aktiviertWissenXXProzent91==0) ) {aktiviertWissenXXProzent91=1;}
if (Prozent92() && (!Prozent93()) && (aktiviertWissenXXProzent92==0) ) {aktiviertWissenXXProzent92=1;}
if (Prozent93() && (!Prozent94()) && (aktiviertWissenXXProzent93==0) ) {aktiviertWissenXXProzent93=1;}
if (Prozent94() && (!Prozent95()) && (aktiviertWissenXXProzent94==0) ) {aktiviertWissenXXProzent94=1;}
if (Prozent95() && (aktiviertWissenXXProzent95==0) ) {aktiviertWissenXXProzent95=1;}
}



//Wissenstufe 7
// wenn irgendein Spieler Fortschritt x hat, aber noch nicht den Fortschritt y
if( ( GetPlrKnowledge(0,_BAL) || GetPlrKnowledge(1,_BAL) || GetPlrKnowledge(2,_BAL) || GetPlrKnowledge(3,_BAL) || GetPlrKnowledge(4,_BAL) || GetPlrKnowledge(5,_BAL) || GetPlrKnowledge(6,_BAL) || GetPlrKnowledge(7,_BAL) ) && 
( (!GetPlrKnowledge(0,_BAL)) || (!GetPlrKnowledge(1,_BAL)) || (!GetPlrKnowledge(2,_BAL)) || (!GetPlrKnowledge(3,_BAL)) || (!GetPlrKnowledge(4,_BAL)) || (!GetPlrKnowledge(5,_BAL)) || (!GetPlrKnowledge(6,_BAL)) || (!GetPlrKnowledge(7,_BAL)) ) )
{
if (Prozent5() && (!Prozent10()) && (aktiviertWissenXXProzent5==0) ) {aktiviertWissenXXProzent5=1;}
if (Prozent10() && (!Prozent15()) && (aktiviertWissenXXProzent10==0) ) {aktiviertWissenXXProzent10=1;}
if (Prozent15() && (!Prozent20()) && (aktiviertWissenXXProzent15==0) ) {aktiviertWissenXXProzent15=1;}
if (Prozent20() && (!Prozent25()) && (aktiviertWissenXXProzent20==0) ) {aktiviertWissenXXProzent20=1;}
if (Prozent25() && (!Prozent30()) && (aktiviertWissenXXProzent25==0) ) {aktiviertWissenXXProzent25=1;}
if (Prozent30() && (!Prozent35()) && (aktiviertWissenXXProzent30==0) ) {aktiviertWissenXXProzent30=1;}
if (Prozent35() && (!Prozent40()) && (aktiviertWissenXXProzent35==0) ) {aktiviertWissenXXProzent35=1;}
if (Prozent40() && (!Prozent45()) && (aktiviertWissenXXProzent40==0) ) {aktiviertWissenXXProzent40=1;}
if (Prozent45() && (!Prozent50()) && (aktiviertWissenXXProzent45==0) ) {aktiviertWissenXXProzent45=1;}
if (Prozent50() && (!Prozent55()) && (aktiviertWissenXXProzent50==0) ) {aktiviertWissenXXProzent50=1;}
if (Prozent55() && (!Prozent60()) && (aktiviertWissenXXProzent55==0) ) {aktiviertWissenXXProzent55=1;}
if (Prozent60() && (!Prozent65()) && (aktiviertWissenXXProzent60==0) ) {aktiviertWissenXXProzent60=1;}
if (Prozent65() && (!Prozent70()) && (aktiviertWissenXXProzent65==0) ) {aktiviertWissenXXProzent65=1;}
if (Prozent70() && (!Prozent75()) && (aktiviertWissenXXProzent70==0) ) {aktiviertWissenXXProzent70=1;}
if (Prozent75() && (!Prozent80()) && (aktiviertWissenXXProzent75==0) ) {aktiviertWissenXXProzent75=1;}
if (Prozent80() && (!Prozent85()) && (aktiviertWissenXXProzent80==0) ) {aktiviertWissenXXProzent80=1;}
if (Prozent85() && (!Prozent90()) && (aktiviertWissenXXProzent85==0) ) {aktiviertWissenXXProzent85=1;}
if (Prozent90() && (!Prozent91()) && (aktiviertWissenXXProzent90==0) ) {aktiviertWissenXXProzent90=1;}
if (Prozent91() && (!Prozent92()) && (aktiviertWissenXXProzent91==0) ) {aktiviertWissenXXProzent91=1;}
if (Prozent92() && (!Prozent93()) && (aktiviertWissenXXProzent92==0) ) {aktiviertWissenXXProzent92=1;}
if (Prozent93() && (!Prozent94()) && (aktiviertWissenXXProzent93==0) ) {aktiviertWissenXXProzent93=1;}
if (Prozent94() && (!Prozent95()) && (aktiviertWissenXXProzent94==0) ) {aktiviertWissenXXProzent94=1;}
if (Prozent95() && (aktiviertWissenXXProzent95==0) ) {aktiviertWissenXXProzent95=1;}
}



//Wissenstufe 8
// wenn irgendein Spieler Fortschritt x hat, aber noch nicht den Fortschritt y
if( ( GetPlrKnowledge(0,_BAL) || GetPlrKnowledge(1,_BAL) || GetPlrKnowledge(2,_BAL) || GetPlrKnowledge(3,_BAL) || GetPlrKnowledge(4,_BAL) || GetPlrKnowledge(5,_BAL) || GetPlrKnowledge(6,_BAL) || GetPlrKnowledge(7,_BAL) ) && 
( (!GetPlrKnowledge(0,_BAL)) || (!GetPlrKnowledge(1,_BAL)) || (!GetPlrKnowledge(2,_BAL)) || (!GetPlrKnowledge(3,_BAL)) || (!GetPlrKnowledge(4,_BAL)) || (!GetPlrKnowledge(5,_BAL)) || (!GetPlrKnowledge(6,_BAL)) || (!GetPlrKnowledge(7,_BAL)) ) )
{
if (Prozent5() && (!Prozent10()) && (aktiviertWissenXXProzent5==0) ) {aktiviertWissenXXProzent5=1;}
if (Prozent10() && (!Prozent15()) && (aktiviertWissenXXProzent10==0) ) {aktiviertWissenXXProzent10=1;}
if (Prozent15() && (!Prozent20()) && (aktiviertWissenXXProzent15==0) ) {aktiviertWissenXXProzent15=1;}
if (Prozent20() && (!Prozent25()) && (aktiviertWissenXXProzent20==0) ) {aktiviertWissenXXProzent20=1;}
if (Prozent25() && (!Prozent30()) && (aktiviertWissenXXProzent25==0) ) {aktiviertWissenXXProzent25=1;}
if (Prozent30() && (!Prozent35()) && (aktiviertWissenXXProzent30==0) ) {aktiviertWissenXXProzent30=1;}
if (Prozent35() && (!Prozent40()) && (aktiviertWissenXXProzent35==0) ) {aktiviertWissenXXProzent35=1;}
if (Prozent40() && (!Prozent45()) && (aktiviertWissenXXProzent40==0) ) {aktiviertWissenXXProzent40=1;}
if (Prozent45() && (!Prozent50()) && (aktiviertWissenXXProzent45==0) ) {aktiviertWissenXXProzent45=1;}
if (Prozent50() && (!Prozent55()) && (aktiviertWissenXXProzent50==0) ) {aktiviertWissenXXProzent50=1;}
if (Prozent55() && (!Prozent60()) && (aktiviertWissenXXProzent55==0) ) {aktiviertWissenXXProzent55=1;}
if (Prozent60() && (!Prozent65()) && (aktiviertWissenXXProzent60==0) ) {aktiviertWissenXXProzent60=1;}
if (Prozent65() && (!Prozent70()) && (aktiviertWissenXXProzent65==0) ) {aktiviertWissenXXProzent65=1;}
if (Prozent70() && (!Prozent75()) && (aktiviertWissenXXProzent70==0) ) {aktiviertWissenXXProzent70=1;}
if (Prozent75() && (!Prozent80()) && (aktiviertWissenXXProzent75==0) ) {aktiviertWissenXXProzent75=1;}
if (Prozent80() && (!Prozent85()) && (aktiviertWissenXXProzent80==0) ) {aktiviertWissenXXProzent80=1;}
if (Prozent85() && (!Prozent90()) && (aktiviertWissenXXProzent85==0) ) {aktiviertWissenXXProzent85=1;}
if (Prozent90() && (!Prozent91()) && (aktiviertWissenXXProzent90==0) ) {aktiviertWissenXXProzent90=1;}
if (Prozent91() && (!Prozent92()) && (aktiviertWissenXXProzent91==0) ) {aktiviertWissenXXProzent91=1;}
if (Prozent92() && (!Prozent93()) && (aktiviertWissenXXProzent92==0) ) {aktiviertWissenXXProzent92=1;}
if (Prozent93() && (!Prozent94()) && (aktiviertWissenXXProzent93==0) ) {aktiviertWissenXXProzent93=1;}
if (Prozent94() && (!Prozent95()) && (aktiviertWissenXXProzent94==0) ) {aktiviertWissenXXProzent94=1;}
if (Prozent95() && (aktiviertWissenXXProzent95==0) ) {aktiviertWissenXXProzent95=1;}
}



//Wissenstufe 9
// wenn irgendein Spieler Fortschritt x hat, aber noch nicht den Fortschritt y
if( ( GetPlrKnowledge(0,_BAL) || GetPlrKnowledge(1,_BAL) || GetPlrKnowledge(2,_BAL) || GetPlrKnowledge(3,_BAL) || GetPlrKnowledge(4,_BAL) || GetPlrKnowledge(5,_BAL) || GetPlrKnowledge(6,_BAL) || GetPlrKnowledge(7,_BAL) ) && 
( (!GetPlrKnowledge(0,_BAL)) || (!GetPlrKnowledge(1,_BAL)) || (!GetPlrKnowledge(2,_BAL)) || (!GetPlrKnowledge(3,_BAL)) || (!GetPlrKnowledge(4,_BAL)) || (!GetPlrKnowledge(5,_BAL)) || (!GetPlrKnowledge(6,_BAL)) || (!GetPlrKnowledge(7,_BAL)) ) )
{
if (Prozent5() && (!Prozent10()) && (aktiviertWissenXXProzent5==0) ) {aktiviertWissenXXProzent5=1;}
if (Prozent10() && (!Prozent15()) && (aktiviertWissenXXProzent10==0) ) {aktiviertWissenXXProzent10=1;}
if (Prozent15() && (!Prozent20()) && (aktiviertWissenXXProzent15==0) ) {aktiviertWissenXXProzent15=1;}
if (Prozent20() && (!Prozent25()) && (aktiviertWissenXXProzent20==0) ) {aktiviertWissenXXProzent20=1;}
if (Prozent25() && (!Prozent30()) && (aktiviertWissenXXProzent25==0) ) {aktiviertWissenXXProzent25=1;}
if (Prozent30() && (!Prozent35()) && (aktiviertWissenXXProzent30==0) ) {aktiviertWissenXXProzent30=1;}
if (Prozent35() && (!Prozent40()) && (aktiviertWissenXXProzent35==0) ) {aktiviertWissenXXProzent35=1;}
if (Prozent40() && (!Prozent45()) && (aktiviertWissenXXProzent40==0) ) {aktiviertWissenXXProzent40=1;}
if (Prozent45() && (!Prozent50()) && (aktiviertWissenXXProzent45==0) ) {aktiviertWissenXXProzent45=1;}
if (Prozent50() && (!Prozent55()) && (aktiviertWissenXXProzent50==0) ) {aktiviertWissenXXProzent50=1;}
if (Prozent55() && (!Prozent60()) && (aktiviertWissenXXProzent55==0) ) {aktiviertWissenXXProzent55=1;}
if (Prozent60() && (!Prozent65()) && (aktiviertWissenXXProzent60==0) ) {aktiviertWissenXXProzent60=1;}
if (Prozent65() && (!Prozent70()) && (aktiviertWissenXXProzent65==0) ) {aktiviertWissenXXProzent65=1;}
if (Prozent70() && (!Prozent75()) && (aktiviertWissenXXProzent70==0) ) {aktiviertWissenXXProzent70=1;}
if (Prozent75() && (!Prozent80()) && (aktiviertWissenXXProzent75==0) ) {aktiviertWissenXXProzent75=1;}
if (Prozent80() && (!Prozent85()) && (aktiviertWissenXXProzent80==0) ) {aktiviertWissenXXProzent80=1;}
if (Prozent85() && (!Prozent90()) && (aktiviertWissenXXProzent85==0) ) {aktiviertWissenXXProzent85=1;}
if (Prozent90() && (!Prozent91()) && (aktiviertWissenXXProzent90==0) ) {aktiviertWissenXXProzent90=1;}
if (Prozent91() && (!Prozent92()) && (aktiviertWissenXXProzent91==0) ) {aktiviertWissenXXProzent91=1;}
if (Prozent92() && (!Prozent93()) && (aktiviertWissenXXProzent92==0) ) {aktiviertWissenXXProzent92=1;}
if (Prozent93() && (!Prozent94()) && (aktiviertWissenXXProzent93==0) ) {aktiviertWissenXXProzent93=1;}
if (Prozent94() && (!Prozent95()) && (aktiviertWissenXXProzent94==0) ) {aktiviertWissenXXProzent94=1;}
if (Prozent95() && (aktiviertWissenXXProzent95==0) ) {aktiviertWissenXXProzent95=1;}
}



//Wissenstufe 10
// wenn irgendein Spieler Fortschritt x hat, aber noch nicht den Fortschritt y
if( ( GetPlrKnowledge(0,_BAL) || GetPlrKnowledge(1,_BAL) || GetPlrKnowledge(2,_BAL) || GetPlrKnowledge(3,_BAL) || GetPlrKnowledge(4,_BAL) || GetPlrKnowledge(5,_BAL) || GetPlrKnowledge(6,_BAL) || GetPlrKnowledge(7,_BAL) ) && 
( (!GetPlrKnowledge(0,_BAL)) || (!GetPlrKnowledge(1,_BAL)) || (!GetPlrKnowledge(2,_BAL)) || (!GetPlrKnowledge(3,_BAL)) || (!GetPlrKnowledge(4,_BAL)) || (!GetPlrKnowledge(5,_BAL)) || (!GetPlrKnowledge(6,_BAL)) || (!GetPlrKnowledge(7,_BAL)) ) )
{
if (Prozent5() && (!Prozent10()) && (aktiviertWissenXXProzent5==0) ) {aktiviertWissenXXProzent5=1;}
if (Prozent10() && (!Prozent15()) && (aktiviertWissenXXProzent10==0) ) {aktiviertWissenXXProzent10=1;}
if (Prozent15() && (!Prozent20()) && (aktiviertWissenXXProzent15==0) ) {aktiviertWissenXXProzent15=1;}
if (Prozent20() && (!Prozent25()) && (aktiviertWissenXXProzent20==0) ) {aktiviertWissenXXProzent20=1;}
if (Prozent25() && (!Prozent30()) && (aktiviertWissenXXProzent25==0) ) {aktiviertWissenXXProzent25=1;}
if (Prozent30() && (!Prozent35()) && (aktiviertWissenXXProzent30==0) ) {aktiviertWissenXXProzent30=1;}
if (Prozent35() && (!Prozent40()) && (aktiviertWissenXXProzent35==0) ) {aktiviertWissenXXProzent35=1;}
if (Prozent40() && (!Prozent45()) && (aktiviertWissenXXProzent40==0) ) {aktiviertWissenXXProzent40=1;}
if (Prozent45() && (!Prozent50()) && (aktiviertWissenXXProzent45==0) ) {aktiviertWissenXXProzent45=1;}
if (Prozent50() && (!Prozent55()) && (aktiviertWissenXXProzent50==0) ) {aktiviertWissenXXProzent50=1;}
if (Prozent55() && (!Prozent60()) && (aktiviertWissenXXProzent55==0) ) {aktiviertWissenXXProzent55=1;}
if (Prozent60() && (!Prozent65()) && (aktiviertWissenXXProzent60==0) ) {aktiviertWissenXXProzent60=1;}
if (Prozent65() && (!Prozent70()) && (aktiviertWissenXXProzent65==0) ) {aktiviertWissenXXProzent65=1;}
if (Prozent70() && (!Prozent75()) && (aktiviertWissenXXProzent70==0) ) {aktiviertWissenXXProzent70=1;}
if (Prozent75() && (!Prozent80()) && (aktiviertWissenXXProzent75==0) ) {aktiviertWissenXXProzent75=1;}
if (Prozent80() && (!Prozent85()) && (aktiviertWissenXXProzent80==0) ) {aktiviertWissenXXProzent80=1;}
if (Prozent85() && (!Prozent90()) && (aktiviertWissenXXProzent85==0) ) {aktiviertWissenXXProzent85=1;}
if (Prozent90() && (!Prozent91()) && (aktiviertWissenXXProzent90==0) ) {aktiviertWissenXXProzent90=1;}
if (Prozent91() && (!Prozent92()) && (aktiviertWissenXXProzent91==0) ) {aktiviertWissenXXProzent91=1;}
if (Prozent92() && (!Prozent93()) && (aktiviertWissenXXProzent92==0) ) {aktiviertWissenXXProzent92=1;}
if (Prozent93() && (!Prozent94()) && (aktiviertWissenXXProzent93==0) ) {aktiviertWissenXXProzent93=1;}
if (Prozent94() && (!Prozent95()) && (aktiviertWissenXXProzent94==0) ) {aktiviertWissenXXProzent94=1;}
if (Prozent95() && (aktiviertWissenXXProzent95==0) ) {aktiviertWissenXXProzent95=1;}
}

}

*/





/*
//wird nicht gebraucht
public func Percentage(c)
{
  if (count==1) return(0);        // alles abzubauen -> kein Rest
  if (count==2) return(c/10);     // 90% abzubauen
  if (count==3) return(c/4);      // 75% abzubauen
  if (count==4) return(c/2);      // 50% abzubauen
  if (count==5) return(c*3/4);    // 25% abzubauen
                return(c*9/10);   // 10% abzubauen
}

public func IsFulfilled()
{
  // noch gar nicht gezählt?
  var pix = MatPixelCount();
  if (pix == -1) return(0);
  // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels)
  {
    // Materialpixel +150, vermutlich weil nicht jeder einzelne Pixel weggesprengt werden kann, deshalb reicht es,  wenn es bei 149 pixel beim komplettabbau schon erfüllt ist.
    pixels = Percentage(pix) + 150;
    // Materialstücke
    objs = Percentage(MatObjsCount());
  }
  // noch zu viel?
  if (pix > pixels)          return(0);
  if (MatObjsCount() > objs) return(0);
  return(1);
}
*/




