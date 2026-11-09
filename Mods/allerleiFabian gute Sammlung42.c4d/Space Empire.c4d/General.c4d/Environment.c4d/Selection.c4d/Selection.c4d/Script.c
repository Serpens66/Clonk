/*-- Auswahlobjekt --*/

/*  Dieses Objekt stellt die grafische Seitenauswahl jedes Spielers dar,
 *  verarbeitet die Tastatureingaben und erzeugt die grafischen Unterobjekte.
 *  Außerdem ist es für die Vergabe von Bauplänen - je nach Wahl der Partei -
 *  zuständig und speichert während des weiteren Spielverlaufs die getroffene
 *  Auswahl für jeden Spieler.
 */


#strict

local selection, party;
local px, py;


/* globale Funktionen */

global func GetPlayerParty(player)
{
  // bei 1 ist es ein CoAl-Spieler, bei 2 ein SLC-Spieler
  var obj = FindObjectOwner(SEL1, player);
  if (!obj) return(0);
  return(obj->GetParty());
}

global func SetPlayerParty(player, plrparty)
{
  var obj = FindObjectOwner(SEL1, player);
  if (!obj) obj = CreateObject(SEL1, 0,0, player);
  obj->SetParty(plrparty);
}


/* Initialisierung */

protected func Initialize()
{
  // bisher keine Wahl getroffen
  party = 0;
  // nur für den Besitzer zu sehen
  SetVisibility(VIS_Owner());
  // Auswahl positionieren
  var obj = FindObjectOwner(0,GetOwner(),0,0,0,0,OCF_CrewMember());
  px = GetX(obj);
  py = GetY(obj) - 50;
  SetPosition(BoundBy(px, 250, LandscapeWidth()-250),
              BoundBy(py, 150, LandscapeHeight()-150));
  // alle Objeke des Spielers erstmal hier herein
  obj = 0;
  while (obj = FindObjectOwner(0,GetOwner(),0,0,0,0,0,0,0,obj))
  {
    // Gebäude bleiben stehen
    if (!(GetOCF(obj) & OCF_Exclusive()))
      Enter(this(), obj);
  }
  // Aktion setzen
  SetAction("Select");
}


/* Aufrufe */

public func GetParty()
{
  return(party);
}

public func SetParty(plrparty)
{
  party = plrparty;
}


/* ActionCalls */

protected func Selecting()
{
  // höchstrangiger Clonk muss ausgewählt bleiben
  var clonk = GetHiRank(GetOwner());
  if (GetCursor(GetOwner()) != clonk)
    SetCursor(GetOwner(),clonk);
}

protected func Waiting()
{
  Selecting();
  // gerade beim Warten und Spiel startet schon?
  if (!FindObjectOwner(GetID(),GetPlayerByIndex(0)))
    return(RemoveObject());
}


/* Wahl der Seite: SLC oder CoAl */

protected func ContainedRight()
{
  [CoAl wählen]
  if (GetAction() ne "Select")
    return(1);
  Sound("Click");
  if (selection != 1)
    CreateObject(SEL2,0,0,GetOwner())->Init(1, this());
  selection = 1;
  return(1);
}

protected func ContainedLeft()
{
  [SL wählen]
  if (GetAction() ne "Select")
    return(1);
  Sound("Click");
  if (selection != 2)
    CreateObject(SEL2,0,0,GetOwner())->Init(2, this());
  selection = 2;
  return(1);
}

