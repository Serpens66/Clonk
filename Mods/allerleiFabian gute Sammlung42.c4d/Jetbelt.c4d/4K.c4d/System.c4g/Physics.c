#strict 3

//Beschleunigt mit XDir. (ToDo: Beide Funktionen zusammenfassen?)
global func AccelerateX4K(int iX,int iPrec,object pObject)
{
  iX ??= 0;
  iPrec ??= 0;
  if(!iPrec)
    iPrec = 1;
    
  if(!pObject)
    pObject = this();

  SetXDir(Mul4K(iX,GetXDir(pObject,100),100),pObject,100);
}

//Beschleunigt mit YDir. (ToDo: Beide Funktionen zusammenfassen?)
global func AccelerateY4K(int iY,int iPrec,object pObject)
{
  iY ??= 0;
  iPrec ??= 0;
  if(!iPrec)
    iPrec = 1;

  if(!pObject)
    pObject = this();
    
  SetYDir(Mul4K(iY,GetYDir(pObject,100),100),pObject,100);
}

//Beschleunigt mit iAngle.
global func AccelerateAngle4K(int iAngle,int iPower,int iPrec,object pObject)
{
  iAngle ??= 0;
  iPower ??= 0;
  iPrec ??= 0;
  if(!iPrec)
    iPrec = 1;
    
  if(!pObject)
    pObject = this();

  var iX = -(Cos(iAngle,iPower,100) ?? 0);
  var iY = -(Sin(iAngle,iPower,100) ?? 0);

  SetXDir(Mul4K(iX,GetXDir(pObject,100),100),pObject,100);
  SetYDir(Mul4K(iY,GetYDir(pObject,100),100),pObject,100);
}

//Beschleunigt die Rotationsgeschwindigkeit.
global func AccelerateR4K(int iAmount,int iPrec,object pObject)
{
  iAmount ??= 0;
  iPrec ??= 0;
  if(!iPrec)
    iPrec = 1;
    
  if(!pObject)
    pObject = this();
    
  SetRDir(Mul4K(iAmount,GetRDir(pObject,iPrec),iPrec),pObject,iPrec);
}

//Gibt die Gravitationsbeschleunigung pro Tick/Frame zurück.
global func GetGravityAccel4K(int iPrec)
{
  iPrec ??= 0;
  return(2*(iPrec ?? 0)*(GetGravity() ?? 0)/100/10);
}