/*--Flagship --*/

#strict 3
#include TEAM
static configurator;

protected func Initialize()
{
  //Anzahl gewinne wird durch Anzahl Aktivierungen eingestellt
  var pGoal;
  if (pGoal = FindObject(GetID()))
  { 
    S42MigrationStep(LocalN("iWinScore", pGoal), 1, true);
    return(RemoveObject()); 
  }
  iWinScore = 1;
  return(_inherited());
}

/* Globale Funktionen */

global func CreateFlagship(int iTeam, int iX, int iY, int iColor) // Platziert einen Kreuzer
{
  iTeam ??= 0;
  iX ??= 0;
  iY ??= 0;
  iColor ??= 0;
  var pShip = CreateObject(CCD_, AbsX(iX), AbsY(iY), -1);
  pShip->Activation(iTeam);
  return(pShip);
}

global func CreateFregatte(int iTeam, int iX, int iY, int iColor) // Platziert eine Fregatte
{
  iTeam ??= 0;
  iX ??= 0;
  iY ??= 0;
  iColor ??= 0;
  var pShip = CreateObject(CGC_, AbsX(iX), AbsY(iY), -1);
  pShip->Activation(iTeam);
  return(pShip);
}

global func CreateTucan(int iTeam, int iX, int iY, int iColor) // Platziert einen Tucan
{
  iTeam ??= 0;
  iX ??= 0;
  iY ??= 0;
  iColor ??= 0;
  var pShip = CreateObject(TCN_, AbsX(iX), AbsY(iY), -1);
  pShip->Activation(iTeam);
  return(pShip);
}

global func CreateCancer(int iTeam, int iX, int iY, int iColor) // Platziert einen Cancer
{
  iTeam ??= 0;
  iX ??= 0;
  iY ??= 0;
  iColor ??= 0;
  var pShip = CreateObject(CAN_, AbsX(iX), AbsY(iY), -1);
  pShip->Activation(iTeam);
  return(pShip);
}


global func SetConfigurable()
{
  configurator=1;
  return(1);
}

// Kann mittels des Spielzielauswählers konfiguriert werden
public func IsConfigurable() { return(configurator); }
