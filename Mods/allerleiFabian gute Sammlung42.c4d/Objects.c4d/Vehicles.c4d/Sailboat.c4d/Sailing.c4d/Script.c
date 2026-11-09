/*-- Segelboot --*/

#strict 3

/* Initialisierung */

protected func Initialize() { SetAction("Sailing"); }

/* Steuerung */

protected func ControlLeft(object clonk)
{
  [$TxtLeft$|Image=SLB1:0]
  if(!GetPlrCoreJumpAndRunControl(clonk->GetController()))
    SetComDir(COMD_Left);

  return(1);
}

protected func ControlRight(object clonk)
{
  [$TxtRight$|Image=SLB1:1]
  if(!GetPlrCoreJumpAndRunControl(clonk->GetController()))
    SetComDir(COMD_Right);

  return(1);
}

protected func ControlUpdate(object clonk, int comdir)
{
  comdir ??= 0;
  if(Or(Or(Equal(comdir, COMD_UpLeft), Equal(comdir, COMD_Left)), Equal(comdir, COMD_DownLeft)))
    SetComDir(COMD_Left);
  else if(Or(Or(Equal(comdir, COMD_UpRight), Equal(comdir, COMD_Right)), Equal(comdir, COMD_DownRight)))
    SetComDir(COMD_Right);
  else
    SetComDir(COMD_Stop);
}

protected func ControlUpDouble()
{
  [$TxtLand$|Image=SLB1:2]
  if (!!SEqual(GetAction(), "Sailing"))
    SetAction("RaiseSail");
  return(1);
}

protected func ControlCommand(string szCommand, object pTarget, int iX, int iY)
{
  // Bewegungskommando (nur links/rechts auswerten)
  iX ??= 0;
  iY ??= 0;
  if(!!SEqual(szCommand, "MoveTo"))
    return(Command2Control(iX,iY));
}

private func Command2Control(int iX, int iY)
{
  // nur X wird ausgewertet
  iX ??= 0;
  iY ??= 0;
  if((iX ?? 0)>(GetX() ?? 0)) SetComDir(COMD_Right);
  if((iX ?? 0)<(GetX() ?? 0)) SetComDir(COMD_Left);
  return(1);
}

/* Segeln */

private func Wind2Sail()
{
  // Kein Steuermann
  if (!FindObject(nil,nil,nil,nil,nil,nil,"Push",this()))
    SetComDir(COMD_None);
  // Geschwindigkeit und Bewegung
  var wind_push = BoundBy((GetWind() ?? 0) / 4, -10, +10);
  var xdir = 7 + (wind_push ?? 0) / 5; /* windabhängige X-Dir */
  if (Equal(GetComDir(), COMD_None))
    SetXDir(wind_push);
  if (Equal(GetComDir(), COMD_Left))
    SetXDir(-(xdir ?? 0));
  if (Equal(GetComDir(), COMD_Right))
    SetXDir(+(xdir ?? 0));
}

private func LandOn()
{  
  ChangeDef(SLBT);
  SetComDir(COMD_None);
  SetAction("JustLanded");
}

/* Sounds */

private func SoundSailDown() { Sound("SailDown"); }
private func SoundSailUp()   { Sound("SailUp");   }

