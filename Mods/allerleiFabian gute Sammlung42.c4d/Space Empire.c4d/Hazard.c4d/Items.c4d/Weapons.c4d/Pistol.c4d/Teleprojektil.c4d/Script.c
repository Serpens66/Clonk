/*-- Teleprojektil --*/

#strict 3

local iRemoveTimer;
local user;

func Initialize()
{
iRemoveTimer=175;
SetPlrViewRange(50);
}

func Hit()
{
    Sound("Bip");
}

func ControlDigDouble()
{
    //var user=
    var xpos=GetX();
    var ypos=GetY();
    
    if(Or(Or(GBackSolid(-2,-20), GBackSolid(2,-20)), GBackSolid(nil,-10))) {Message("$BadPlace$",user); SetCursor(GetController(),user); return(RemoveObject());}
    
    DrawParticleLine("XSpark",nil,nil,(GetX(user) ?? 0)-(xpos ?? 0),(GetY(user) ?? 0)-(ypos ?? 0),1,50,RGBa(255,nil,nil,128),RGBa(nil,255,nil,128));
    SetPosition(xpos,(ypos ?? 0)-12,user);
    SetCursor(GetController(),user);
    RemoveObject();
    Sound("Electric");
    CastObjects(SPRK,20,30,nil,-20);
    Sound("Fire1");
    
}

func Launch(int xdir, int ydir, int iDmg, Nutzer)
{
  xdir ??= 0;
  ydir ??= 0;
  iDmg ??= 0;
  user=Nutzer;
  SetSpeed(xdir, ydir);
}

func CheckCursor() {
    S42MigrationStep(iRemoveTimer, -1, true);
    if(!iRemoveTimer) {SetCursor(GetController(),user); RemoveObject();}
    //if(!GetCursor(GetOwner(this()))==this()) RemoveObject();
}

func IsBouncy() { return(true); }
