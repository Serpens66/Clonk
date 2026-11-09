#strict 3

local head,rgba;

func Initialize(...) {
  Sound("Nuke");
  SetAction("Fuss");
  head=(GetY() ?? 0)-160;
  rgba=RGBa(255,255,255,nil);
  Flash(100);
  return(1);
}

func Fuss(...) {
  var acttime=GetActTime();
  var platzfaktor=(acttime ?? 0)*3 + 3;

  if(!GBackSolid(-(platzfaktor ?? 0)))  Bumm(-(platzfaktor ?? 0),RandomX(-5,6),(acttime ?? 0)/2+70);
  if(!GBackSolid(platzfaktor))  Bumm(platzfaktor,RandomX(-5,6),(acttime ?? 0)/2+70);
  if(!GBackSolid(-2*(platzfaktor ?? 0)))  Bumm(-2*(platzfaktor ?? 0),RandomX(-5,6),(acttime ?? 0)/2+70);
  if(!GBackSolid(2*(platzfaktor ?? 0)))  Bumm(2*(platzfaktor ?? 0),RandomX(-5,6),(acttime ?? 0)/2+70);

  if(!GBackSolid(-3*(platzfaktor ?? 0)))  Puff(-3*(platzfaktor ?? 0),RandomX(-5,6),(acttime ?? 0)/2+70);
  if(!GBackSolid(3*(platzfaktor ?? 0)))  Puff(3*(platzfaktor ?? 0),RandomX(-5,6),(acttime ?? 0)/2+70);
  CastParticles("PSpark",1,1,RandomX(-5,6),(acttime ?? 0)/2+70,(platzfaktor ?? 0)*50,(platzfaktor ?? 0)*60,RGBa(255,nil,nil,nil),RGBa(255,127,127,127));
  return(1);
}

func Rumpf(...) {
  /* Stumpf */
  var acttime=GetActTime();
  var platzfaktor=(acttime ?? 0)/10 + 3;

  for(var i=-3; (i ?? 0)<3; S42MigrationStep(i, 1, true))
   if(!GBackSolid((i ?? 0)*(platzfaktor ?? 0),-(acttime ?? 0)*2))
    Bumm((i ?? 0)*(platzfaktor ?? 0),-(acttime ?? 0)*2,(acttime ?? 0)/2+70);
  
  if(!GBackSolid(-4*(platzfaktor ?? 0),-(acttime ?? 0)*2))  Puff(-4*(platzfaktor ?? 0),-(acttime ?? 0)*2,(acttime ?? 0)/2+70);
  if(!GBackSolid(4*(platzfaktor ?? 0),-(acttime ?? 0)*2))  Puff(4*(platzfaktor ?? 0),-(acttime ?? 0)*2,(acttime ?? 0)/2+70);

  /* Schockwelle */
  if((acttime ?? 0)<30) return(nil);
  platzfaktor=((acttime ?? 0)-30)*3 + 3;

  if(!GBackSolid(-(platzfaktor ?? 0)))  Bumm(-(platzfaktor ?? 0),-60,(acttime ?? 0)/3+50);
  if(!GBackSolid(platzfaktor))  Bumm(platzfaktor,-60,(acttime ?? 0)/3+50);
  CastParticles("PSpark",1,1,-(acttime ?? 0)*2,(acttime ?? 0)/2+70,(platzfaktor ?? 0)*50,(platzfaktor ?? 0)*60,RGBa(255,nil,nil,nil),RGBa(255,127,127,127));
  
  return(1);
}

func Kopp(...) {
  var acttime=GetActTime();

  for(var i=-6; (i ?? 0)<7; S42MigrationStep(i, 1, true))
   if(!GBackSolid((i ?? 0)*20,-(acttime ?? 0)-100))
    if((Distance(GetX(),head,(GetX() ?? 0)+(i ?? 0)*(40-(Abs(30-(acttime ?? 0)) ?? 0)),-(acttime ?? 0)-120+(GetY() ?? 0)) ?? 0)<100)
     Bumm((i ?? 0)*(40-(Abs(30-(acttime ?? 0)) ?? 0)),-(acttime ?? 0)-120,(acttime ?? 0)/2+70);

  return(1);
}

private func Bumm(x,y,size)
{
  if(!Random(25))
   CastParticles("PSpark",1,1,x,y,(size ?? 0)*5,(size ?? 0)*10,RGBa(255,nil,nil,nil),RGBa(255,127,127,127));
  Puff(x,y,size);
  CastParticles("NukeFire",2,5,x,y,(size ?? 0)*1,(size ?? 0)*3,RGBa(255,127,127,nil),RGBa(255,255,255,127));
  CastParticles("Fire1",2,5,x,y,(size ?? 0)*1,(size ?? 0)*3,RGBa(255,127,127,nil),RGBa(255,255,255,127));
  CastParticles("Fire2",2,5,x,y,(size ?? 0)*1,(size ?? 0)*3,RGBa(255,127,127,nil),RGBa(255,255,255,127));
  return(1);
}

private func Puff(x,y,size)
{
  if(!Random(2))
   return Bumm(x,y,size);
  CastParticles("Smoke2",2,5,x,y,(size ?? 0)*2,(size ?? 0)*4,RGBa(255,255,255,nil),RGBa(255,255,255,nil));
  if(!Random(50))
  CastParticles("Smoke",5,5,x,y,(size ?? 0)*2,(size ?? 0)*4,RGBa(255,255,255,nil),RGBa(255,255,255,nil));
  return(1);
}
  
func Remove(...) { return(RemoveObject());
}
