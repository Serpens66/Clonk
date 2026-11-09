/*-- Fernrohr --*/

#strict

local pPicture, pClonk, iClonkDir;
local iX, iY;

func Activate(pUser)
{
  [$TxtUseTelescope$]
  if(GetAction(pUser) ne "Walk" && GetAction(pUser) ne "HoldWalk") return();
  pUser->SetXDir(0);
  iX=GetX(pUser);
  iY=GetY(pUser);
  pClonk=pUser;
  CreateMenu(AM5P,pUser);
  AddMenuItem("$TxtHorizontal$","TelescopePicture",AN5P,pUser);
  AddMenuItem("$TxtDiagUp$","TelescopePicture",AO5P,pUser,0,1);
  AddMenuItem("$TxtDiagDown$","TelescopePicture",AP5P,pUser,0,2);
  AddMenuItem("$TxtDown$","TelescopePicture",E48Z,pUser,0,3);
  AddMenuItem("$TxtUp$","TelescopePicture",E38Z,pUser,0,4);
  return(1);
}

func TelescopePicture(idID,iPhase)
{
  iClonkDir=GetDir(pClonk);
  pClonk->SetPhase(4);
  if(GetDir(pClonk)==DIR_Right()) { if(!pPicture) pPicture=CreateObject(AR5P,9,1); }
  else
  {
    if(!pPicture) pPicture=CreateObject(AR5P,-7,1);
    SetGraphics("Left",pPicture);
  }
  pPicture->SetColorDw(GetColorDw(pClonk));
  pPicture->SetAction("Phases");
  var yd, wnd=CreateObject(AQ5P,0,-4);
  SetPhase(iPhase,pPicture);
  if (( iPhase != 3) && (iPhase != 4))
    SetXDir(-200+400*GetDir(pClonk),wnd);
  else
    SetXDir(0);
  if(iPhase==1) yd=-200;
  if(iPhase==2) yd=200;
  if(iPhase==3) yd=200;
  if(iPhase==4) yd=-200;
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
  pPicture->SetPosition(GetX(pClonk)-7+(GetDir(pClonk)*16),GetY(pClonk)-6);
  if((GetAction(pClonk) ne "Walk" && GetAction(pClonk) ne "HoldWalk") || GetPhase(pClonk)!=4)
  {
    if(pPicture) RemoveObject(pPicture);
    SetAction("Idle");
  }
  if(iClonkDir!=GetDir(pClonk))
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