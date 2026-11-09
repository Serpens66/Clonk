#strict

local head,rgba;

Initialize:
  Sound("Nuke");
  SetAction("Fuss");
  head=GetY()-160;
  rgba=RGBa(255,255,255,0);
  Flash(100);
  return(1);

Fuss:
  var acttime=GetActTime();
  var platzfaktor=acttime*3 + 3;

  if(!GBackSolid(-platzfaktor))  Bumm(-platzfaktor,RandomX(-5,6),acttime/2+70);  
  if(!GBackSolid(platzfaktor))  Bumm(platzfaktor,RandomX(-5,6),acttime/2+70); 
  if(!GBackSolid(-2*platzfaktor))  Bumm(-2*platzfaktor,RandomX(-5,6),acttime/2+70);  
  if(!GBackSolid(2*platzfaktor))  Bumm(2*platzfaktor,RandomX(-5,6),acttime/2+70);

  if(!GBackSolid(-3*platzfaktor))  Puff(-3*platzfaktor,RandomX(-5,6),acttime/2+70);  
  if(!GBackSolid(3*platzfaktor))  Puff(3*platzfaktor,RandomX(-5,6),acttime/2+70);
  CastParticles("PSpark",1,1,RandomX(-5,6),acttime/2+70,platzfaktor*50,platzfaktor*60,RGBa(255,0,0,0),RGBa(255,127,127,127));
  return(1);

Rumpf:
  /* Stumpf */
  var acttime=GetActTime();
  var platzfaktor=acttime/10 + 3;

  for(var i=-3; i<3; i++) 
   if(!GBackSolid(i*platzfaktor,-acttime*2))
    Bumm(i*platzfaktor,-acttime*2,acttime/2+70);
  
  if(!GBackSolid(-4*platzfaktor,-acttime*2))  Puff(-4*platzfaktor,-acttime*2,acttime/2+70);  
  if(!GBackSolid(4*platzfaktor,-acttime*2))  Puff(4*platzfaktor,-acttime*2,acttime/2+70); 

  /* Schockwelle */
  if(acttime<30) return(0);
  platzfaktor=(acttime-30)*3 + 3;

  if(!GBackSolid(-platzfaktor))  Bumm(-platzfaktor,-60,acttime/3+50);  
  if(!GBackSolid(platzfaktor))  Bumm(platzfaktor,-60,acttime/3+50); 
  CastParticles("PSpark",1,1,-acttime*2,acttime/2+70,platzfaktor*50,platzfaktor*60,RGBa(255,0,0,0),RGBa(255,127,127,127));
  
  return(1);

Kopp: 
  var acttime=GetActTime();

  for(var i=-6; i<7; i++)
   if(!GBackSolid(i*20,-acttime-100))
    if(Distance(GetX(),head,GetX()+i*(40-Abs(30-acttime)),-acttime-120+GetY())<100)
     Bumm(i*(40-Abs(30-acttime)),-acttime-120,acttime/2+70);

  return(1);

private func Bumm(x,y,size)
{
  if(!Random(25))
   CastParticles("PSpark",1,1,x,y,size*5,size*10,RGBa(255,0,0,0),RGBa(255,127,127,127));
  Puff(x,y,size);
  CastParticles("NukeFire",2,5,x,y,size*1,size*3,RGBa(255,127,127,0),RGBa(255,255,255,127));
  CastParticles("Fire1",2,5,x,y,size*1,size*3,RGBa(255,127,127,0),RGBa(255,255,255,127));
  CastParticles("Fire2",2,5,x,y,size*1,size*3,RGBa(255,127,127,0),RGBa(255,255,255,127));
  return(1);
}

private func Puff(x,y,size)
{
  if(!Random(2))
   return Bumm(x,y,size);
  CastParticles("Smoke2",2,5,x,y,size*2,size*4,RGBa(255,255,255,0),RGBa(255,255,255,0));
  if(!Random(50))
  CastParticles("Smoke",5,5,x,y,size*2,size*4,RGBa(255,255,255,0),RGBa(255,255,255,0));
  return(1);
}
  
Remove: return(RemoveObject());
