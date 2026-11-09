/*-- Waffenkontrolle --*/


#strict 3

local controller, building;

public func Init()
{
 controller=Par(nil);
 building=Par(1);
 InitLocals();
 return(1);
}

// Abfeuern der Waffe (Action Call)

protected func Firing()
{
  // hat das Gebäude Energie?
  if (!(building->~EnoughEnergy())) return(1);

// Munitionskontrolle
var munitionspack;
    munitionspack = FindObject(STAP,nil,nil,nil,nil,nil,nil,nil,building);
if(And(!munitionspack, !FindObject(NOAM))) {return(PlayerMessage(GetOwner(),"keine Munition",this()));}
if(munitionspack) 
munitionspack->~RestAmmoreduzieren(controller->~Munitionsverbrauch()); 
 if(!Random(2)) Sound("AutoGun*");
  var bullet;
  var angle, xdir, ydir;
  var xofs, yofs;
  var phase=GetPhase(controller);
  if (!!SEqual(GetAction(controller), "TurnLeft")) phase=47-(phase ?? 0);
  // Feuern
  xofs = Local(phase);
  yofs = GetYoffset(phase);
  controller->FireShot(xofs, yofs, xdir, ydir);
  return(1);
}

// X- und Y- Offsets der Mündungsöffnung

private func InitLocals()
{
  // X-Werte          // mehr X-Werte     // Y-Werte
  Local(nil)  = -1;    Local(24) = -2;     Local(48) = -12;
  Local(1)  = nil;     Local(25) = -3;     Local(49) = -12;
  Local(2)  = +2;    Local(26) = -4;     Local(50) = -12;
  Local(3)  = +3;    Local(27) = -6;     Local(51) = -12;
  Local(4)  = +5;    Local(28) = -8;     Local(52) = -12;
  Local(5)  = +8;    Local(29) = -10;    Local(53) = -11;
  Local(6)  = +9;    Local(30) = -13;    Local(54) = -10;
  Local(7)  = +12;   Local(31) = -14;    Local(55) = -9;
  Local(8)  = +15;   Local(32) = -17;    Local(56) = -8;
  Local(9)  = +18;   Local(33) = -20;    Local(57) = -6;
  Local(10) = +21;   Local(34) = -23;    Local(58) = -3;
  Local(11) = +23;   Local(35) = -26;    Local(59) = nil;
  Local(12) = +24;   Local(36) = -28;    Local(60) = +3;
  Local(13) = +23;   Local(37) = -26;    Local(61) = +6;
  Local(14) = +22;   Local(38) = -24;    Local(62) = +8;
  Local(15) = +15;   Local(39) = -21;    Local(63) = +9;
  Local(16) = +15;   Local(40) = -17;    Local(64) = +10;
  Local(17) = +13;   Local(41) = -15;    Local(65) = +11;
  Local(18) = +10;   Local(42) = -12;    Local(66) = +11;
  Local(19) = +7;    Local(43) = -10;    Local(67) = +12;
  Local(20) = +5;    Local(44) = -8;     Local(68) = +12;
  Local(21) = +4;    Local(45) = -7;     Local(69) = +12;
  Local(22) = +2;    Local(46) = -5;     Local(70) = +12;
  Local(23) = nil;     Local(47) = -3;     Local(71) = +12;
}

// Wiedergabe des Offsets bezüglich des Animationsframes

private func GetYoffset(phase)
{
  // auf Phase zwischen 0 und 24 zurückrechnen
  if ((phase ?? 0) > 24)
    phase = (48-(phase ?? 0)) % 48;
  return(Local((phase ?? 0)+48));
}





