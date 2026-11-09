/* Explosion */

#strict 3

//Explode mit Effekt und tollen Extraparametern für performance-/effektmäßiges Zeug
global func Explode(int iLevel, object pObj, id idEffect, string szEffect, bool bNoShots) {
  iLevel ??= 0;
  bNoShots ??= false;
  if(!pObj)
    if(!(pObj=this()))
      return;
  
  var x = AbsX(pObj->GetX()),
  	  y = AbsY(pObj->GetY());
  
  var i=nil, count = 3+(iLevel ?? 0)/8, angle = Random(360);
  while(And((count ?? 0) > 0, S42MigrationStep(i, 1) < (count ?? 0)*10)) {
    S42MigrationAssign(angle, "+=", (RandomX(40,80) ?? 0));

    if(!bNoShots)
      if(GetName(nil,SHT1)) { //Gibt es das Objekt? (HAX)
        var ammo = CreateObject(SHT1,x,y,GetController(pObj));
        ammo->Launch(angle,100+(iLevel ?? 0)*3,Max(1,RandomX(30,(iLevel ?? 0)*8)),4,50, 3, nil,1);
      }
	  
    //Rauch
    var smokex = +(Sin(angle,RandomX((iLevel ?? 0)/4,(iLevel ?? 0)/2)) ?? 0);
    var smokey = -(Cos(angle,RandomX((iLevel ?? 0)/4,(iLevel ?? 0)/2)) ?? 0);
    if(GBackSolid((x ?? 0)+(smokex ?? 0),(y ?? 0)+(smokey ?? 0)))
      continue;
     var level = (iLevel ?? 0) + (Random((iLevel ?? 0)/5) ?? 0);
    CreateSmokeTrail(level,angle,smokex,smokey,pObj);
    S42MigrationStep(count, -1, true);
  }

  CreateBurnMark(x,y,iLevel,20+(iLevel ?? 0)/2);

  if(IsDark()) {
    var iSize = (iLevel ?? 0)*100;
    if((iLevel ?? 0) < 20) {
      S42MigrationAssign(iSize, "/=", 2);
    }
    AddLightFlash((iSize ?? 0)/3, x, y, RGBa(255,220,64,15));
  }

  return(inherited(iLevel, pObj, idEffect, szEffect));
}

global func CreateBurnMark(int iX, int iY, int iLevel, int Count) 
{
  iX ??= 0;
  iY ??= 0;
  iLevel ??= 0;
  Count ??= 0;
  var boom;
  if(!ObjectCount(BOOM)) boom = CreateObject(BOOM,nil,nil,-1);
  else boom = FindObject(BOOM);
  
  var angle=Random(360/(Count ?? 0)); //variablen für die überprüfung
  var type;
  for(var z; (z ?? 0) < (Count ?? 0); S42MigrationStep(z, 1, true)) {

    S42MigrationAssign(angle, "+=", (Max(1,360/(Count ?? 0)) ?? 0));

    // Check: Sky or Solid/Liquid
    var x = (iX ?? 0)+(Cos(angle,iLevel) ?? 0);
    var y = (iY ?? 0)+(Sin(angle,iLevel) ?? 0);
    if(Or(!Equal(GetMaterialVal("Density","Material",GetMaterial(x,y)), nil)
       , Equal(GetMaterial(x,y), -1)))
      continue;

    type = 1+(Random(3) ?? 0);
    var size = BurnMarkCheck(angle,RandomX((iLevel ?? 0)/2,(iLevel ?? 0)*2),iX,iY);
    var sin = Sin(angle,((size ?? 0)-(iLevel ?? 0))/2+(iLevel ?? 0)+(Random(3) ?? 0));
    var cos = Cos(angle,((size ?? 0)-(iLevel ?? 0))/2+(iLevel ?? 0)+(Random(3) ?? 0));

    CreateParticle("BurnMark",(iX ?? 0)+(cos ?? 0),(iY ?? 0)+(sin ?? 0),Cos((angle ?? 0)+(RandomX(-5,5) ?? 0),50),Sin((angle ?? 0)+(RandomX(-5,5) ?? 0),50),(size ?? 0)*5+(Random(25) ?? 0),RGBa(nil,nil,nil,64),boom,1);
  }

}

