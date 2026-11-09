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
  if ((GetEnergy(building) ?? 0) < (LocalN("energyneed",controller) ?? 0)) return(1);

// Munitionskontrolle
var munitionspack;
    munitionspack = FindObject(PTAP,nil,nil,nil,nil,nil,nil,nil,building);
if(And(!munitionspack, !FindObject(NOAM))) {return(PlayerMessage(GetOwner(),"keine Munition",this()));}
if(munitionspack) 
munitionspack->~RestAmmoreduzieren(controller->~Munitionsverbrauch()); 

  var bullet;
  var angle, xdir, ydir;
  var xofs, yofs;
  var phase=GetPhase(controller);
  if (!!SEqual(GetAction(controller), "TurnLeft")) phase=47-(phase ?? 0);
  // Feuern (linke und rechte Kanone)
  for (var i=nil; (i ?? 0) < 2; S42MigrationStep(i, 1, true)) {
    xofs = GetXoffset(phase,i);
    yofs = GetYoffset(phase);
    controller->FireShot(xofs, yofs, xdir, ydir);
  }
  Sound("Plasmashot");
  return(1);
}

// X- und Y- Offsets der Mündungsöffnung

private func InitLocals()
{
  // X-Werte          // mehr X-Werte     // Y-Werte
  Local(nil)  = -9;    Local(24) = -8;     Local(48) = -13;
  Local(1)  = -7;    Local(25) = -10;    Local(49) = -13;
  Local(2)  = -6;    Local(26) = -11;    Local(50) = -13;
  Local(3)  = -5;    Local(27) = -13;    Local(51) = -13;
  Local(4)  = -3;    Local(28) = -14;    Local(52) = -13;
  Local(5)  = -1;    Local(29) = -15;    Local(53) = -12;
  Local(6)  = +1;    Local(30) = -17;    Local(54) = -12;
  Local(7)  = +3;    Local(31) = -18;    Local(55) = -12;
  Local(8)  = +6;    Local(32) = -19;    Local(56) = -11;
  Local(9)  = +10;   Local(33) = -21;    Local(57) = -10;
  Local(10) = +16;   Local(34) = -22;    Local(58) = -8;
  Local(11) = +20;   Local(35) = -22;    Local(59) = -5;
  Local(12) = +20;   Local(36) = -21;    Local(60) = -2;
  Local(13) = +20;   Local(37) = -20;    Local(61) = +2;
  Local(14) = +16;   Local(38) = -20;    Local(62) = +4;
  Local(15) = +11;   Local(39) = -19;    Local(63) = +6;
  Local(16) = +8;    Local(40) = -18;    Local(64) = +8;
  Local(17) = +5;    Local(41) = -16;    Local(65) = +9;
  Local(18) = +3;    Local(42) = -15;    Local(66) = +10;
  Local(19) = nil;     Local(43) = -14;    Local(67) = +10;
  Local(20) = -2;    Local(44) = -13;    Local(68) = +11;
  Local(21) = -3;    Local(45) = -12;    Local(69) = +11;
  Local(22) = -5;    Local(46) = -10;    Local(70) = +11;
  Local(23) = -7;    Local(47) = -9;     Local(71) = +11;
}

// Wiedergabe des Offsets bezüglich des Animationsframes

private func GetXoffset(phase, cannon)
{
  // rechte Kanone
  if (cannon)
  { 
    phase = (48-(phase ?? 0)) % 48;
    return(-(Local(phase) ?? 0)-1);
  }
  // linke Kanone
  return(Local(phase));
}

private func GetYoffset(phase)
{
  // auf Phase zwischen 0 und 24 zurückrechnen
  if ((phase ?? 0) > 24)
    phase = (48-(phase ?? 0)) % 48;
  return(Local((phase ?? 0)+48));
}





