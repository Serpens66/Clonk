/*-- Batterie --*/

#strict
#include BOB2
#include XMTR

local basement, manager;
local glow, FillEnergy;    // Glühen, Füllstand


/* Fundament */

protected func Initialize()
{
  manager = CreateObject(BMAN,0,0,GetOwner());
  manager->Init(this(), 1);
  manager->InitLight(-3,-20);
  // Glühen
  glow = CreateObject(SBTG,0,0,GetOwner());
  glow->Init(this());
  FillEnergy = 0;
}

protected func Construction()
{
  basement = CreateObject(BS08,0,+8,GetOwner());
}
  
protected func Destruction()
{
  PreDestruction();
  if (basement) RemoveObject(basement);
  if (glow)     RemoveObject(glow);
}

public func PreDestruction()
{
  if (manager) RemoveObject(manager);
}


/* Steuerung */

protected func ControlUp(clonk)
{
  [Statusanzeige]
  if (!inherited(clonk)) return(1);   // wenn verfeindeter Spieler
  // Anzeige
  ShowInfoBar(IBRE, this(), FillEnergy, MaxFillEnergy(), 5, 1);
  return(1);
}

protected func ControlDig(clonk)
{
  [Akku-Menü|Image=SACU]
  if (Hostile(GetController(clonk),GetOwner())) return(1);
  var accu = AccuAttached();
  // Menü öffnen
  CreateMenu(GetID(),clonk);
  if (accu)
    AddMenuItem("Akku abnehmen", "RemoveAccu", SACU,clonk, 0,clonk, "Den an der Batterie angebrachten Akku wieder abnehmen.");
  if (!accu && FindContents(SACE,clonk))
    AddMenuItem("Akku aufladen",   "LoadAccu", SACE,clonk, 0,clonk, "Einen leeren Akku auffüllen.");
  if (!accu && FindContents(SACU,clonk))
    AddMenuItem("Akku anschließen","TakeAccu", SACU,clonk, 0,clonk, "Einen Akku als Zusatzspeicher für die Batterie verwenden.");
  AddMenuItem("Abbrechen", "Nothing", SYCC,clonk);
  return(1);
}


/* Akku-Funktionen */

private AccuAttached: return(FindObject(SACA, 0,0,0,0, 0,0,this()));

private Nothing: return(0);

private func RemoveAccu(par0, clonk)
{
  // Akku abnehmen
  var accu = AccuAttached();
  if (!accu)
    return(Sound("Error"));
  accu->Remove(clonk);
  // zurück zum Menü
  ControlUp(clonk);
}

private func LoadAccu(par0, clonk)
{
  // leeren Akku finden
  var accu = FindContents(SACE,clonk);
  if (!accu || HasEMP())
    return(Sound("Error"));
  // aufladen, wenn möglich
  if (FillEnergy < 800)
  {
    Message("zuwenig Energie", this());
    return(Sound("Error"));
  }
  FillEnergy -= 800;
  ChangeDef(SACU,accu);
  // zurück zum Menü
  ControlUp(clonk);
}

private func TakeAccu(par0, clonk)
{
  // Akku anschließen
  var accu = FindContents(SACU,clonk);
  if (!accu || HasEMP())
    return(Sound("Error"));
  // neuen Akku draus machen
  RemoveObject(accu);
  accu = CreateObject(SACA,0,0,GetOwner());
  ObjectSetAction(accu, "HighNRG", this());
  // zurück zum Menü
  ControlUp(clonk);
}


/* TimerCall */

protected func CheckEnergy()
{
  // internen Energiewert auf das Maximum 100 bringen
  var nrg = Min(100-GetEnergy(), FillEnergy);
  DoEnergy(+nrg);
  FillEnergy -= nrg;
  // Energie versenden
  TransmitEnergy();
  // Regelniveau: GetEnergy() = 50
  nrg = BoundBy(50-GetEnergy(), FillEnergy-MaxFillEnergy(),   // nicht mehr abgeben, als da ist
                                FillEnergy);                  // und Batterie nicht überfüllen
  DoEnergy(+nrg);
  FillEnergy -= nrg;
  // Update für Anzeige
  ShowInfoBar(IBRE, this(), FillEnergy, MaxFillEnergy(), 0, 1);
  // kleiner Effekt
  if ((Random(5000) < FillEnergy) && ActIdle())
    if (!Random(25))
    {
      Sound("Electric");
      SetAction("Electric");
    }
}

public DoFill:  return(FillEnergy += Par(0));
public GetFill: return(FillEnergy);

public func EnoughEnergy()
{
  return(GetEnergy()+FillEnergy);
}

public IsEnergyConsumer: return(1);


/* Blitzeinschlag */

public func LightningStrike()
{
  // nur wenn fertig gebaut
  if (GetCon() < 100) return(0);
  DoEnergy(+100);
  FillEnergy = Min(FillEnergy+1000, MaxFillEnergy());
  // Sound & Effekt
  Sound("Electric");
  SetAction("Lightning");
  return(1);
}


/* Anzeigentext */

private func InfoBarText(barID, fill, maxf)
{
  if (barID == IBRE)
    return(Format("@Füllmenge:  %d%", fill*100/maxf));
  return(0);
}


/* Eigenschaften */

public MaxFillEnergy: return(3000);
public InfoBarSize:   return(1);
public GetMaxDamage:  return(80);

