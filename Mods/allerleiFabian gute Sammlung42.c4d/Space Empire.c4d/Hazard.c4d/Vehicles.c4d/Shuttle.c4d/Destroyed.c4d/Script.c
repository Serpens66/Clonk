#strict 3

local target;

public func Init(int i) {
	i ??= 0;
	SetAction("Be");
	SetDir(Random(2));
	SetPhase(i);
	SetR(Random(360));
	SetRDir(RandomX(-10,10));
	var r = Random(360);
	var s = RandomX(20,30);
	var xdir = (Cos(r,s) ?? 0)-(Sin(r,s) ?? 0);
	var ydir = (Cos(r,s) ?? 0)+(Sin(r,s) ?? 0);
	SetXDir(xdir);
	SetYDir(xdir);
}
