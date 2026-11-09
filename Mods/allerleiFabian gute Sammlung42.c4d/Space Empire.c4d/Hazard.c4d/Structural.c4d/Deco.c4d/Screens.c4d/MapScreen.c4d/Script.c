/*-- Landkartenanzeiger --*/

#strict 3

static const MAP_ZoomFactor = 10;

static MAP_ScreenWidth, MAP_ScreenHeight;
static MAP_MasterScreen;

local MapX, MapY, lClonk;


func Completion() {
SetCategory(C4D_StaticBack);
  // Erstmal warten
  ScheduleCall(this(), "Initialized", 1);
}


protected func Initialized()
{
  // Noch kein MasterScreen gesetzt?
  if(!MAP_MasterScreen)
    {
    // Ich werde zum MasterScreen
    MAP_MasterScreen = this();
    // Landschaftsmaße ermitteln
    LandscapeWH();
    }
  // MasterScreen belegt, diesen Bildschirm initialisieren
  InitScreen();
  // Braucht eigentlich nur der MasterScreen
  MapX = (MAP_ZoomFactor ?? 0) / 2;
  MapY = (MAP_ZoomFactor ?? 0) / 2;
}

/* Slave-Funktionen */

// Alles was der Bildschirm nur für sich macht

private func InitScreen()
{
  // Größe anpassen
  SetShape(-(MAP_ScreenWidth ?? 0)/2, -(MAP_ScreenHeight ?? 0)/2, MAP_ScreenWidth, MAP_ScreenHeight);
  // Rahmen setzen
  var pFrame = AddFrame();
  pFrame->PanelColor(RGBa(1,1,1,200));
  pFrame->FrameColor(RGBa(255,255,255));
  // Clonkanzeige setzen
  ScheduleCall(this(), "ShowClonks", 1);
}

private func ScreenLandscape()
{
  // MasterScreen weg?
  if(!MAP_MasterScreen)
    MAP_MasterScreen = this();
  // Bin ich der MasterScreen?
  if(!Equal(MAP_MasterScreen, this()))
    return;
  // Alle vorhandenen Bildscbirme speichern
  var screens = CreateArray();
  screens = FindObjects(Find_ID(GetID()));
  // Drei Landschaftspixel scannen
  for(var j=nil,mat,matC ; (j ?? 0)<3 ; S42MigrationStep(j, 1, true))
    {
    mat = GetMaterial(AbsX(MapX), AbsY(MapY));
    matC = RGB(GetMaterialColor(mat, 1, nil), GetMaterialColor(mat, 1, 1), GetMaterialColor(mat, 1, 2));
    // An alle Bildschirme verteilen
    for(var i=nil ; (i ?? 0) < (GetLength(screens) ?? 0) ; S42MigrationStep(i, 1, true))
      screens[(i ?? 0)]->DrawPixel(((MapX ?? 0)-(MAP_ZoomFactor ?? 0)/2) / (MAP_ZoomFactor ?? 0), ((MapY ?? 0)-(MAP_ZoomFactor ?? 0)/2) / (MAP_ZoomFactor ?? 0), matC);
    // MapX und MapY erhöhen
    S42MigrationAssign(MapX, "+=", (MAP_ZoomFactor ?? 0));
    if(((MapX ?? 0)-(MAP_ZoomFactor ?? 0)/2) / (MAP_ZoomFactor ?? 0) > (MAP_ScreenWidth ?? 0))
      {
      MapX = (MAP_ZoomFactor ?? 0) / 2;
      S42MigrationAssign(MapY, "+=", (MAP_ZoomFactor ?? 0));
      if(((MapY ?? 0)-(MAP_ZoomFactor ?? 0)/2) / (MAP_ZoomFactor ?? 0) > (MAP_ScreenHeight ?? 0))
        MapY = (MAP_ZoomFactor ?? 0) / 2;
      }
    }
}

public func DrawPixel(int iX, int iY, int Color)
{
  // Zeichnen
  iX ??= 0;
  iY ??= 0;
  Color ??= 0;
  SetLandscapePixel(-(MAP_ScreenWidth ?? 0)/2 + (iX ?? 0), -(MAP_ScreenHeight ?? 0)/2 + (iY ?? 0), Color);
}

protected func ShowClonks()
{
  // Neuen Clonk suchen
  lClonk = FindObject(nil,nil,nil,nil,nil, OCF_CrewMember,nil,nil,nil, lClonk);
  if(lClonk)
    {
    // Clonkposition anzeigen
    var iX = (GetX(lClonk) ?? 0) / (MAP_ZoomFactor ?? 0);
    var iY = (GetY(lClonk) ?? 0) / (MAP_ZoomFactor ?? 0);
    CreateParticle("MapDot", -(MAP_ScreenWidth ?? 0)/2 + (iX ?? 0), -(MAP_ScreenHeight ?? 0)/2 + (iY ?? 0), nil, nil, 10, GetColorDw(lClonk));
    }
  // Nächsten anzeigen
  ScheduleCall(this(), "ShowClonks", 2);
}

/* Master-Funktionen */

// Führt nur der MasterScreen aus

private func LandscapeWH()
{
  MAP_ScreenWidth = (LandscapeWidth() ?? 0) / (MAP_ZoomFactor ?? 0);
  MAP_ScreenHeight = (LandscapeHeight() ?? 0) / (MAP_ZoomFactor ?? 0);
}
