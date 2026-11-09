/*-- Fernrohr --*/

#strict 3

local pPicture, pClonk, iClonkDir;
local iX, iY;

func Activate(pUser)
{
  [$TxtUseTelescope$]
  if(And(!SEqual(GetAction(pUser), "Walk"), !SEqual(GetAction(pUser), "HoldWalk"))) return;
  pUser->SetXDir(nil);
  iX=GetX(pUser);
  iY=GetY(pUser);
  pClonk=pUser;
  CreateMenu(AM5P,pUser);
  AddMenuItem("$TxtHorizontal$","TelescopePicture",AN5P,pUser);
  AddMenuItem("$TxtDiagUp$","TelescopePicture",AO5P,pUser,nil,1);
  AddMenuItem("$TxtDiagDown$","TelescopePicture",AP5P,pUser,nil,2);
  AddMenuItem("$TxtDown$","TelescopePicture",E48Z,pUser,nil,3);
  AddMenuItem("$TxtUp$","TelescopePicture",E38Z,pUser,nil,4);
  return(1);
}

func TelescopePicture(idID,iPhase)
{
  iClonkDir=GetDir(pClonk);
  pClonk->SetPhase(4);
  if(Equal(GetDir(pClonk), DIR_Right)) { if(!pPicture) pPicture=CreateObject(AR5P,9,1); }
  else
  {
    if(!pPicture) pPicture=CreateObject(AR5P,-7,1);
    SetGraphics("Left",pPicture);
  }
  pPicture->SetColorDw(GetColorDw(pClonk));
  pPicture->SetAction("Phases");
  var yd, wnd=CreateObject(AQ5P,nil,-4);
  SetPhase(iPhase,pPicture);
  if (And(( !Equal(iPhase, 3)), (!Equal(iPhase, 4))))
    SetXDir(-200+400*(GetDir(pClonk) ?? 0),wnd);
  else
    SetXDir(nil);
  if(Equal(iPhase, 1)) yd=-200;
  if(Equal(iPhase, 2)) yd=200;
  if(Equal(iPhase, 3)) yd=200;
  if(Equal(iPhase, 4)) yd=-200;
  SetYDir(yd,wnd);
  LocalN("iYDir",wnd)=yd;
  Finish(wnd);
}

func Finish(wnd)
{
  SetPlrView(GetOwner(pClonk),wnd);
  SetAction("Checker"); 
}


func Hit()
{
  Sound("Crystal*");
}

func Checker()
{
  pPicture->SetPosition((GetX(pClonk) ?? 0)-7+((GetDir(pClonk) ?? 0)*16),(GetY(pClonk) ?? 0)-6);
  if(Or((And(!SEqual(GetAction(pClonk), "Walk"), !SEqual(GetAction(pClonk), "HoldWalk"))), !Equal(GetPhase(pClonk), 4)))
  {
    if(pPicture) RemoveObject(pPicture);
    SetAction("Idle");
  }
  if(!Equal(iClonkDir, GetDir(pClonk)))
  {
    if(!GetDir(pClonk)) SetGraphics("Left",pPicture);
    else SetGraphics("",pPicture);
    iClonkDir=GetDir(pClonk);
  }
}

func IsAnvilProduct()
{
  return(1);
}