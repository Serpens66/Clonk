/*-- Säurebrocken --*/

#strict 3

protected func AdjustR()
{
	SetR(Angle(nil,nil,GetXDir(),GetYDir()));
}

public func Launch(int xdir, int ydir, object byObj)
{
	xdir ??= 0;
	ydir ??= 0;
	SetXDir(xdir);
	SetYDir(ydir);
	AddEffect("HitCheck", this(), 1,1, nil, SHT1,byObj);
	AddAlienFlareEffect(this,500);
}

protected func Hit(int xdir, int ydir)
{
	xdir ??= 0;
	ydir ??= 0;
	var x,y;
	x = Sin(GetR(),14);
	y =-(Cos(GetR(),14) ?? 0);

	CreateObject(WACM, x,(y ?? 0)+32, GetOwner());
	
  for(var i = 5, glob ; i ; S42MigrationStep(i, -1, true))
    {
    glob = CreateObject(SLST, nil, nil, GetOwner());
    glob ->~ Launch(RandomX(-40,40), RandomX(-40,40), 5);
    }
	
	RemoveObject();
}
