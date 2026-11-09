/*-- Barierre --*/

#strict

local ticker, barierre;


func Initialize()
{
SetPosition(1,1);
if(!HashGet(hashSpielRegeln, "Barriere")) return(RemoveObject()); // nur aktiv werden, wenn variabel eingestellt
barierre = CreateObject(_PER, LandscapeWidth()/2, 0,-1);
ticker = CreateObject(CLK_, 1, 1,-1);
ticker ->~ DieSettings(100,RGBa(240,1,1,250),0,this()); // Zeit wird jetzt über hastable variable bestimmt
}


func Zeitvorbei() // wird vom ticker aufgerufen, wenn Zeit vorbei
{
// hier evtl noch sobald zeit vorbei bei jedem spieler der das EJD erforscht hat, ein paar kaufbar machen, denn da man sich damit evlt durch die barierre porten kann, sollte es nicht von beginn an kaufbar sein.
//DoHomebaseMaterial(GetOwner(pClonk), GetID(pClonk), 1);
GameCall("Losgehts");
barierre ->~Disable();
RemoveObject(ticker);
RemoveObject(barierre);
RemoveObject(this());
return();
}

func Activate(int iPlayer)
{
  MessageWindow("Erzeugt eine Sperre in der Mitte der landschaft, um zu verhindern, das sich die Gegner vor Ablauf der Zeit einen Besuch abstatten",iPlayer);
  return(1);
}

// Kann mittels des neuen Spielzielauswählers variabel eingestellt werden
public func IsVariabelChooseable2() { return(1); }


func Destruction()
{
// if(FindObject(_PER))
// RemoveAll(_PER);
// if(FindObject(CLK_))
// RemoveAll(CLK_);
if(barierre) {RemoveObject(barierre);}
if(ticker) RemoveObject(ticker);
}

