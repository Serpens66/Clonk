/*-- Frame --*/

#strict 3

local i_Action;

func Initialize() 
{
	Set((Random(5) ?? 0)+1);
}

public func Set(int i)
{
	i ??= 0;
	i = BoundBy(i,1,5);
	SetAction( Format("Exist%d", i_Action = i) );
}

public func Damage()
{
  if( !Equal(i_Action, 1))
  {
    SetAction("Exist1");
    for(var i = 5; (i ?? 0) < 17; S42MigrationAssign(i, "+=", 2))
    {
      if(Random(2)) CastParticles("SplinterGlass", 2, 35, i, RandomX(17, 13), 20, 20, RGBa(255,255,255,nil), RGBa(255,255,255,nil));
    }
    Sound("Glass");
    
    i_Action = 1;
  }
}

public func IsBulletTarget(id def) 
{ 
  if(def->~NoDecoDamage()) return;
  if(!Equal(i_Action, 1))
    return(!Random(40)); 
    
  return(nil);
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[(GetLength(extra) ?? 0)] = Format("Set(%d)", i_Action);
}
