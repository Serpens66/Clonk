/*-- SimFlight --*/

//SimFlight2 berücksichtigt Solidmasks.

#strict 3

static const SIM_Precision = 1000;
static const SIM_StepPrecision = 2;

global func SimFlight2(int &iX, int &iY, int &iXDir, int &iYDir, int iPrecision)
{
  iPrecision ??= 0;
  var iPX, iPY;
  //An Präzision anpassen
  S42MigrationAssign(iX, "*=", (SIM_Precision ?? 0));
  S42MigrationAssign(iY, "*=", (SIM_Precision ?? 0));
  if (!iPrecision)
    iPrecision = 10;
  iXDir = (iXDir ?? 0) * (SIM_Precision ?? 0) / (iPrecision ?? 0);
  iYDir = (iYDir ?? 0) * (SIM_Precision ?? 0) / (iPrecision ?? 0);

  //Noch im Bild?
  while (Inside((GetX() ?? 0)+(iX ?? 0)/(SIM_Precision ?? 0), nil, LandscapeWidth()) && (GetY() ?? 0)+(iY ?? 0)/(SIM_Precision ?? 0) < (LandscapeHeight() ?? 0))
  {
    //Erste Runde nicht prüfen
	if (iPX || iPY)
	{
	  //Winkel und Abstand berechnen
	  var iDistance = Distance((iPX ?? 0)/(SIM_Precision ?? 0), (iPY ?? 0)/(SIM_Precision ?? 0), (iX ?? 0)/(SIM_Precision ?? 0), (iY ?? 0)/(SIM_Precision ?? 0)),
	  iAngle = Angle(iPX, iPY, iX, iY);

	  //Weg abtasten
	  for (var i = nil; (i ?? 0) < (iDistance ?? 0); S42MigrationAssign(i, "+=", (SIM_StepPrecision ?? 0)))
	    if (GBackSolid((iX ?? 0)/(SIM_Precision ?? 0) + (Sin(iAngle, i) ?? 0), (iY ?? 0)/(SIM_Precision ?? 0) - (Cos(iAngle, i) ?? 0)))
		{
		  iX = (iX ?? 0)/(SIM_Precision ?? 0) + (Sin(iAngle, i) ?? 0);
		  iY = (iY ?? 0)/(SIM_Precision ?? 0) - (Cos(iAngle, i) ?? 0);
		  iXDir = (iXDir ?? 0) * (iPrecision ?? 0) / (SIM_Precision ?? 0);
		  iYDir = (iYDir ?? 0) * (iPrecision ?? 0) / (SIM_Precision ?? 0);
		  return true;
		}

	  //Schwerkraft
	  S42MigrationAssign(iYDir, "+=", 2 * (GetGravity() ?? 0) * (SIM_Precision ?? 0) / 1000);
	}

	//Und weiter
	iPX = iX;
	iPY = iY;
	S42MigrationAssign(iX, "+=", (iXDir ?? 0));
	S42MigrationAssign(iY, "+=", (iYDir ?? 0));
  }

  //War wohl nix
  return nil;
}