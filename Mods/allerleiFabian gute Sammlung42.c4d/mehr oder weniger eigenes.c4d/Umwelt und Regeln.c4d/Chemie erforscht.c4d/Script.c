/*-- Chemie Forschungen --*/

#strict 3

func Initialize()
{
SetPosition(1,1);
}


func Activate()
{
  MessageWindow(GetDesc(),Par());
  return(1);
}

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
public func IsChooseable2() { return(1); }

func ChemieForschungen() // dies wird im settings objekt aufgerufen. Kann durch Entfernung der Regel nicht mehr rückgängig gemacht werden, außer wenn keine SzenarioForschung aktiv und das Startzeitalter verändert wird!
{
    return([ERER,EAW2,SAER,C11B,F_0C,L11B,SPCO,CRCO,EAWL,GLAS,OILR,CHEM,CHPI,FH9I,FLNT,GOL5]);
} // Erz aus Erde , gr. lehmklumpen , Sand aus Erde, Beton , Säure und Erde zu Erz, fl. Granit, Öl zu KohleSchwefel, Hohle zu Kristall, Lehmklumpen, Glas, Öl aus Kohle, Chemiefabrik, Kohlenmeiler,Forsthaus, Flint, 5erGold, 