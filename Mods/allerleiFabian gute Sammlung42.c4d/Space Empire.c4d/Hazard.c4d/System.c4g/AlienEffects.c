/*-- Alienpartikeleffekte --*/

#strict 3

global func AlienSplatter(int iSize, int iX, int iY, int iColor) {
	// not on sky
	iSize ??= 0;
	iX ??= 0;
	iY ??= 0;
	iColor ??= 0;
	if(Or(!Equal(GetMaterialVal("Density","Material",GetMaterial(iX,iY)), nil)
       , Equal(GetMaterial(iX,iY), -1))) return;
	
	// behind everything (like the burn mark of the explosion)
	var boom;
	if(!ObjectCount(BOOM)) boom = CreateObject(BOOM,nil,nil,-1);
	else boom = FindObject(BOOM);
	
	var r = Random(360);
	var xdir = Sin(r,100);
	var ydir = -(Cos(r,100) ?? 0);
	CreateParticle("AlienSplatter",iX,iY,xdir,ydir,(iSize ?? 0)*5,iColor,boom,1);
}

global func AlienBlood(int iSize, int iX, int iY, int iColor, object attach) {
	
	iSize ??= 0;
	iX ??= 0;
	iY ??= 0;
	iColor ??= 0;
	var rand = Random(360);
	for(var i=nil; (i ?? 0)<5; S42MigrationStep(i, 1)) {
		var size = (iSize ?? 0)*(RandomX(10,20) ?? 0)/15*(GetCon() ?? 0)/100;
		var angle = (rand ?? 0)+(i ?? 0)*(RandomX(50,80) ?? 0);
		var xdir = Sin(angle,100);
		var ydir = -(Cos(angle,100) ?? 0);
		var xoff = Sin(angle,(iSize ?? 0)/2-5);
		var yoff = -(Cos(angle,(iSize ?? 0)/2-5) ?? 0);
		CreateParticle("AlienBlood",(iX ?? 0)+(xoff ?? 0),(iY ?? 0)+(yoff ?? 0),xdir,ydir,(iSize ?? 0)*5,iColor, attach);
	}
	
}

global func AlienHive(int iSize, int iX, int iY, int iColor, object attach, bool foreground) {
	
	// not on sky
	//if(GetMaterialVal("Density","Material",GetMaterial(iX,iY)) != 0
    //   || GetMaterial(iX,iY) == -1) return();
	
	iSize ??= 0;
	iX ??= 0;
	iY ??= 0;
	iColor ??= 0;
	foreground ??= false;
	var boom;	
	// to which object (if any)
	if(attach) boom = attach;
	else if(!foreground) {
		if(!ObjectCount(BOOM)) boom = CreateObject(BOOM,nil,nil,-1);
		else boom = FindObject(BOOM);
	}
	
	var r = Random(360);
	var xdir = Sin(r,100);
	var ydir = -(Cos(r,100) ?? 0);
	CreateParticle("AlienHive",iX,iY,xdir,ydir,(iSize ?? 0)*5,iColor,boom,!foreground);
}

/* Alienpartikel */

global func AddAlienFlareEffect(object target, int size) {
	size ??= 0;
	if(!target) target = this;
	if(!target) return;
	
	AddEffect("AlienFlareEffect", target, 1, 1, nil, nil,size);
	
}

global func FxAlienFlareEffectStart(object target, int effect, int temp, size) {
	effect ??= 0;
	temp ??= 0;
	EffectVar(nil,target,effect) = size;
}

global func FxAlienFlareEffectTimer(object target, int effect, int time)
{
  effect ??= 0;
  time ??= 0;
  var xdir = GetXDir(target);
  var ydir = GetYDir(target);
  var size = (EffectVar(nil,target,effect) ?? 0)+(RandomX(-100,100) ?? 0);
 
  if(!Contained(target))
        CreateParticle("FrSprk", GetX(target), GetY(target), (xdir ?? 0)/3+(RandomX(-10, 10) ?? 0), (ydir ?? 0)/3+(RandomX(-10, 10) ?? 0),
                 size, RGBa(160,255,nil,nil));
}
