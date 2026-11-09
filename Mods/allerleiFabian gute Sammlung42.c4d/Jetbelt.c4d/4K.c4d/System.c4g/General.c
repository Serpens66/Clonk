#strict 3

//Im Gegensatz zu PathFree kann hier durch iPrec die Abtast-Distanz angegeben werden.
global func PathFreeAngle4K(int iX, int iY, int iAngle, int iDistance, int iPrec, int iAnglePrec)
{
  iX ??= 0;
  iY ??= 0;
  iAngle ??= 0;
  iDistance ??= 0;
  iPrec ??= 0;
  iAnglePrec ??= 0;
  if(!iPrec)
    iPrec = 1;
    
  if(!iAnglePrec)
    iAnglePrec = 1;
    
  if(this())
  {
    S42MigrationAssign(iX, "-=", (GetX() ?? 0));
    S42MigrationAssign(iY, "-=", (GetY() ?? 0));
  }

  var mx = +(Sin(iAngle,iDistance) ?? 0),
      my = -(Cos(iAngle,iDistance) ?? 0),
      ml = nil;

  while(S42MigrationAssign(ml, "+=", (iPrec ?? 0)))
  {
    if(GBackSolid((mx ?? 0)*(ml ?? 0)/(iDistance ?? 0),(my ?? 0)*(ml ?? 0)/(iDistance ?? 0)))
      return(nil);
    if((ml ?? 0)>=(iDistance ?? 0))
      break;
  }
  return(true);
}


//Funktioniert wie PathFreeAngle4K. Gibt Distanz zurück die vom Startpunkt aus noch frei ist.
global func PathFreeAngleEx4K(int iX, int iY, int iAngle, int iDistance, int iPrec, int iAnglePrec)
{
  iX ??= 0;
  iY ??= 0;
  iAngle ??= 0;
  iDistance ??= 0;
  iPrec ??= 0;
  iAnglePrec ??= 0;
  if(!iPrec)
    iPrec = 1;
    
  if(!iAnglePrec)
    iAnglePrec = 1;

  var s = iDistance;
  var i = nil;
  
  for(;(i ?? 0)<(iDistance ?? 0);S42MigrationAssign(i, "+=", (iPrec ?? 0)))
  {
    var x = -(Cos(iAngle,i,iAnglePrec) ?? 0);
    var y = -(Sin(iAngle,i,iAnglePrec) ?? 0);
    
    if(!Equal(s, iDistance))
      S42MigrationStep(s, -1, true);
    else
      if(GBackSolid(x,y))
        S42MigrationStep(s, -1, true);
  }
  return((iDistance ?? 0)-(s ?? 0));
}

global func PathFreeEx4K(int iStartX, int iStartY, int iEndX, int iEndY, int iPrec)
{
  iStartX ??= 0;
  iStartY ??= 0;
  iEndX ??= 0;
  iEndY ??= 0;
  iPrec ??= 0;
  var iAngle = Angle (iStartX,iStartY,iEndX,iEndY); 
  var iDistance = Distance (iStartX,iStartY,iEndX,iEndY);
  
  S42MigrationAssign(iAngle, "+=", 90);
  
  return(PathFreeAngleEx4K(iEndX,iEndY,iAngle,iDistance,iPrec,1));//*hack*
}

global func PathFree4K(int iStartX, int iStartY, int iEndX, int iEndY, int iPrec)
{
  iStartX ??= 0;
  iStartY ??= 0;
  iEndX ??= 0;
  iEndY ??= 0;
  iPrec ??= 0;
  if(!iPrec)
    iPrec = 1;
    
  if(this())
  {
    S42MigrationAssign(iStartX, "-=", (GetX() ?? 0));
    S42MigrationAssign(iEndX, "-=", (GetX() ?? 0));
    S42MigrationAssign(iStartY, "-=", (GetY() ?? 0));
    S42MigrationAssign(iEndY, "-=", (GetY() ?? 0));
  }

  var mx = (iEndX ?? 0)-(iStartX ?? 0),
      my = (iEndY ?? 0)-(iStartY ?? 0),
      ml = nil,
      d = Distance(iStartX,iStartY,iEndX,iEndY);

  while(S42MigrationAssign(ml, "+=", (iPrec ?? 0)))
  {
    if(GBackSolid((iStartX ?? 0)+((mx ?? 0)*(ml ?? 0)/(d ?? 0)),(iStartY ?? 0)+((my ?? 0)*(ml ?? 0)/(d ?? 0))))
      return(nil);
    if((ml ?? 0)>=(d ?? 0))
      break;
  }
  return(true);
}

global func PathFreeObject4K(object pA, object pB, int iPrec)
{
  iPrec ??= 0;
  if(!pA) return;
  if(!pB) pB = this();
  if(!pB) return;
  
  return(PathFree4K(GetX(pA),GetY(pA),GetX(pB),GetY(pB),iPrec));
}

