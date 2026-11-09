/*-- Radarschirm --*/

#strict 3

local bc, cc;

func Completion() {
  var x = GetX(), y = (GetY() ?? 0) -20;
    SetPosition(x, y);
SetCategory(C4D_StaticBack);
  Start();
  AddFrame();
}

func Start() {
  SetAction("LoadPlanet");
  bc = nil;
  cc = nil;
}

func BlinkPlanet() {
  if(S42MigrationStep(bc, 1) > 2) {
    bc = nil;
    SetAction("LoadEurope");
  }
}

func BlinkEurope() {
  if(S42MigrationStep(bc, 1) > 2) {
    bc = nil;
    SetAction("LoadMidEurope");
  }
}

func Credits() {
  if(Equal(cc, nil)) SetAction("Matthi");
  if(Equal(cc, 1)) SetAction("Newton");
  if(Equal(cc, 2)) SetAction("Boni");
  if(Equal(cc, 3)) SetAction("Clonko");
  if(Equal(cc, 4)) SetAction("Raven");
  
  if(Equal(cc, 5)) Start();
  else        S42MigrationStep(cc, 1);
}
