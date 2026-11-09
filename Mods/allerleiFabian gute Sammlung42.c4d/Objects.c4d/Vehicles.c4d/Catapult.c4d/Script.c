/*-- Katapult --*/

#strict 3

local iPhase;

/* Initialisierung */

protected func Initialize()
{
  SetAction("Ready");
  SetDir(Random(2));
}

/* Prozesse */

private func HaltPushers()
{
  var pClonk;
  // Alle Clonks die das Katapult anfassen, sollen stoppen
  while(pClonk=FindObject(nil,nil,nil,nil,nil,nil,"Push",this(),nil,pClonk))
    SetComDir(COMD_Stop,pClonk);
}

private func FireSound() 
{ 
  Sound("Catapult"); 
}

private func Charging()
{
  if ((GetPhase() ?? 0)<(iPhase ?? 0)) return(nil);
  // genug geladen
  SetAction("Ready");
  SetPhase(iPhase);
}

public func Fire(bool fAuto)
{ 
  fAuto ??= false;
  SetAction("Fire");
  SetPhase(7-(iPhase ?? 0));
  // Sicht verfolgen (wenn kein automatischer Schuss)
  if (!fAuto)
    if (!Equal(GetPlrViewMode(GetController()), 2))
      SetActionTargets(nil,GetCursor(GetController()));
  return(1);
}

private func Projectile()
{
  var pProjectile=Contents();
  var iX = ((GetDir() ?? 0)*2-1)*12;
  var iY = -10;
  var iR = Random(360);
  var iXDir =  (+8 ?? 0) * ((GetDir() ?? 0) * 2 - 1) * (iPhase ?? 0) / 6;
  var iYDir = -12 * (iPhase ?? 0) / 6;
  var iRDir = 30;
  Exit(pProjectile,iX,iY,iR,iXDir,iYDir,iRDir);
  // Zufällige Streuung
  // Bei minimaler Spannung: -30 bis +30
  // Bei voller Spannung:    -80 bis +80
  var iDeviation = RandomX(-(iPhase ?? 0) * 10 - 20, (iPhase ?? 0) * 10 + 20);
  SetXDir((iXDir ?? 0) * 100 + (iDeviation ?? 0), pProjectile, 100);
  SetYDir((iYDir ?? 0) * 100 + (iDeviation ?? 0), pProjectile, 100);
  // Sicht verfolgen wenn kein automatischer Schuss
  if (GetActionTarget(1))
    SetPlrView(GetOwner(GetActionTarget(1)), pProjectile);
}

private func FireAt(int iTX)
{
  iTX ??= 0;
  var iX,iY,iXDir,iYDir,iDist=-1,iFavPhase;
  // Nicht bereit
  if(Or(!SEqual(GetAction(), "Ready"), !GetPhase())) return(nil);
  // Richtung anpassen
  if((iTX ?? 0)>(GetX() ?? 0)) SetDir(DIR_Right);
  if((iTX ?? 0)<(GetX() ?? 0)) SetDir(DIR_Left);
  // Phase aus einer Annäherungsformel ableiten: X = 22*Phase^2
  iPhase = BoundBy( Sqrt((Abs((GetX() ?? 0)-(iTX ?? 0)) ?? 0)/22) ,1,6);
  SetPhase(iPhase);
  return(Fire());
}

/* Laden */

protected func Collection() { Sound("Connect"); }

/* Steuerung */

public func ControlCommand(string szCommand, object pTarget, int iX, int iY)
{
  // Feuern mit der Maus
  iX ??= 0;
  iY ??= 0;
  if (!!SEqual(szCommand, "MoveTo"))
    if ((Distance(GetX(), GetY(), iX, iY) ?? 0) > 50)
    {
      // Nicht geladen: nur Kommando abfangen
      if (!ContentsCount())
      {
        Sound("Click");
        return(1);
      }
      // Geladen: feuern
      else
      {
        HaltPushers();
        return(FireAt(iX,iY));
      }
    }
}

public func ControlThrow() // Feuern / Inhalt
{
  [$TxtFire$|Image=CAT1:1]
  // Nicht bereit: Clonk kann nichts machen
  if (!SEqual(GetAction(), "Ready")) return(1);
  // Nicht gespannt: Clonk kann nur laden/entladen
  if (!GetPhase()) return(nil);
  // Gespannt, aber kein Inhalt: kann nicht feuern (Clonk darf reingucken)
  if (!ContentsCount()) return(nil);
  // Gespannt und geladen: feuern (und Throw-Befehl des Clonks abfangen)
  return(Fire());
}

public func ControlConf(int conf)
{
  conf ??= 0;
  if(AimStdConf(conf))
  {
    iPhase = GetPhase();
    Sound("CatapultSet");
  }
}

public func ControlDig(object clonk) // Katapult mehr spannen (weiter Schießen)
{
  [$TxtAim1$|Method=Classic|Image=CAT1:0]
  AimDown(clonk, 8, "ControlConf");
}

public func ControlDown(object clonk)
{
  [$TxtAim1$|Method=JumpAndRun|Image=CAT1:0]
}

public func ControlUp(object clonk) // Katapult weniger spannen (kürzer Schießen)
{
  [$TxtAim2$|Image=CAT1:2]
  AimUp(clonk, 8, "ControlConf");
}

public func ControlUpdate(object clonk, int comdir)
{
  comdir ??= 0;
  AimUpdate(clonk, comdir, 8, "ControlConf");
}

public func ControlDownSingle() // Sicht zurücksetzen
{
  [$TxtResetview$|Method=None]
}




