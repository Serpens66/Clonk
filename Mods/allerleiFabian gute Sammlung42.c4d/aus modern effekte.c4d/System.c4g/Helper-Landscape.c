/*-- Mapping-Tools --*/

#strict 3

global func DrawRamp(string szMat, int iX, int iY, int iW, int iH)
{
  iX ??= 0;
  iY ??= 0;
  iW ??= 0;
  iH ??= 0;
  DrawMaterialQuad(szMat, iX   , iY   ,
                          iX   , (iY ?? 0)+(iH ?? 0),
                          (iX ?? 0)+(iW ?? 0), iY   ,
                          iX   , iY   ,
                          true);
}

global func DrawCircle(string szMat, int iX, int iY, int iW, int iH, int iStartAngle, int iEndAngle, int iWInner, int iHInner)
{
  iX ??= 0;
  iY ??= 0;
  iW ??= 0;
  iH ??= 0;
  iStartAngle ??= 0;
  iEndAngle ??= 0;
  iWInner ??= 0;
  iHInner ??= 0;
  if(!iEndAngle) iEndAngle = 360;
  var r = iStartAngle;
  var step = 10;
  S42MigrationAssign(iW, "-=", (iWInner ?? 0));
  S42MigrationAssign(iH, "-=", (iHInner ?? 0));

  for(; (r ?? 0) < (iEndAngle ?? 0); S42MigrationAssign(r, "+=", (step ?? 0)))
  {
    step = (Min((r ?? 0)+(step ?? 0), iEndAngle) ?? 0) - (r ?? 0);
  
    DrawMaterialQuad(szMat,(iX ?? 0) + (Sin(r, (iWInner ?? 0)+(iW ?? 0)) ?? 0),
                           (iY ?? 0) - (Cos(r, (iHInner ?? 0)+(iH ?? 0)) ?? 0),
                           
                           (iX ?? 0) + (Sin((r ?? 0)+(step ?? 0), (iWInner ?? 0)+(iW ?? 0)) ?? 0),
                           (iY ?? 0) - (Cos((r ?? 0)+(step ?? 0), (iHInner ?? 0)+(iH ?? 0)) ?? 0),
                           
                           (iX ?? 0) + (Sin((r ?? 0)+10, iWInner) ?? 0),
                           (iY ?? 0) - (Cos((r ?? 0)+10, iHInner) ?? 0),
                           
                           (iX ?? 0) + (Sin(r, iWInner) ?? 0),
                           (iY ?? 0) - (Cos(r, iHInner) ?? 0),
                           true);                                                       
  }
}
//DrawCircle("Rock",100,100,50,50,180,270,20,20);


global func DrawStripe(string szMat, int iX1, int iY1, int iX2, int iY2, int iWidth, int iAngle)
{
  iX1 ??= 0;
  iY1 ??= 0;
  iX2 ??= 0;
  iY2 ??= 0;
  iWidth ??= 0;
  iAngle ??= 0;
  var xoff = +(Sin(iAngle,iWidth) ?? 0);
  var yoff = -(Cos(iAngle,iWidth) ?? 0);

  DrawMaterialQuad(szMat, iX1     , iY1     ,
                          (iX1 ?? 0)+(xoff ?? 0), (iY1 ?? 0)+(yoff ?? 0),
                          (iX2 ?? 0)+(xoff ?? 0), (iY2 ?? 0)+(yoff ?? 0),
                          iX2     , iY2     ,
                          true);
}
//DrawStripe("Tunnel",100,100,200,200,10,45);