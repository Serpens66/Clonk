/*-- Bombe --*/

#strict 3
#include TBUL

func Init(int dir) {
  dir ??= 0;
  SetYDir(RandomX(4,6),this(),1);
  SetXDir((dir ?? 0)*(RandomX(4,6) ?? 0),this(),1);
  SetR((dir ?? 0)*-20,this());
  //SetXDir(Sin(GetWind(0,0,1)/3,10),this(),1);
  return(1);
}
