/*-- Flash --*/

#strict 3

global func DoExplosion(int x, int y, int level, object inobj, int cause_plr, id effect_def, string effect_particle, object layer)
{
  x ??= 0;
  y ??= 0;
  level ??= 0;
  cause_plr ??= 0;
  _inherited(x,y,level,inobj,cause_plr,effect_def,effect_particle,layer);
  if((dayhelligkeit ?? 0)<60)
    if(!IsAntiLag())
      CreateParticle("PSpark",x,y,nil,nil,(level ?? 0)*100,RGBa(100,100,100,(dayhelligkeit ?? 0)*5/2));
}
