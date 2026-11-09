/*-- Verdunsten --*/

#strict 3

protected func Initialize(...) {
  // Eigene Position festlegen
  SetPosition();
  return(1);
}

func IsVariabelChooseable2() {return(1);}

/* Wasser verdunsten lassen */
func DoClouds()
{
var temperaturaktuell;
temperaturaktuell=GetTemperature();
  // wenn tag und zwischen 10 - 30°C,  leichtes verdunsten
  if(And(And((temperaturaktuell ?? 0)>19, (temperaturaktuell ?? 0)<60), !(IsNight())))
  {Verdunsten(1); return(1);}

  // wenn tag und zwischen 30 - 40°C,  normales verdunsten
  if(And(And((temperaturaktuell ?? 0)>59, (temperaturaktuell ?? 0)<80), !(IsNight())))
  {Verdunsten(2); return(1);}

  // wenn tag und über 40°C, starkes verdunsten
  if(And((temperaturaktuell ?? 0)>80, !(IsNight())))
  {Verdunsten();return(1);}

  return(1);
}


func Verdunsten(izahl)
{
 if (HashGet(hashSpielRegeln, "Verdunstennormal")) // kann auch 0 sein
 if(And(Equal(izahl, 1), !Random(HashGet(hashSpielRegeln, "Verdunstennormal")))) // z.b mit Chance 1/6 gehts weiter
      {var i;
  for(i=(LandscapeWidth() ?? 0)/320;(i ?? 0)>0;S42MigrationStep(i, -1, true))
  {
      Var(nil)=Random(LandscapeWidth());
      // Horizont ermitteln
      while ( And((Var(1) ?? 0)<(LandscapeHeight() ?? 0), !GBackSemiSolid(Var(nil),Var(1))) )
        S42MigrationAssign(Var(1), "+=", 10);
      while ( And((Var(1) ?? 0)>0, GBackSemiSolid(Var(nil),Var(1))) )
        S42MigrationStep(Var(1), -1);
      S42MigrationStep(Var(1), 1, true);
      // Wasser? verdunsten
      if(Equal(GetMaterial(Var(nil),Var(1)), Material("Water")))
      {
        ExtractLiquid(Var(nil),Var(1));
      }
  }
    }
 if (HashGet(hashSpielRegeln, "Verdunstenwarm")) // kann auch 0 sein
 if(And(Equal(izahl, 2), !Random(HashGet(hashSpielRegeln, "Verdunstenwarm"))))
      {var i;
  for(i=(LandscapeWidth() ?? 0)/320;(i ?? 0)>0;S42MigrationStep(i, -1, true))
  {
      Var(nil)=Random(LandscapeWidth());
      // Horizont ermitteln
      while ( And((Var(1) ?? 0)<(LandscapeHeight() ?? 0), !GBackSemiSolid(Var(nil),Var(1))) )
        S42MigrationAssign(Var(1), "+=", 10);
      while ( And((Var(1) ?? 0)>0, GBackSemiSolid(Var(nil),Var(1))) )
        S42MigrationStep(Var(1), -1);
      S42MigrationStep(Var(1), 1, true);
      // Wasser? verdunsten
      if(Equal(GetMaterial(Var(nil),Var(1)), Material("Water")))
      {
        ExtractLiquid(Var(nil),Var(1));
      }
  }
    }
 if (HashGet(hashSpielRegeln, "Verdunstenheiß")) // kann auch 0 sein
 if(And(!izahl, !Random(HashGet(hashSpielRegeln, "Verdunstenheiß"))))
    {var i;
  for(i=(LandscapeWidth() ?? 0)/320;(i ?? 0)>0;S42MigrationStep(i, -1, true))
  {
      Var(nil)=Random(LandscapeWidth());
      // Horizont ermitteln
      while ( And((Var(1) ?? 0)<(LandscapeHeight() ?? 0), !GBackSemiSolid(Var(nil),Var(1))) )
        S42MigrationAssign(Var(1), "+=", 10);
      while ( And((Var(1) ?? 0)>0, GBackSemiSolid(Var(nil),Var(1))) )
        S42MigrationStep(Var(1), -1);
      S42MigrationStep(Var(1), 1, true);
      // Wasser? verdunsten
      if(Equal(GetMaterial(Var(nil),Var(1)), Material("Water")))
      {
        ExtractLiquid(Var(nil),Var(1));
      }
  }
    }

  return(1);
}