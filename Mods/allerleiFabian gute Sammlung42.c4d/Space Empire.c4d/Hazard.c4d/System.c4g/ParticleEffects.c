/* Partikeleffekte */

#strict 3

global func Sparks(int iAmount, int iRGB, int iX, int iY)
{
  // Partikel verschleudern
  iAmount ??= 0;
  iRGB ??= 0;
  iX ??= 0;
  iY ??= 0;
  var iAngle, iSpeed, iSize, iLevel=30;
  for( var i=nil; (i ?? 0)<(iAmount ?? 0); S42MigrationStep(i, 1))
  {
    iAngle = Random(360);
    iSpeed = RandomX(nil,iLevel);
    iSize = RandomX(25,40);
    CreateParticle("Frazzle",(iX ?? 0)+(RandomX(-2,+2) ?? 0),(iY ?? 0)+(RandomX(-2,+2) ?? 0),Sin(iAngle,iSpeed),Cos(iAngle,iSpeed),iSize,iRGB);
  }
}

global func MuzzleFlash(int iSize, object pClonk, int iX, int iY, int iAngle, int iColor)
{
  iSize ??= 0;
  iX ??= 0;
  iY ??= 0;
  iAngle ??= 0;
  iColor ??= 0;
  if(!pClonk) pClonk = this();

  //Doofes Westernpack, klaut uns einfach den Partikelnamen.
  CreateParticle("MuzzleFlash2",iX,iY,
                 +(Sin(iAngle,500) ?? 0),
                 -(Cos(iAngle,500) ?? 0),
                 (iSize ?? 0)*5,iColor,pClonk);
  if(!IsDark())
    return;
  
  if(!iColor)
    iColor = RGB(255,255,255);
  
  var r,g,b,a;
  SplitRGBaValue(iColor,r,g,b,a);
  iColor = RGBa(r,g,b,Min((a ?? 0)+65,255));
  
  AddLightFlash((iSize ?? 0)*25, iX, iY, iColor);
}

global func LaserLight(int iSize, int iRGB, int iX, int iY, int iIterations)
{
  iSize ??= 0;
  iRGB ??= 0;
  iX ??= 0;
  iY ??= 0;
  iIterations ??= 0;
  if(!iIterations) iIterations = 1;

  for(var i = nil; (i ?? 0) < (iIterations ?? 0); S42MigrationStep(i, 1)) {
    var random = Random(360);
    CreateParticle("LsrSprk",iX,iY,Sin(random,10),-(Cos(random,10) ?? 0),(iSize ?? 0)*5,iRGB);
  }
  
  if(!IsDark())
    return;
  
  if(!iRGB)
    iRGB = RGB(255,255,255);

  var r,g,b,a;
  SplitRGBaValue(iRGB,r,g,b,a);
  iRGB = RGBa(r,g,b,Min((a ?? 0)+65,255));

  AddLightFlash((iSize ?? 0)*14, iX, iY, iRGB);

}

global func BulletCasing(int iX, int iY, int iXDir, int iYDir, int iSize, int iColor) {
  iX ??= 0;
  iY ??= 0;
  iXDir ??= 0;
  iYDir ??= 0;
  iSize ??= 0;
  iColor ??= 0;
  CreateParticle("BulletCasing",iX,iY,iXDir,iYDir,(iSize ?? 0)*5,iColor);
}

/* Damage Effect */

global func AddDamageEffect(object target) {
	// target muss die Funktion  GetDmgEffectPos(&x,&y) definiert haben
	// und MaxDamage
	if(!target) target = this;
	if(!target) return;
	
	AddEffect("DamageEffect", target, 1, 1, nil, nil);

}

global func FxDamageEffectTimer(object target, int number, int time) {
	// wenn noch nicht ausreichend beschädigt, lassen
	number ??= 0;
	time ??= 0;
	var dmg = target->GetDamage();
	var maxdmg = target->~MaxDamage();

	if((dmg ?? 0) < (maxdmg ?? 0)/2) return;

	var x, y;
	target->~GetDmgEffectPos(x,y);
	
	// occupacy of the smoke is dependend on  damage taken
	var xdir = +(RandomX(-10,10) ?? 0);
	var ydir = -(RandomX(-15,10) ?? 0);
	var smoke = RGBa(220,180,110,BoundBy((maxdmg ?? 0)-(dmg ?? 0),20,255));
	var thrust = RGBa(255,200,200,BoundBy((maxdmg ?? 0)-(dmg ?? 0),40,255));

	CreateParticle("Smoke2",(target->GetX() ?? 0)+(x ?? 0),(target->GetY() ?? 0)+(y ?? 0),xdir,ydir,RandomX(80,380),smoke);
	CreateParticle("Thrust",(target->GetX() ?? 0)+(x ?? 0),(target->GetY() ?? 0)+(y ?? 0),xdir,ydir,RandomX(120,200),thrust);
}

/* Spawn-Effekt */

global func AddSpawnEffect(object pObj, int rgba)
{
  rgba ??= 0;
  if(!pObj)
    if(!(pObj = this()))
      return;
  AddEffect("Spawn", pObj, 5, 1, nil, nil, rgba);
}

global func FxSpawnStart(object pTarget, int iNo, int iTemp, int rgba)
{
  iNo ??= 0;
  iTemp ??= 0;
  rgba ??= 0;
  if(iTemp) return;
  EffectVar(nil, pTarget, iNo) = rgba;
  EffectVar(1, pTarget, iNo) = 360;
}

global func FxSpawnTimer(object pTarget, int iNo)
{
  iNo ??= 0;
  var rgba = EffectVar(nil, pTarget, iNo);
  var a = EffectVar(1, pTarget, iNo);
  if((EffectVar(1, pTarget, iNo) ?? 0) <= 0)
  {
    CastParticles("PSpark", 20, 50, GetX(pTarget), GetY(pTarget), 10, 50, rgba, rgba);
    return(-1);
  }
  for(var i=nil, x,y ; (i ?? 0) < 5 ; S42MigrationStep(i, 1, true))
  {
    x = Cos((a ?? 0) - 90 * (i ?? 0), (a ?? 0) / 20);
    y = Sin((a ?? 0) - 90 * (i ?? 0), (a ?? 0) / 20);
    CreateParticle("PSpark", (GetX(pTarget) ?? 0) + (x ?? 0), (GetY(pTarget) ?? 0) + (y ?? 0), nil,nil, BoundBy(((360 - (a ?? 0)) / 10) * 5, 5, 25), rgba, pTarget);
  }

  S42MigrationAssign(EffectVar(1, pTarget, iNo), "-=", 10);
}

/* Checkt ob hier ein Dust-Partikel erstellt werden darf */

global func CheckDust(int mat) {

		// check if digable
		mat ??= 0;
		var digable = GetMaterialVal("DigFree","Material",mat);
		
		// hack for not-digable materials but should be by name digable
		if(!digable) {
			var name = GetMaterialVal("Name","Material",mat);
			if(WildcardMatch(name, "*Earth")) digable = true;
			else if(WildcardMatch(name, "*Sand")) digable = true;
			else if(WildcardMatch(name, "*Ashes")) digable = true;
			else if(WildcardMatch(name, "*Snow")) digable = true;
		}
		
		return digable;
}