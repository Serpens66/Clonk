/*-- Verdunsten --*/

#strict

protected Initialize:
  // Eigene Position festlegen
  SetPosition();
  return(1);

func IsVariabelChooseable2() {return(1);}

/* Wasser verdunsten lassen */
func DoClouds()
{
var temperaturaktuell;
temperaturaktuell=GetTemperature();
  // wenn tag und zwischen 10 - 30°C,  leichtes verdunsten
  if(temperaturaktuell>19 && temperaturaktuell<60 && !(IsNight()))  
  {Verdunsten(1); return(1);}

  // wenn tag und zwischen 30 - 40°C,  normales verdunsten
  if(temperaturaktuell>59 && temperaturaktuell<80 && !(IsNight()))  
  {Verdunsten(2); return(1);}

  // wenn tag und über 40°C, starkes verdunsten
  if(temperaturaktuell>80 && !(IsNight()))  
  {Verdunsten();return(1);}

  return(1);
}


func Verdunsten(izahl)
{
 if (HashGet(hashSpielRegeln, "Verdunstennormal")) // kann auch 0 sein
 if(izahl==1 && !Random(HashGet(hashSpielRegeln, "Verdunstennormal"))) // z.b mit Chance 1/6 gehts weiter
      {var i;
  for(i=LandscapeWidth()/320;i>0;i--)
  {
      Var(0)=Random(LandscapeWidth());
      // Horizont ermitteln
      while ( Var(1)<LandscapeHeight() && !GBackSemiSolid(Var(0),Var(1)) )
        Var(1)+=10;
      while ( Var(1)>0 && GBackSemiSolid(Var(0),Var(1)) )
        --Var(1);
      Var(1)++;
      // Wasser? verdunsten
      if(GetMaterial(Var(0),Var(1)) == Material("Water"))
      {
        ExtractLiquid(Var(0),Var(1));
      }
  }
    }
 if (HashGet(hashSpielRegeln, "Verdunstenwarm")) // kann auch 0 sein
 if(izahl==2 && !Random(HashGet(hashSpielRegeln, "Verdunstenwarm")))
      {var i;
  for(i=LandscapeWidth()/320;i>0;i--)
  {
      Var(0)=Random(LandscapeWidth());
      // Horizont ermitteln
      while ( Var(1)<LandscapeHeight() && !GBackSemiSolid(Var(0),Var(1)) )
        Var(1)+=10;
      while ( Var(1)>0 && GBackSemiSolid(Var(0),Var(1)) )
        --Var(1);
      Var(1)++;
      // Wasser? verdunsten
      if(GetMaterial(Var(0),Var(1)) == Material("Water"))
      {
        ExtractLiquid(Var(0),Var(1));
      }
  }
    }
 if (HashGet(hashSpielRegeln, "Verdunstenheiß")) // kann auch 0 sein
 if(!izahl && !Random(HashGet(hashSpielRegeln, "Verdunstenheiß"))) 
    {var i;
  for(i=LandscapeWidth()/320;i>0;i--)
  {
      Var(0)=Random(LandscapeWidth());
      // Horizont ermitteln
      while ( Var(1)<LandscapeHeight() && !GBackSemiSolid(Var(0),Var(1)) )
        Var(1)+=10;
      while ( Var(1)>0 && GBackSemiSolid(Var(0),Var(1)) )
        --Var(1);
      Var(1)++;
      // Wasser? verdunsten
      if(GetMaterial(Var(0),Var(1)) == Material("Water"))
      {
        ExtractLiquid(Var(0),Var(1));
      }
  }
    }

  return(1);
}