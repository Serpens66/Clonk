/*-- Vehicle-Spawnpoint --*/

#strict

local  player, spawntimer, timer , deltaY , vehikel , init;

global func PlaceVehicleSpawnpoint(id idObj, int iX, int iY, int timer, int dY)
{
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
  SetClrModulation(RGBa(0,0,0,255));
  timer=spawntimer;
}

protected func Timer()
{
  //Zu beginn ein Fahrzeug erstellen
  if(!init)
  {
  vehikel=CreateObject(GetID(Contents()),0,-deltaY);
  init=1;
  }
  // Tollen Effekt starten
  if(Random(2)) CreateParticle("NoGravSpark", RandomX(-5,5), RandomX(0,-5), 0, -5, 25, RGBa(210, 210, 255, 100));
  // Timer runterzählen
  timer--;
  if(timer<0) if(!vehikel) Spawn();
}



protected func Spawn()
{
   vehikel = CreateObject(GetID(Contents()),0,-deltaY);
   Sound("Magic1",this());
   timer = spawntimer;
   return(1);
}