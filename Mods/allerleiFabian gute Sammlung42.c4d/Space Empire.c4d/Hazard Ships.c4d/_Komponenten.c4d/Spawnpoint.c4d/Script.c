/*-- Vehicle-Spawnpoint --*/

#strict 3

local  player, spawntimer, timer , deltaY , vehikel , init;

global func PlaceVehicleSpawnpoint(id idObj, int iX, int iY, int timer, int dY)
{
  iX ??= 0;
  iY ??= 0;
  timer ??= 0;
  dY ??= 0;
  var spwn = CreateObject(PRS_, iX, iY, -1);
  spwn->CreateContents(idObj);
//  spwn->LocalN("deltaY") = dY;
  if(timer)
    spwn->LocalN("spawntimer") = timer;
  return(spwn);
}


protected func Initialize()
{
  // Standardtimer = 1000 Frames
  spawntimer = 1000;
  SetClrModulation(RGBa(nil,nil,nil,255));
  timer=spawntimer;
}

protected func Timer()
{
  //Zu beginn ein Fahrzeug erstellen
  if(!init)
  {
  vehikel=CreateObject(GetID(Contents()),nil,-(deltaY ?? 0));
  init=1;
  }
  // Tollen Effekt starten
  if(Random(2)) CreateParticle("NoGravSpark", RandomX(-5,5), RandomX(nil,-5), nil, -5, 25, RGBa(210, 210, 255, 100));
  // Timer runterzählen
  S42MigrationStep(timer, -1, true);
  if((timer ?? 0)<0) if(!vehikel) Spawn();
}



protected func Spawn()
{
   vehikel = CreateObject(GetID(Contents()),nil,-(deltaY ?? 0));
   Sound("Magic1",this());
   timer = spawntimer;
   return(1);
}