/*-- Flash --*/

#strict

global func DoExplosion(int x, int y, int level, object inobj, int cause_plr, id effect_def, string effect_particle, object layer)
{
  _inherited(x,y,level,inobj,cause_plr,effect_def,effect_particle,layer);
  if(dayhelligkeit<60)
    if(!IsAntiLag())
      CreateParticle("PSpark",x,y,0,0,level*100,RGBa(100,100,100,dayhelligkeit*5/2));
}