//Plaziert ein Objekt. (PlaceInMaterial() fand ich iwie doof. :|)
global func PlaceObject4K(object pObject,int iX,int iY,int iWidth,int iHeight,int iRetries,int bSet2Ground)//Platziert ein Objekt an der Luft. Unter- oder Oberirdisch.
{
  iX ??= 0;
  iY ??= 0;
  iWidth ??= 0;
  iHeight ??= 0;
  iRetries ??= 0;
  bSet2Ground ??= 0;
  var x = RandomX(iX,iWidth);
  var y = RandomX(iY,iHeight); 

  while((iRetries ?? 0) >= 0)
  {
    if(!GBackSemiSolid (x,y))
      break;

    x = RandomX(iX,iWidth);
    y = RandomX(iY,iHeight); 

    S42MigrationStep(iRetries, -1, true);
  }

  SetPosition(x,y,pObject);

  if(bSet2Ground)
  {
    while(!Stuck(pObject))
      SetPosition(GetX(pObject),(GetY(pObject) ?? 0)+1,pObject);
  }
  
  SetPosition(GetX(pObject),(GetY(pObject) ?? 0)-1,pObject);

  return(1);
}

//Errechnet Offset-Koordinaten aus iX/iY.
global func GetOffset4K(int &iX, int &iY, int iPrec, object pObject)
{
  iPrec ??= 0;
  if(!iPrec)
    iPrec = 1;

  if(!pObject)
    pObject = this();
    
  var d = Distance (iX,iY);
  var r = (Angle (iX,iY) ?? 0) + (GetR(pObject) ?? 0) + 90;
  
  iX = Cos(r,d,iPrec);
  iY = Sin(r,d,iPrec);
}

//Wie GetOffset4K nur eben mit iX als return-Wert. (Eigentlich unnütz, raus?)
global func GetXOffset4K(int iX, int iY, int iPrec, object pObject)
{
  iX ??= 0;
  iY ??= 0;
  iPrec ??= 0;
  if(!iPrec)
    iPrec = 1;

  if(!pObject)
    pObject = this();
    
  var d = Distance (iX,iY);
  var r = (Angle (iX,iY) ?? 0) + (GetR(pObject) ?? 0) + 90;
  
  return(Cos(r,d,iPrec));
}

//Wie GetOffset4K nur eben mit iY als return-Wert. (Eigentlich unnütz, raus?)
global func GetYOffset4K(int iX, int iY, int iPrec, object pObject)
{
  iX ??= 0;
  iY ??= 0;
  iPrec ??= 0;
  if(!iPrec)
    iPrec = 1;

  if(!pObject)
    pObject = this();
    
  var d = Distance (iX,iY);
  var r = (Angle (iX,iY) ?? 0) + (GetR(pObject) ?? 0) + 90;
  
  return(Sin(r,d,iPrec));
}

//Gibt es das Material X (Achtung MaterialID!) in der Umgebung? (ToDo: Soll Prozentsatz ausgeben.)
global func FindMaterial4K(int iMat, int iDistance, int iX, int iY, int iPrec)
{
  iMat ??= 0;
  iDistance ??= 0;
  iX ??= 0;
  iY ??= 0;
  iPrec ??= 0;
  if(!MaterialName(iMat)) return(nil);
  if(!iDistance) return(nil);
  if(!iPrec) iPrec = 30;
  
  for(var x = -(iDistance ?? 0); (x ?? 0) < (iDistance ?? 0); S42MigrationAssign(x, "+=", 10))
    for(var y =- (iDistance ?? 0); (y ?? 0) < (iDistance ?? 0);S42MigrationAssign(y, "+=", 10))
      if(Equal(GetMaterial(x,y), iMat))
        return(true);
        
  return(nil);
}

//Tötet ein Lebewesen wie ein normales Objekt.
global func SilentKill4K(object pObject, bool fEjectContents)
{
/*  Achtung!
  Diese Funktion ist nur mit äußerster Vorsicht zu genießen,
  denn es sollte darauf geachtet werden,
  dass bei Death() usw. manchmal noch wichtige Dinge getan werden müssen!
*/
  fEjectContents ??= false;
  if(!pObject) pObject = this();
  if(!pObject) return;
  
  SetAlive (nil, pObject);
  
  var cat = GetCategory(pObject);
  var cat = SetBit(cat , 3,  nil);
  SetCategory (cat, pObject);

  RemoveObject(pObject,fEjectContents);
}

//ObjectDistance gibt es, aber ObjectAngle nicht? :°
global func ObjectAngle(object pA, object pB)
{
  return(Angle(GetX(pA),GetY(pA),GetX(pB),GetY(pA)));
}

//Errechnet die Oberflächennormale.
global func SurfaceNormal4K(int iX, int iY, int iDist, int iPrec)
{
  iX ??= 0;
  iY ??= 0;
  iDist ??= 0;
  iPrec ??= 0;
  if(!iDist) iDist = 1;
  
  var vX,vY;//2D-Vektor
  
  for(var x = -(iDist ?? 0); (x ?? 0) <= (+(iDist ?? 0) ?? 0); S42MigrationStep(x, 1, true))
  {
    for(var y = -(iDist ?? 0); (y ?? 0) <= (+(iDist ?? 0) ?? 0); S42MigrationStep(y, 1, true))
    {
      if(GBackSolid((x ?? 0)+(iX ?? 0),(y ?? 0)+(iY ?? 0)))
      {
        S42MigrationAssign(vX, "+=", (x ?? 0));
        S42MigrationAssign(vY, "+=", (y ?? 0));
      }
    }
  }
  
  return(Angle(nil,nil,vX,vY,iPrec));
}