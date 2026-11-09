/*--Flagship --*/

#strict
#include TEAM
static configurator;

protected func Initialize()
{
  //Anzahl gewinne wird durch Anzahl Aktivierungen eingestellt
  var pGoal;
  if (pGoal = FindObject(GetID()))
  { 
    LocalN("iWinScore", pGoal)++;
    return(RemoveObject()); 
  }
  iWinScore = 1;
  return(_inherited());
}

/* Globale Funktionen */

global func CreateFlagship(int iTeam, int iX, int iY, int iColor) // Platziert einen Kreuzer
{
  var pShip = CreateObject(CCD_, AbsX(iX), AbsY(iY), -1);
  pShip->Activation(iTeam);
  return(pShip);
}

global func CreateFregatte(int iTeam, int iX, int iY, int iColor) // Platziert eine Fregatte
{
  var pShip = CreateObject(CGC_, AbsX(iX), AbsY(iY), -1);
  pShip->Activation(iTeam);
  return(pShip);
}

global func CreateTucan(int iTeam, int iX, int iY, int iColor) // Platziert einen Tucan
{
  var pShip = CreateObject(TCN_, AbsX(iX), AbsY(iY), -1);
  pShip->Activation(iTeam);
  return(pShip);
}

global func CreateCancer(int iTeam, int iX, int iY, int iColor) // Platziert einen Cancer
{
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