protected func ContainedThrow()
{
  [Auswahl bestätigen]
  if (GetAction() ne "Select")
    return(1);
  if (!selection)
    return(Sound("Error"));
  Sound("Click");
  // je nach Auswahl...
  var str;
  if (selection == 1)
  {
    Select_CoAl();
    str = "Cognition Alliance";
  }
  else
  {
    Select_SLC();
    str = "SpaceLaunch Corporation";
  }
  // Log-Message
  var i = Random(5);
  if (i == 0)  Log("%s spielt mit der %s",         GetPlayerName(GetOwner()), str);
  if (i == 1)  Log("%s kämpft für die %s",         GetPlayerName(GetOwner()), str);
  if (i == 2)  Log("%s unterstützt die %s",        GetPlayerName(GetOwner()), str);
  if (i == 3)  Log("%s wählte die %s",             GetPlayerName(GetOwner()), str);
  if (i == 4)  Log("%s entschied sich für die %s", GetPlayerName(GetOwner()), str);
  // Unterobjekte löschen
  var obj;
  while (obj = FindObject(SEL2,0,0,0,0,0,0,this()))
    RemoveObject(obj);
  // Inhalt raus
  obj = 0;
  while (obj = Contents())
    Exit(obj, px-GetX()+Random(25)-12,
              py-GetY()+50);
  // minimieren
  SetAction("Minimized");
  SetPosition(0,0);
}


/* SLC oder CoAl gewählt */

private func Select_CoAl()
{
  var player = GetOwner();
  // Chemisches
  AddSomeKnowledges(player, FLNT, SFLN, EFLN, TFLN, STFN, FBMP, GCAN);
  // Materialien
  AddSomeKnowledges(player, CALU, CTIT, METL);
  // Ausrüstung
  AddSomeKnowledges(player, BARL, CMDP, CABO, CGBE, CNBF, CSBF);
  // Waffen
  AddSomeKnowledges(player, CWBL, CWMG, CWSG, CWPG, CWFT, SFEX);
  AddSomeKnowledges(player, CWBO, CWPG, CWMP, TRP1);
  // Fahrzeuge
  AddSomeKnowledges(player, BRDG, LORY, CVFR, CVLA, SUB1);
  // Gebäude
  AddSomeKnowledges(player, CNBS, CBBS, CPBX);
  if (FindObject(ENRG))
    AddSomeKnowledges(player, CWPP, CSOC, CFCP, CFUR, CENC, CENR);
  AddSomeKnowledges(player, CCHF, CREF, CVFC, CH2P, CHOL);
  AddSomeKnowledges(player, CDFT, CGDT, CMST, CPMP);
  // Waffensysteme
  AddSomeKnowledges(player, CAM1, CAM2);
  // Spieler als CoAl-Spieler markieren
  party = 1;
}

private func Select_SLC()
{
  var player = GetOwner();
  // Chemisches
  AddSomeKnowledges(player, FLNT, SFLN, EFLN, TFLN, STFN);
  AddSomeKnowledges(player, FBMP, CNCR, LIQG, SDBM, SBST, GCAN);
  // Ausrüstung
  AddSomeKnowledges(player, SMED, SREP, SO2P, SACU);
  // Waffen
  AddSomeKnowledges(player, SREV, SFTH, SFEX, SBAZ, SMGN);
  AddSomeKnowledges(player, SPC1, SPC2, SPC3, SPC4);
  AddSomeKnowledges(player, SELW, SROL, SROP, STRK, SMPC);
  AddSomeKnowledges(player, TRP1, SSPR, SNSS);
  // Fahrzeuge
  AddSomeKnowledges(player, LORY, BRDG, SUB1, SHTL, SCMD);
  // Gebäude
  AddSomeKnowledges(player, SDWU, SMBS, SBTR);
  if (FindObject(ENRG))
    AddSomeKnowledges(player, STRM, STTM, SWEG, SAPP, SGEO);
  AddSomeKnowledges(player, SFDY, SCHM, SWEF, SMFC, SLD1);
  AddSomeKnowledges(player, SLTW, STOW, SCNB, SPMP, SDRK);
  // Waffensysteme
  AddSomeKnowledges(player, SCN1, SCN2);
  AddSomeKnowledges(player, SLSW, STW1, STW4, STW2, STW3);
  // Spieler als SLC-Spieler markieren
  party = 2;
}

