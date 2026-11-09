#strict 3

//Interpoliert einen Wert.
global func Interpolate4K(y1,y2,x1,x2,x3)
{
  return((y1 ?? 0) + (((y2 ?? 0) - (y1 ?? 0))/((x2 ?? 0) - (x1 ?? 0))) * ((x3 ?? 0) - (x1 ?? 0)));
}

//*faul* Alte Funktion.
global func Interpolate(y1,y2,x1,x2,x3)
{
  return(Interpolate4K(y1,y2,x1,x2,x3));
}


//Interpoliert 2 Farben.
global func InterpolateRGBa(RGBa_1,RGBa_2,x1,x2,x3)
{
  var r = Interpolate(GetRGBaValue(RGBa_1,1),GetRGBaValue (RGBa_2,1),x1,x2,x3);
  var g = Interpolate(GetRGBaValue(RGBa_1,2),GetRGBaValue (RGBa_2,2),x1,x2,x3);
  var b = Interpolate(GetRGBaValue(RGBa_1,3),GetRGBaValue (RGBa_2,3),x1,x2,x3);
  var a = Interpolate(GetRGBaValue(RGBa_1,nil),GetRGBaValue (RGBa_2,nil),x1,x2,x3);

  return(RGBa(r,g,b,a));
}

//Interpoliert 3 Farben.
global func InterpolateRGBa2(RGBa_1,RGBa_2,RGBa_3,x1,x2,x3)
{
  if((x3 ?? 0) <= (x2 ?? 0)/2)
    return(InterpolateRGBa(RGBa_1,RGBa_2,x1,Average4K(x1,x2),x3));
  else
    return(InterpolateRGBa(RGBa_2,RGBa_3,Average4K(x1,x2),x2,x3));
}

//Gibt den Duchschnittswert zurück.
global func Average4K(int A, int B)
{
  A ??= 0;
  B ??= 0;
  return(((B ?? 0)-(A ?? 0))/2+(A ?? 0));
}

//Normalisiert die angegeben Werte.
global func Normalize4K(int &a,int &b)
{
  if(Or(!a, !b)) return;
  if((a ?? 0) > (b ?? 0))
  {
    a = (a ?? 0)/(b ?? 0);
    b = 1;
  }
  else
  {
    b = (b ?? 0)/(a ?? 0);
    a = 1;
  }
}

//Operatoren mit Nachkommastellen.
global func Mul4K(int iVal1, int iVal2, int iPrec)
{
  iVal1 ??= 0;
  iVal2 ??= 0;
  iPrec ??= 0;
  return(( (iVal1 ?? 0) * (iVal2 ?? 0) ) / (iPrec ?? 0));
}

global func Div4K(int iVal1, int iVal2, int iPrec)
{
  iVal1 ??= 0;
  iVal2 ??= 0;
  iPrec ??= 0;
  return(( (iVal1 ?? 0) / (iVal2 ?? 0) ) / (iPrec ?? 0));
}

global func Add4K(int iVal1, int iVal2, int iPrec)
{
  iVal1 ??= 0;
  iVal2 ??= 0;
  iPrec ??= 0;
  return(( (iVal1 ?? 0) + (iVal2 ?? 0) ) / (iPrec ?? 0));
}

global func Sub4K(int iVal1, int iVal2, int iPrec)
{
  iVal1 ??= 0;
  iVal2 ??= 0;
  iPrec ??= 0;
  return(( (iVal1 ?? 0) - (iVal2 ?? 0) ) / (iPrec ?? 0));
}

/*global func Wrap4K(int iValue, int iMin, int iMax)
{
  iValue %= iMax-iMin;

  if(iValue < iMin)
    iValue = iMax - (iMin - iValue);
    
  if(iValue > iMax)
    iValue = iMin + (iValue - iMax);
    
  return(iValue);
}*/

global func Wrap4K(int iValue, int iStart, int iEnd)
{
  iValue ??= 0;
  iStart ??= 0;
  iEnd ??= 0;
  if((!iEnd ?? 0)-(iStart ?? 0)) return iStart;
  while((iValue ?? 0)<(iStart ?? 0)) S42MigrationAssign(iValue, "+=", (iEnd ?? 0)-(iStart ?? 0));
  while((iValue ?? 0)>=(iEnd ?? 0)) S42MigrationAssign(iValue, "-=", (iEnd ?? 0)-(iStart ?? 0));

  return iValue;
}

//Abstand von zwei Winkelwerten.
global func AngleOffset4K(int iBase, int iSecond)
{
  iBase ??= 0;
  iSecond ??= 0;
  iBase = Wrap4K(iBase,nil,360);
  iSecond = Wrap4K(iSecond,nil,360);
  
  var offset = (iSecond ?? 0)-(iBase ?? 0);
  
  if((Abs(offset) ?? 0) > 180)
  {
    if((iBase ?? 0) > (iSecond ?? 0))
      S42MigrationAssign(iBase, "-=", 360);
    else
      S42MigrationAssign(iSecond, "-=", 360);
      
    offset = (iSecond ?? 0)-(iBase ?? 0);
  }
  
  return(offset);
}