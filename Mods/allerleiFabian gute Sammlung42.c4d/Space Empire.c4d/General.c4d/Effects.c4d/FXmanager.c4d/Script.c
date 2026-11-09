/*-- FX Manager --*/

#strict

static fx_bitmask;      //     1 = Effekt: Sternenhimmel
                        //     2 = Effekt: Hintergrund-Landschaften
                        //     4 = Effekt: Atmosphärische Nacht
                        //   512 = Present Bit (immer gesetzt)
                        //  1024 = Cheat: mehr Kombinationsstufen für Bazooka
                        // 65536 = Debug: Testlicht für Gebäude

local starlight;        // Helligkeit des nächtlichen Sternenhimmels
local bgl_ID, bgl_y;    // ID und y-Position der Hintergrund-Landschaft (0 wenn keine benutzt)


/* Initialisierung */

protected func Initialize()
{
  SetAction("Init");
}

protected func PostInit()
{
  // Startwert vom ersten Spieler holen
  fx_bitmask = GetPlrExtraData(GetPlayerByIndex(), "SE_fx_bitmask") & 1023;   // ab 1024 Cheats
  // Present Bit nicht gesetzt, dann Defaults herstellen
  if (!(fx_bitmask & 512))
    fx_bitmask = 519;
  // Effekte aufbauen
  if (fx_bitmask & 1)  CreateStars();
  if (fx_bitmask & 2)  CreateBGL();
    // ...bei (fx_bitmask & 4) nichts zu tun
  // Cheats, Eastereggs, Debug
  AddMsgBoardCmd("cheat:fairbleiben", "FindObject(SFXM)->DeactivateCheats()");
  AddMsgBoardCmd("cheat:ballerorgie", "FindObject(SFXM)->ActivateCheat(1024)");
  AddMsgBoardCmd("debug:licht",       "FindObject(SFXM)->ActivateCheat(65536)");
}


/* Wechsel der Effekte */

public func SwitchFX(bitvalue)
{
  // Cheat-Bits ignorieren
  bitvalue = bitvalue & 1023;
  // bitweises XOR zum Invertieren der betroffenen Bits
  fx_bitmask = fx_bitmask ^ bitvalue;
  // Sternenhimmel
  if (bitvalue & 1)
    if (fx_bitmask & 1)  CreateStars();
                   else  RemoveStars();
  // Hintergrund-Landschaft
  if (bitvalue & 2)
    if (fx_bitmask & 2)
    {
      CreateBGL();
      if (!bgl_ID) Log("Das Szenario unterstützt keine Hintergrund-Landschaft!");
    }
    else RemoveBGL();
  // Atmosphärische Nacht
  if (bitvalue & 4)
    if (!FindObject(TIME))
      Log("Das Szenario unterstützt keinen Tag/Nachtwechsel!");
  // Helligkeit neu setzen
  var timeobj = FindObject(TIME);
  if (timeobj)
//    timeobj->RefreshScreen();
timeobj->RestoreColors();
}


/* Cheats, Eastereggs, Debug */

public func ActivateCheat(bitvalue)
{
  // untere Bits ignorieren
  bitvalue = bitvalue & 4294966273;
  // Reaktion
  if (bitvalue & 1024)
    Message("Bazooka-Cheat aktiviert!|Viel Spaß beim Ballern!");
  if (bitvalue & 65536)
  {
    var light;
    while (light = FindObject(LGHT,0,0,0,0,0,0,0,0,light))
      SetGraphics("Test", light);
  }
  // Bits setzen
  fx_bitmask = fx_bitmask | bitvalue;
}

public func DeactivateCheats()
{
  // Meldung ausgeben
  Message("Alle Cheats deaktiviert!");
  // Cheat-Bits löschen
  fx_bitmask = fx_bitmask & 1023;
}


/* Nachthimmel */

private func CreateStars()
{
  // Sternenhimmel schon da?
  if (starlight) return(1);
  // ist es noch Tag?
//  if (DayDarkness < 128) return(1);
 if (IsDay()||IsVeryDay()) return(1);
  // Sternenhimmel aufbauen
  var x, y;
  for (x = 0; x < LandscapeWidth(); x += 300)
    for (y = 0; y < LandscapeHeight(); y += 300)
      SetPosition(x, y, CreateObject(SSKY,0,0,-1));
  // spezielle Sternenplatte mit Aussparung für den Mond
  var moon = FindObject(SMON);
  FindObject(SSKY, GetX(moon)-300, GetY(moon)-300, 300, 300)->SetStarPhase(8);
  starlight = 1;
  // Hintergrund-Landschaft neu aufbauen
  if (fx_bitmask & 2)
    CreateBGL();
}

private func RemoveStars()
{
  // nicht wenn eh keine Sterne da
  if (!starlight) return(1);
  // Sternenhimmel löschen
  var obj;
  while (obj = FindObject(SSKY))
    RemoveObject(obj);
  // Mond ausblenden
  obj = FindObject(SMON);
  if (obj)
    obj->Disappear();
  starlight = 0;
}

private func SetStarlight(value)
{
  // Sterne
  var obj;
  while (obj = FindObject(SSKY, 0,0,0,0, 0,0,0,0, obj))
    obj->SetLightness(value);
  // Mond
  obj = FindObject(SMON);
  if (obj)
    obj->SetLightness(value);
  starlight = value;
}


/* Hintergrund-Landschaft */

private func CreateBGL()
{
  // keine Landschaft benutzt?
  if (!bgl_ID) return(1);
  // alte Landschaft erst loswerden
  RemoveAll(bgl_ID);
  // Hintergrund neu aufbauen
  var x;
  var xplus = GetDefWidth(bgl_ID);
  for (x; x < LandscapeWidth(); x += xplus)
    SetPosition(x, bgl_y, CreateObject(bgl_ID,0,0,-1));
  // Helligkeit neu setzen
  var timeobj = FindObject(TIME);
  if (timeobj)
 //   timeobj->RefreshScreen();
timeobj->RestoreColors();
}

private func RemoveBGL()
{
  // keine Landschaft benutzt?
  if (!bgl_ID) return(1);
  // alte Hintergrund-Landschaft löschen
  RemoveAll(bgl_ID);
}


/* Aufruf vom Tag/Nacht-Objekt bei Helligkeitsänderungen */

public func SetLightness(light)
{
  // Verdunklung der Landschaft bei Nacht
  if (bgl_ID && (fx_bitmask & 2))
  {
    var color = RGB(128+light/2, 128+light/2, 128+light/2);
    var obj;
    while (obj = FindObject(bgl_ID, 0,0,0,0, 0,0,0,0, obj))
      SetClrModulation(color, obj);
  }
  // Sterne & Mond
  if (fx_bitmask & 1)
  {
    if (light < 128)
    {
      if (!starlight) CreateStars();
      SetStarlight(255-light*2);
      return(1);
    }
    if (starlight) RemoveStars();
  }
}