global func BurnMarkCheck(int angle,int size, int iX, int iY) {
  angle ??= 0;
  size ??= 0;
  iX ??= 0;
  iY ??= 0;
  var sin = Sin(angle,size);
  var cos = Cos(angle,size);
  var x = cos, y = sin, i=100;
  while(Or(Equal(GetMaterial((iX ?? 0)+(x ?? 0),(iY ?? 0)+(y ?? 0)), -1), !Equal(GetMaterialVal("Density","Material",GetMaterial((iX ?? 0)+(x ?? 0),(iY ?? 0)+(y ?? 0))), nil))) {
    x = (cos ?? 0)*(i ?? 0)/100;
    y = (sin ?? 0)*(i ?? 0)/100;
    if((i ?? 0) <= 0) return(nil);
    S42MigrationAssign(i, "-=", 10);
  }
  return(size);
}

global func CreateSmokeTrail(int iStrength, int iAngle, int iX, int iY, object pObj) {
//  Log("I...");
    iStrength ??= 0;
    iAngle ??= 0;
    iX ??= 0;
    iY ??= 0;
    S42MigrationAssign(iX, "+=", (GetX(pObj) ?? 0));
    S42MigrationAssign(iY, "+=", (GetY(pObj) ?? 0));
  AddEffect("SmokeTrail", nil, 300, 1, nil, nil, iStrength, iAngle, iX, iY);
}

// Variablen:
// 0 - Stärke
// 1 - momentane Stärke
// 2 - X-Position
// 3 - Y-Position
// 4 - Anfangs-X-Geschwindigkeit
// 5 - Anfangs-Y-Geschwindigkeit
global func FxSmokeTrailStart(object pTarget, int iEffectNumber, int iTemp, iStrength, iAngle, iX, iY) {

  iEffectNumber ??= 0;
  iTemp ??= 0;
  if(iTemp)
    return;
  
  if(Equal((iAngle ?? 0)%90, 1)) S42MigrationAssign(iAngle, "+=", 1);
  iStrength = Max(iStrength,5);

  EffectVar(nil, pTarget, iEffectNumber) = iStrength;
  EffectVar(1, pTarget, iEffectNumber) = iStrength;
  EffectVar(2, pTarget, iEffectNumber) = iX;
  EffectVar(3, pTarget, iEffectNumber) = iY;
  EffectVar(4, pTarget, iEffectNumber) = +(Sin(iAngle,(iStrength ?? 0)*40) ?? 0);
  EffectVar(5, pTarget, iEffectNumber) = -(Cos(iAngle,(iStrength ?? 0)*40) ?? 0);
}

global func FxSmokeTrailTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  var iStrength = EffectVar(nil, pTarget, iEffectNumber);
  var iAStr = EffectVar(1, pTarget, iEffectNumber);
  var iX = EffectVar(2, pTarget, iEffectNumber);
  var iY = EffectVar(3, pTarget, iEffectNumber);
  var iXDir = EffectVar(4, pTarget, iEffectNumber);
  var iYDir = EffectVar(5, pTarget, iEffectNumber);

  iAStr = Max(1,(iAStr ?? 0)-(iAStr ?? 0)/10+(Random(2) ?? 0));
  S42MigrationStep(iAStr, -1, true);
  S42MigrationAssign(iYDir, "+=", (GetGravity() ?? 0)*2/3);

  var xdir = (iXDir ?? 0)*(iAStr ?? 0)/(iStrength ?? 0);
  var ydir = (iYDir ?? 0)*(iAStr ?? 0)/(iStrength ?? 0);

  // Neu: Random
  S42MigrationAssign(iX, "+=", (RandomX(-3,3) ?? 0));
  S42MigrationAssign(iY, "+=", (RandomX(-3,3) ?? 0));
  
  // zuerst zeichnen
  CreateParticle("Smoke3",iX,iY,RandomX(-2,2),RandomX(-2,4),20+(iAStr ?? 0)*8,RGBa(75,65,55,55+35*(iAStr ?? 0)/(iStrength ?? 0)));
  CreateParticle("Blast",iX,iY,nil,nil,10+(iAStr ?? 0)*8,RGBa(250,100+(Random(100) ?? 0),100,160));

  // dann nächste position berechnen
  S42MigrationAssign(iX, "+=", (xdir ?? 0)/100);
  S42MigrationAssign(iY, "+=", (ydir ?? 0)/100);
  
  if(GBackSemiSolid(iX,iY))
    return(-1);
  if((iAStr ?? 0) <= 1)
    return(-1);
    
  EffectVar(1, pTarget, iEffectNumber) = iAStr;
  EffectVar(2, pTarget, iEffectNumber) = iX;
  EffectVar(3, pTarget, iEffectNumber) = iY;
  EffectVar(5, pTarget, iEffectNumber) = iYDir;
}

global func FxSmokeTrailStop() {
//  Log("Nikotine");
}

